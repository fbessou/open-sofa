#include "BufferedConnection.hpp"
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
      : clientConnected(false), clientId(0), server(server), cnx(nullptr)
  {
  }
  void onConnected(unsigned int id)
  {
    cout << "Client has joined!" << endl;
    clientId = id;
    cnx = new BufferedConnection(*server->getConnections()[clientId]);
    clientConnected = true;
  }
  void onDisconnected(unsigned int id)
  {
    cout << "Client has left!" << endl;
    if (id == clientId)
      clientConnected = false;
  }

  bool clientConnected;
  unsigned int clientId;
  Server* server;
  Connection* cnx;
};

std::unique_ptr<Server> srv;
std::shared_ptr<CallbackConnection> cc;

void initServer(unsigned short port)
{
  srv.reset(new TCPServer{ port });
  srv->start();
  cc.reset(new CallbackConnection(srv.get()));
  srv->setConnectionListener(cc);
}

int main(int argc, char** argv)
{
  unsigned short port;
  if(argc > 1) {
    port = (unsigned short) atoi(argv[1]);
  }
  else {
    cout << "Server port: " << endl;
    cin >> port;
  }
  initServer(port);
  string line;
  uint8_t buf[1024];
  while (getline(std::cin, line)) {
    if (line.size() && cc->clientConnected) {
      auto cnx = cc->cnx;
      cnx->getOutputStream().write((const uint8_t*)line.c_str(), line.size());
    }
    if (cc->clientConnected) {
      auto cnx = cc->cnx;
      int size = cnx->getInputStream().read(buf, 1024 - 1);
      buf[size] = 0;
      cout << "\e[33m\"" << (char*)buf << "\"\e[0m" << endl;
    }
  }

  return 0;
}
