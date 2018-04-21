#pragma once

#include <map>
#include <memory>
#include <thread>
#include <vector>

#include "Connection.hpp"
#include "Server.hpp"

namespace OpenSofa {
namespace io {

class TCPServer : public Server {
public:
  TCPServer(unsigned short port = 9876);
  void start();
  void stop();
  void setConnectionListener(const Server::OnConnectedCallback& conn, const Server::OnDisconnectedCallback& disc);
  std::map<unsigned int, Connection::Ptr> getConnections() const;

private:
  struct ConnectionListener {
    const Server::OnConnectedCallback onConnected;
    const Server::OnDisconnectedCallback onDisconnected;

    ConnectionListener(const Server::OnConnectedCallback& conn, const Server::OnDisconnectedCallback& disc)
      : onConnected(conn), onDisconnected(disc)
    {
    }
  };

  void startAccept();
  void stopAccept();
  void runAccept();
  void accept(unsigned int hAccept);
  unsigned short port;
  int listenSocket;
  std::optional<ConnectionListener> connectionListener_;
  std::unique_ptr<std::thread> acceptThread_;
  bool acceptThreadRunning_;
  std::map<unsigned int, Connection::Ptr> connections_;
  std::size_t sendNetwork(const uint8_t* buf, std::size_t count, unsigned int dst);
  std::size_t recvNetwork(uint8_t* buf, std::size_t count, unsigned int dst);
};
}}
