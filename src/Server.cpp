#include "Server.hpp"

#include <utility>

using namespace OpenSofa;
using namespace std::placeholders;

Server::Server(unsigned short port, const std::string& uuid)
  : tcpServer_(port), uuid(uuid)
{
}

void Server::start()
{
  tcpServer_.setConnectionListener(
      std::bind(&Server::onClientConnected, this, _1),
      std::bind(&Server::onClientDisconnected, this, _1));
  tcpServer_.start();
}

void Server::onClientConnected(unsigned int id, const Connection::Ptr& connection)
{
  clientThread = std::thread(&Server::clientLoop, this, connection);
}

void Server::onClientDisconnected(unsigned int id)
{
}

bool Server::send(const std::string& uuid, const msgpack::object_handle& message)
{
  auto outputStream = outputStreams_.find(uuid);
  if (outputStream == outputStreams_.end())
    return false

  outputStreams_->write(message);
  return true;
}

void Server::clientLoop(const Connection::Ptr& connection)
{
//  auto inputStream = io::ObjectHandleInputStream(connection->getInputStream());
//  auto event = ServerEvent(uuid, Event(event::Connected());
//  outputStream_.emplace(connection->getOutputStream());
//  events_.push(std::move(event));
//  int clientState = 0;
//  while(true) {
//      switch(clientState):
//        case 0:
//          outputStream.write("Hey ! What is your UUID ?");
//          auto uuid = inputStream.read();
//          clientState = 1;
//          break;
//        case 1:
//          auto objectHandle = inputStream.read();
//          events_.push(Event(event::Message(std::move(objectHandle))));
//    }
//  }
}

std::optional<Event> Server::pollEvent()
{
  Event event;
  if (events_.peek(event)) {
    return std::optional<Event>(std::move(event));
  }
  return {};
}
