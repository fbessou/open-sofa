#include "Client.hpp"

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

bool Client::send(const msgpack::object_handle& message)
{
  if (outputStream_) {
    outputStream_->write(message);
    return true;
  }
  return false;
}

void Client::loop()
{
  auto connection = tcpClient_.connect();
  auto inputStream = io::ObjectHandleInputStream(connection->getInputStream());
  auto event = Event(event::Connected());
  outputStream_.emplace(connection->getOutputStream());
  events_.push(std::move(event));
  
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
