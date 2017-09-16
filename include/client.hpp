#pragma once
#include <string>

namespace OpenSofa
{

  class Client
  {
    public:
      class ConnectionListener
      {
        public:
          virtual void onConnected() = 0;
          virtual void onDisconnected() = 0;
      };

      virtual void start() = 0;
      virtual void stop() = 0;
      virtual void send(const void* buffer, std::size_t bufferSize) = 0;
      virtual void setConnectionListener(ConnectionListener* listener)  = 0;
  };
}
