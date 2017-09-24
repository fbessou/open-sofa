#pragma once

#include "Connection.hpp"
#include <functional>
#include <memory>
#include <cstddef>
#include <cstdint>

namespace OpenSofa {

class NetworkConnection : public Connection {
public:
  class ByteInputStream : public OpenSofa::ByteInputStream {
    public:
      size_t read(uint8_t* buf, std::size_t count);
      void close() { }
  };
  class ByteOutputStream : public OpenSofa::ByteOutputStream {
    public:
      size_t write(const uint8_t* buf, std::size_t count);
      void close() { }
  };
  typedef std::shared_ptr<NetworkConnection> Ptr;

  typedef std::function<void(const uint8_t* buf, std::size_t count)> SendFunc;
  typedef std::function<size_t(uint8_t* buf, std::size_t count)> RecvFunc;

  NetworkConnection(const SendFunc& sendFunc, const RecvFunc& recvFunc);

  OpenSofa::ByteInputStream& getInputStream();
  OpenSofa::ByteOutputStream& getOutputStream();

private:
  SendFunc send_;
  RecvFunc recv_;
  ByteInputStream inputStream_;
  ByteOutputStream outputStream_;
};


}
