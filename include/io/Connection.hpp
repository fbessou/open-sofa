#pragma once

#include "ByteIOStream.hpp"
#include <memory>

namespace OpenSofa {
namespace io {

class Connection {
public:
  typedef std::shared_ptr<Connection> Ptr;

  virtual ByteInputStream& getInputStream() = 0;
  virtual ByteOutputStream& getOutputStream() = 0;
};
}}
