#include "TCPServer.hpp"
#define SOCKET int
#define SOCKET_ERRNO errno
#define ERRNO errno
#define closesocket close
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace OpenSofa {

TCPServer::TCPServer(unsigned short port)
    : port(port)
{
}

void TCPServer::start()
{
  struct sockaddr_in addr;
  int len = sizeof(addr);
  int nPort = port;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons((unsigned short)port);
  listenSocket = socket(PF_INET, SOCK_STREAM, 0);
  if (bind(listenSocket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    printf("bind() error %d\n", SOCKET_ERRNO);
  }
  if (listenSocket == -1) {
    printf("socket() error %d\n", SOCKET_ERRNO);
  }
  if (listen(listenSocket, 100) == -1) {
    printf("listen() error %d\n", SOCKET_ERRNO);
    exit(1);
  }
  startAccept();
}

void TCPServer::stop() { stopAccept(); }

void TCPServer::send(const RawBuffer& buffer, unsigned int dst)
{
  if (connections_.count(dst))
    connections_[dst]->send(buffer);
}

bool TCPServer::recv(RawBuffer& buffer, unsigned int dst, unsigned int timeOutMS)
{
  if (connections_.count(dst))
    return connections_[dst]->recv(buffer, timeOutMS);
  return false;
}

bool TCPServer::recv(RawBuffer& buffer, unsigned int dst)
{
  if (connections_.count(dst))
    return connections_[dst]->recv(buffer);
  return false;
}

void TCPServer::setConnectionListener(const std::shared_ptr<ConnectionListener>& listener)
{
  connectionListener_ = listener;
}

void TCPServer::startAccept()
{
  acceptThreadRunning_ = true;
  acceptThread_.reset(new std::thread(&TCPServer::runAccept, this));
}

void TCPServer::runAccept()
{
  while (acceptThreadRunning_) {
    // select(...) # <sys/select.h>
    unsigned short hAccept = ::accept(listenSocket, nullptr, nullptr);
    if (hAccept != (unsigned short)0xffff)
      accept(hAccept);
  }
}

void TCPServer::stopAccept()
{
  acceptThreadRunning_ = false;
  acceptThread_->join();
  acceptThread_.reset(nullptr);
}

void TCPServer::accept(unsigned short hAccept)
{
  // see std::bind
  connections_[hAccept] = TCPConnectionPtr(new TCPConnection(
      [hAccept](const RawBuffer& buffer) { ::send(hAccept, buffer.getData(), buffer.length, 0); },
      [hAccept](RawBuffer& buffer) -> bool {
        std::shared_ptr<void> data = std::shared_ptr<void>(new char[RawBuffer::MaxLength]);
        ssize_t rs = ::recv(hAccept, data.get(), RawBuffer::MaxLength, 0);
        if (rs > 0) {
          buffer.data = data;
          buffer.length = rs;
          return true;
        } else {
          return false;
        }
      }));
  connectionListener_->onConnected(hAccept);
}
}
