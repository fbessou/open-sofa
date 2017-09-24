#pragma once

#include "Stream.hpp"
#include <memory>

namespace OpenSofa {

class Connection {
public:
  typedef std::shared_ptr<Connection> Ptr;

  virtual ByteInputStream& getInputStream() = 0;
  virtual ByteOutputStream& getOutputStream() = 0;
};

}

