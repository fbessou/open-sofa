#include "Client.hpp"
#include "Message.hpp"

#include <utility>

using namespace OpenSofa;

Client::Client(const std::string& host, unsigned short port, const std::string& uuid)
  : tcpClient_(host, port), uuid(uuid)
{
}

void Client::start()
{
  thread_ = std::thread(&Client::loop, this);
}

void Client::loop()
{
  auto connection = tcpClient_.connect();
  auto inputStream = io::ObjectHandleInputStream(connection->getInputStream());
  auto event = Event(event::Connected());
  outputStream_.emplace(connection->getOutputStream());
  events_.push(std::move(event));

  RegisterMsg registerMsg = {"Bob"};
  std::unique_ptr<msgpack::zone> zone(new msgpack::zone);
  msgpack::object obj(registerMsg, *zone);
  msgpack::object_handle objh(obj, std::move(zone));

  
  while(true) {
    auto objectHandle = inputStream.read();
    events_.push(Event(event::Message(std::move(objectHandle))));
  }
}

std::optional<Event> Client::pollEvent()
{
  Event event;
  if (events_.peek(event)) {
    return std::optional<Event>(std::move(event));
  }
  return {};
}
