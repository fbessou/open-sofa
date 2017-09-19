#pragma once

#include <memory>
#include <thread>
#include <vector>
#include <map>

#include "Server.hpp"
#include "Connection.hpp"

namespace OpenSofa {

class TCPServer : public Server {
  public:
  typedef Connection<RawBuffer> TCPConnection;
  typedef std::unique_ptr<TCPConnection> TCPConnectionPtr;

  TCPServer(unsigned short port = 9876);
  void start();
  void stop();
  void send(const RawBuffer& buffer, unsigned int dst);
  bool recv(RawBuffer& buffer, unsigned int dst, unsigned int timeOutMS);
  bool recv(RawBuffer& buffer, unsigned int dst);
  void setConnectionListener(const std::shared_ptr<ConnectionListener>& listener);

  private:
  void startAccept();
  void stopAccept();
  void runAccept();
  void accept(unsigned short hAccept);

  unsigned short port;
  int listenSocket;
  std::shared_ptr<ConnectionListener> connectionListener_;
  std::unique_ptr<std::thread> acceptThread_;
  bool acceptThreadRunning_;
  std::map<unsigned short, TCPConnectionPtr> connections_;
};
}
