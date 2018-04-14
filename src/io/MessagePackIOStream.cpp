#include "io/MessagePackIOStream.hpp"

using namespace OpenSofa::io;

ObjectHandleInputStream::ObjectHandleInputStream(ByteInputStream& is)
    : bis(is),
      unpacker([](msgpack::type::object_type, uint64_t, void*) { return false; }, nullptr, 1024)
{
}

msgpack::object_handle ObjectHandleInputStream::read()
{
  msgpack::object_handle result;

  while (!unpacker.next(result)) {
    unpacker.reserve_buffer(1024);
    std::size_t actual_size
        = bis.read(reinterpret_cast<uint8_t*>(unpacker.buffer()), unpacker.buffer_capacity());

    unpacker.buffer_consumed(actual_size);
  }

  return result;
}

ObjectHandleOutputStream::ObjectHandleOutputStream(ByteOutputStream& os) : bos(os)
{
}

size_t ObjectHandleOutputStream::write(const msgpack::object_handle& objh)
{
  msgpack::pack(*this, objh.get());
  return 1;
}

size_t ObjectHandleOutputStream::write(const char* buf, size_t s)
{
  return bos.write(reinterpret_cast<const uint8_t*>(buf), s);
}
