#pragma once

#include "ByteIOStream.hpp"

#include <cstdlib>
#include <msgpack.hpp>

namespace OpenSofa {
namespace io {

class ObjectHandleInputStream {
public:
  ObjectHandleInputStream(ByteInputStream& is);
  msgpack::object_handle read();

private:
  ByteInputStream& bis;
  msgpack::unpacker unpacker;
};

class ObjectHandleOutputStream {
public:
  ObjectHandleOutputStream(ByteOutputStream& os);
  
  template <typename T>
  inline size_t write(const T& obj)
  { 
    msgpack::pack(*this, obj);
    return 1;
  }
  
  size_t write(const char* buf, size_t s); // UGLY!!!
private:
  ByteOutputStream& bos;
};
}}
