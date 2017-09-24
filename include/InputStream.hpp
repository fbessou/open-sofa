#pragma once

namespace OpenSofa {

class ByteInputStream {
public:
  size_t read(uint8_t* buf, size_t count);
  void close();
};

class ByteOutputStream {
public:
  int write(const uint8_t* buf, size_t count);
  void close();
};

}
