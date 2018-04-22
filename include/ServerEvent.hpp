#pragma once

#include "Event.hpp"

#include <string>

namespace OpenSofa {
struct ServerEvent {
  
  ServerEvent() = default;

  //! T is Event or Event's alternatives
  template<typename T>
  ServerEvent(const std::string& uuid, T&& event)
    : clientUUID(uuid),
    event(std::move(event))
  {
  }

  ServerEvent(ServerEvent&& se) = default;

  ServerEvent& operator=(ServerEvent&& se) = default;

  std::string clientUUID;
  Event event;
};
}
