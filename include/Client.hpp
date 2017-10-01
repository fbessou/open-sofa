#pragma once

#include "Connection.hpp"

#include <memory>

namespace OpenSofa {

class Client {
public:
  virtual std::shared_ptr<Connection> connect() = 0;
};
}
