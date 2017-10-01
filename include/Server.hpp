#pragma once

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

  //! Set a listener for new connections
  virtual void setConnectionListener(const std::shared_ptr<ConnectionListener>& listener) = 0;

  virtual std::map<unsigned int, Connection::Ptr> getConnections() const = 0;
};
}
