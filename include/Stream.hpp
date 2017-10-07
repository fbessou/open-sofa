#pragma once

#include <cstddef>
#include <cstdint>

namespace OpenSofa {

class ByteInputStream {
public:
  virtual size_t read(uint8_t* buf, std::size_t count) = 0;
  virtual void close() = 0;
};

class ByteOutputStream {
public:
  virtual size_t write(const uint8_t* buf, std::size_t count) = 0;
  virtual void close() = 0;
};
}
