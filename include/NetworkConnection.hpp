#pragma once

#include "Connection.hpp"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>

namespace OpenSofa {

class NetworkConnection : public Connection {
public:
  typedef std::shared_ptr<NetworkConnection> Ptr;

  typedef std::function<size_t(const uint8_t* buf, std::size_t count)> SendFunc;
  typedef std::function<size_t(uint8_t* buf, std::size_t count)> RecvFunc;

private:
  class NetworkByteInputStream : public ByteInputStream {
  public:
    NetworkByteInputStream(RecvFunc recv);
    size_t read(uint8_t* buf, std::size_t count);
    void close()
    {
    }

  private:
    RecvFunc recv_;
  };
  class NetworkByteOutputStream : public ByteOutputStream {
  public:
    NetworkByteOutputStream(SendFunc send);
    size_t write(const uint8_t* buf, std::size_t count);
    void close()
    {
    }

  private:
    SendFunc send_;
  };

public:
  NetworkConnection(SendFunc sendFunc, RecvFunc recvFunc);

  ByteInputStream& getInputStream();
  ByteOutputStream& getOutputStream();

private:
  NetworkByteInputStream inputStream_;
  NetworkByteOutputStream outputStream_;
};
}
