#include "Message.hpp"

namespace OpenSofa
{
const object_handle & createRegistered() {
  msgpack::object obj(REGISTER);
  msgpack::object_handle hdl(obj);
  return hdl;
}
}
