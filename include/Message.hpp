#pragma once

#include <msgpack.hpp>

enum MSG_TYPE {
  REGISTER = 0,
  REGISTERED = 1,
  UNREGISTER = 2,
  DISCONNECT = 3,
  PING = 4,
  USER = 5,
};



struct RegisterMsg
{
  std::string uuid;

  MSGPACK_DEFINE_MAP(uuid);
};

class MessageFactory
{
  public:
    const msgpack::object_handle& createRegistered();
};
