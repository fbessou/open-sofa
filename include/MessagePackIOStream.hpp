#pragma once

#include "ByteIOStream.hpp"

#include <cstdlib>
#include <msgpack.hpp>

namespace OpenSofa {
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
  size_t write(const msgpack::object_handle&);
private:
  ByteOutputStream& bos;
};
}
