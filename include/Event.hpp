#pragma once

#include <msgpack.hpp>

#include <variant>

namespace OpenSofa{
namespace event {
  struct Connected{};
  struct Disconnected{};
  struct Message {
    public:
      Message(msgpack::object_handle&& obj);
    private:
      msgpack::object_handle obj_;
  };
}

typedef std::variant<event::Connected, event::Disconnected, event::Message> Event;
}
