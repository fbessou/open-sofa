#include "MessagePackIOStream.hpp"

#include <cstring>
#include <gtest/gtest.h>

using namespace OpenSofa;

namespace {
std::vector<std::string> INPUT_VECT{"ABC", "123"};
const std::size_t INPUT_VEC_PACKED_SIZE = 9;
const uint8_t INPUT_VEC_PACKED[INPUT_VEC_PACKED_SIZE] = {
  0x92u, 0xa3u, 0x41u, 0x42u, 0x43u, 0xa3u, 0x31u, 0x32u, 0x33u
};
class BufferWrapper {
private:
   uint8_t * buffer;
   const std::size_t size;
   std::size_t cursor;
public:
  BufferWrapper(uint8_t * buffer, std::size_t size)
    : buffer(buffer)
    , size(size)
    , cursor{0} {}

  std::size_t write(const char* b, std::size_t s)
  {
    std::memcpy(buffer+cursor, b, s);
    cursor += s;
    return s;
  }

  std::size_t count() { return cursor; }
};

class DummyByteInputStream: public ByteInputStream {
private:
  bool closed = false;
public:
  std::size_t read(uint8_t* buf, std::size_t count)
  {
    if(!closed) {
    BufferWrapper bufWrapper(buf, count);
    msgpack::pack(bufWrapper, INPUT_VECT);
    closed = true;
    return bufWrapper.count();
    }
    else return 0;
  }
  void close()
  {
    closed = true;
  }
};

class DummyByteOutputStream : public ByteOutputStream {
public:
  DummyByteOutputStream() : len{0} {}
  void close() { }
  size_t write(const uint8_t* buf, std::size_t count) {
    memcpy(buffer+len, buf, count);
    len += count;
  }
  uint8_t buffer[1024];
  size_t len;
};

TEST(DummyForBIStream, CanPackVector)
{
  DummyByteInputStream is;
  uint8_t buf[64];
  auto actual_size = is.read(buf, 64);

  ASSERT_EQ(actual_size, INPUT_VEC_PACKED_SIZE) << "Pack failed";
  ASSERT_TRUE(memcmp(INPUT_VEC_PACKED, buf, actual_size) == 0) << "Pack failed";
}

TEST(ObjectHandleInputStream, CanReadFromInputStream)
{
  DummyByteInputStream is;
  OpenSofa::ObjectHandleInputStream stream(is);
  auto objh = stream.read();
  auto obj = objh.get();
  std::vector<std::string> actual;
  
  obj.convert(actual);
  ASSERT_EQ(actual, INPUT_VECT) << "Vector is not the same !! ";
}

TEST(ObjectHandleOutputStream, CanWriteToOutputStream)
{
  DummyByteOutputStream os;
  OpenSofa::ObjectHandleOutputStream stream(os);
  std::unique_ptr<msgpack::zone> zone(new msgpack::zone);
  msgpack::object obj(INPUT_VECT, *zone);
  msgpack::object_handle objh(obj, std::move(zone));

  stream.write(objh);
  auto* actual_buffer = os.buffer;
  auto actual_size = os.len;

  ASSERT_EQ(actual_size, INPUT_VEC_PACKED_SIZE);
  ASSERT_TRUE(memcmp(INPUT_VEC_PACKED, actual_buffer, actual_size) == 0);
}

}
