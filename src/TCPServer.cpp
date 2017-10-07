#include "TCPServer.hpp"
#include "NetworkConnection.hpp"
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace OpenSofa {

TCPServer::TCPServer(unsigned short port) : port(port)
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

void TCPServer::setConnectionListener(const std::shared_ptr<ConnectionListener>& listener)
{
  connectionListener_ = listener;
}

std::map<unsigned int, Connection::Ptr> TCPServer::getConnections() const
{
  return connections_;
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
  using namespace std::placeholders;

  auto sendFunc = std::bind(&TCPServer::sendNetwork, this, _1, _2, hAccept);
  auto recvFunc = std::bind(&TCPServer::recvNetwork, this, _1, _2, hAccept);
  connections_[hAccept].reset(new NetworkConnection(sendFunc, recvFunc));

  connectionListener_->onConnected(hAccept);
}

std::size_t TCPServer::sendNetwork(const uint8_t* buf, std::size_t count, unsigned int dst)
{
  ::send(dst, buf, count, 0);
}

std::size_t TCPServer::recvNetwork(uint8_t* buf, std::size_t count, unsigned int dst)
{
  ssize_t rs = ::recv(dst, buf, count, 0);
  // FIXME Handle error where rs < 0
  return rs;
}
}
