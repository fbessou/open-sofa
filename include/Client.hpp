#pragma once

#include "Event.hpp"
#include "io/SyncQueue.hpp"
#include "io/TCPClient.hpp"
#include "io/MessagePackIOStream.hpp"

#include <msgpack.hpp>

#include <string>
#include <optional>
#include <thread>


namespace OpenSofa {

class Client {
  public:
    Client(const std::string& host, unsigned short port = 9876, const std::string & uuid = "");
    //! Started in a thread
    void start();
    //! Synchronously send a Message event
    bool send(const msgpack::object_handle& message);
    //! Synchronously read an event or nothing
    std::optional<Event> pollEvent();

  private:
    void loop();

  private:
    const std::string uuid;
    io::TCPClient tcpClient_;
    std::thread thread_;
    std::optional<io::ObjectHandleOutputStream> outputStream_;
    io::SyncQueue<Event> events_;
};
}
