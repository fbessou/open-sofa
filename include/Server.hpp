#pragma once

#include "RawBuffer.hpp"
#include <chrono>
#include <memory>

namespace OpenSofa {

class Server {
public:
  class ConnectionListener {
  public:
    virtual void onConnected(unsigned int id) = 0;
    virtual void onDisconnected(unsigned int id) = 0;
  };

  //! Start server and associated components
  virtual void start() = 0;

  //! Stop server and associated components
  virtual void stop() = 0;

  //! Send a message to the given destination
  virtual void send(const RawBuffer& buffer, unsigned int dst) = 0;

  //! Wait for a message from dst with timeout in milliseconds
  virtual bool recv(RawBuffer& buffer, unsigned int dst, unsigned int timeOutMS) = 0;

  //! Give a pending message from dst if any, immediately return otherwise
  virtual bool recv(RawBuffer& buffer, unsigned int dst) = 0;

  //! Set a listener for new connections
  virtual void setConnectionListener(const std::shared_ptr<ConnectionListener>& listener) = 0;
};
}
