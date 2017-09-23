#include "TCPServer.hpp"
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <functional>

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
    printf("bind() error: (%d) %s\n", errno, strerror(errno));
  }
  if (listenSocket == -1) {
    printf("socket() error: (%d) %s\n", errno, strerror(errno));
  }
  if (listen(listenSocket, 100) == -1) {
    printf("listen() error: (%d) %s\n", errno, strerror(errno));
  }
  startAccept();
}

void TCPServer::stop()
{
  stopAccept();
  close(listenSocket);
}

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
    int hAccept = ::accept(listenSocket, nullptr, nullptr);
    if (hAccept != -1)
      accept(hAccept);
  }
}

void TCPServer::stopAccept()
{
  acceptThreadRunning_ = false;
  acceptThread_->join();
  acceptThread_.reset(nullptr);
}

void TCPServer::accept(unsigned int hAccept)
{
  // see std::bind
  connections_[hAccept] = TCPConnectionPtr(new TCPConnection(
        std::bind(&TCPServer::sendNetwork, this, std::placeholders::_1, hAccept),
        std::bind(&TCPServer::recvNetwork, this, std::placeholders::_1, hAccept)));

  connectionListener_->onConnected(hAccept);
}

void TCPServer::sendNetwork(const RawBuffer& buffer, unsigned int dst)
{
  ::send(dst, buffer.getData(), buffer.length, 0);
}

bool TCPServer::recvNetwork(RawBuffer& buffer, unsigned int dst)
{
  std::shared_ptr<void> data = std::shared_ptr<void>(new char[RawBuffer::MaxLength]);
  ssize_t rs = ::recv(dst, data.get(), RawBuffer::MaxLength, 0);
  
  if (rs > 0) {
    buffer.data = data;
    buffer.length = rs;
    return true;
  }

  return false;
}

}
