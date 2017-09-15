#pragma once
#include <vector>

#include "server.hpp"

namespace OpenSofa
{

  class TCPServer: public Server
  {
    public:
      TCPServer(unsigned short port = 9876);
      void start();
      void send(const void * buffer, std::size_t bufferSize, unsigned int dst);
      void setConnectionListener(ConnectionListener* listener);
    private:
      unsigned short port;
      int listenSocket;
      ConnectionListener * connectionListener;
  };
}
