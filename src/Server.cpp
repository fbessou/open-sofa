#include "Server.hpp"
#include "Message.hpp"

#include <utility>

using namespace OpenSofa;
using namespace std::placeholders;

Server::Server(unsigned short port, const std::string& uuid)
  : tcpServer_(port), uuid(uuid), msg_factory()
{
}

void Server::start()
{
  tcpServer_.setConnectionListener(
      std::bind(&Server::onClientConnected, this, _1, _2),
      std::bind(&Server::onClientDisconnected, this, _1));
  tcpServer_.start();
}

void Server::onClientConnected(unsigned int id, const io::Connection::Ptr& connection)
{
  threads_[id] = std::thread(&Server::clientLoop, this, connection);
}

void Server::onClientDisconnected(unsigned int id)
{
  const auto& thread = threads_.find(id);
  if(thread != threads_.end()) {
    threads_.erase(thread);
  }
}

void Server::onClientRegistered(const std::string& uuid, io::ByteOutputStream& byteOutputStream)
{
  outputStreams_[uuid] = std::make_unique<io::ObjectHandleOutputStream>(byteOutputStream);
}

void Server::onClientUnregistered(const std::string& uuid)
{
  outputStreams_.erase(uuid);
}

void Server::clientLoop(const io::Connection::Ptr& connection)
{
  auto inputStream = io::ObjectHandleInputStream(connection->getInputStream());
  auto event = ServerEvent(uuid, Event(event::Connected()));
  //outputStream_.emplace(connection->getOutputStream());
  events_.push(std::move(event));
  int clientState = 0;
  while(true) {
    switch(clientState) {
      case 0: { // CONNECTED
        auto objh = inputStream.read();
        auto registerMsg = objh.get().as<RegisterMsg>();
        onClientRegistered(registerMsg.uuid, connection->getOutputStream());
        send(registerMsg.uuid, msg_factory.createRegistered().get());
        clientState = 1;
        } break;
      case 1: // REGISTERED
        auto objectHandle = inputStream.read();
        //events_.push(Event(event::Message(std::move(objectHandle))));
    }
  }
}

std::optional<ServerEvent> Server::pollEvent()
{
  ServerEvent event;
  if (events_.peek(event)) {
    return std::optional<ServerEvent>(std::move(event));
  }
  return {};
}
