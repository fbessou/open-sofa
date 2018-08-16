#pragma once

#include "ServerEvent.hpp"
#include "io/SyncQueue.hpp"
#include "io/Connection.hpp"
#include "io/TCPServer.hpp"
#include "Message.hpp"
#include "io/MessagePackIOStream.hpp"

#include <msgpack.hpp>

#include <string>
#include <optional>
#include <thread>
#include <map>


namespace OpenSofa {

class Server {
  public:
    Server(unsigned short port = 9876, const std::string & uuid = "");
    //! Started in a thread
    void start();
    //! Synchronously send a Message event
    template<typename T>
    bool send(const std::string& uuid, const T& obj)
    {
      auto outputStream = outputStreams_.find(uuid); // TODO mutex on ouputStream
      if (outputStream == outputStreams_.end())
        return false;

      // TODO wrap obj in a message with type=USER
      outputStream->second->write(obj);
      return true;
    }
    //! Synchronously read an event or nothing
    std::optional<ServerEvent> pollEvent();

  private:
    void clientLoop(const io::Connection::Ptr& connection);
    void onClientConnected(unsigned int id, const io::Connection::Ptr& connection);
    void onClientRegistered(const std::string& uuid, io::ByteOutputStream& connection);
    void onClientUnregistered(const std::string& uuid);
    void onClientDisconnected(unsigned int id);

  private:
    const std::string uuid;
    io::TCPServer tcpServer_;
    std::map<unsigned int, std::thread> threads_;
    std::map<std::string, std::unique_ptr<io::ObjectHandleOutputStream>> outputStreams_;
    io::SyncQueue<ServerEvent> events_;
    MessageFactory msg_factory;
};
}
