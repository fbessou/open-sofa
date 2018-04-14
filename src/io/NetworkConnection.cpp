#include "io/NetworkConnection.hpp"

using namespace  OpenSofa::io;

NetworkConnection::NetworkByteInputStream::NetworkByteInputStream(RecvFunc recv) : recv_(recv)
{
}

NetworkConnection::NetworkByteOutputStream::NetworkByteOutputStream(SendFunc send) : send_(send)
{
}

size_t NetworkConnection::NetworkByteInputStream::read(uint8_t* buf, std::size_t count)
{
  return recv_(buf, count);
}

size_t NetworkConnection::NetworkByteOutputStream::write(const uint8_t* buf, std::size_t count)
{
  return send_(buf, count);
}

NetworkConnection::NetworkConnection(SendFunc sendFunc, RecvFunc recvFunc)
    : inputStream_(recvFunc), outputStream_(sendFunc)
{
}

ByteInputStream& NetworkConnection::getInputStream()
{
  return inputStream_;
}

ByteOutputStream& NetworkConnection::getOutputStream()
{
  return outputStream_;
}
