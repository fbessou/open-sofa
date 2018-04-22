#pragma once

#include "ServerEvent.hpp"
#include "io/SyncQueue.hpp"
#include "io/Connection.hpp"
#include "io/TCPServer.hpp"
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
    bool send(const std::string& uuid, const msgpack::object_handle& message);
    //! Synchronously read an event or nothing
    std::optional<ServerEvent> pollEvent();

  private:
    void clientLoop(const io::Connection::Ptr& connection);
    void onClientConnected(unsigned int id, const io::Connection::Ptr& connection);
    void onClientDisconnected(unsigned int id);

  private:
    const std::string uuid;
    io::TCPServer tcpServer_;
    std::map<unsigned int, std::thread> threads_;
    std::map<std::string, io::ObjectHandleOutputStream> outputStreams_;
    io::SyncQueue<ServerEvent> events_;
};
}
