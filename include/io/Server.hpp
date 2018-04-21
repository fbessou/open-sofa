#pragma once

#include "io/Connection.hpp"

#include <memory>
#include <functional>

namespace OpenSofa {
namespace io {

class Server {
public:
  typedef std::function<void(unsigned int id, const Connection::Ptr&)> OnConnectedCallback;
  typedef std::function<void(unsigned int id)> OnDisconnectedCallback;

  //! Start server and associated components
  virtual void start() = 0;

  //! Stop server and associated components
  virtual void stop() = 0;

  //! Set a listener for new connections
  virtual void setConnectionListener(const OnConnectedCallback& onConnected, const OnDisconnectedCallback& onDisconnected) = 0;

  virtual std::map<unsigned int, Connection::Ptr> getConnections() const = 0;
};
}}
