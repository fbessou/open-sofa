#include "NetworkConnection.hpp"

namespace OpenSofa {

NetworkConnection::NetworkConnection(const SendFunc& sendFunc, const RecvFunc& recvFunc)
    : send_(sendFunc)
    , recv_(recvFunc)
{
}

OpenSofa::ByteInputStream& NetworkConnection::getInputStream()
{
  return inputStream_;
}

OpenSofa::ByteOutputStream& NetworkConnection::getOutputStream()
{
  return outputStream_;
}

size_t NetworkConnection::ByteInputStream::read(uint8_t* buf, std::size_t count)
{
  return count; //recv_(buf, count);
}

size_t NetworkConnection::ByteOutputStream::write(const uint8_t* buf, std::size_t count)
{
  //send_(buf, count);
  return count; // TODO send_ -> size_t (const uint8_t*, size_t)
}

}
