#include "Event.hpp"

using namespace OpenSofa::event;

Message::Message(msgpack::object_handle&& obj)
  : obj_(std::move(obj))
{
}
