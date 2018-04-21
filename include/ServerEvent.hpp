#pragma once

#include "Event.hpp"

#include <string>

namespace OpenSofa {
struct ServerEvent {
  //! T is Event or Event's alternatives
  template<typename T>
  ServerEvent(const std::string& uuid, T&& event)
    : clientUUID(uuid),
    event(std::move(event))
  {
  }

  const std::string clientUUID;
  const Event event;
};
}
