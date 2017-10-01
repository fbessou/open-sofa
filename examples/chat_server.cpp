#include "TCPServer.hpp"
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

using namespace std;
using namespace OpenSofa;

class CallbackConnection : public Server::ConnectionListener {
public:
  CallbackConnection(Server* server)
      : clientConnected(false)
      , clientId(0)
      , server(server)
  {
  }
  void onConnected(unsigned int id)
  {
    cout << "Client has joined!" << endl;
    clientId = id;
    clientConnected = true;
  }
  void onDisconnected(unsigned int id)
  {
    cout << "Client has left!" << endl;
    if (id == clientId)
      clientConnected = false;
  }

  shared_ptr<Connection>& getConnection() {
    return server->getConnections()[clientId];
  }

  bool clientConnected;
  unsigned int clientId;
  Server* server;
};

std::unique_ptr<Server> srv;
std::shared_ptr<CallbackConnection> cc;

void initServer()
{
  unsigned short port;
  cout << "Server port: " << endl;
  cin >> port;
  srv.reset(new TCPServer{ port });
  srv->start();
  cc.reset(new CallbackConnection(srv.get()));
  srv->setConnectionListener(cc);
}

int main(int argc, char** argv)
{
  initServer();
  string line;
  uint8_t buf[1024];
  while (getline(std::cin, line)) {
    if (line.size() && cc->clientConnected) {
      auto cnx = cc->getConnection();
      cnx->getOutputStream().write((const uint8_t*)line.c_str(), line.size()+1);
    }
    if (cc->clientConnected) {
	auto cnx = cc->getConnection();
	int size = cnx->getInputStream().read(buf, 1024-1);
        buf[size] = 0;
        cout << "\e[33m\"" << (char*)buf << "\"\e[0m" << endl;
    }
  }

  return 0;
}
