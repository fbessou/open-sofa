#include "io/BufferedConnection.hpp"
#include "io/TCPServer.hpp"
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

using namespace std;
using namespace OpenSofa::io;


bool clientConnected = false;
unsigned int clientId = 0;
std::unique_ptr<Connection> cnx;
std::unique_ptr<Server> srv;

void onConnected(unsigned int id, const std::shared_ptr<Connection>& cc)
{
  cout << "Client has joined!" << endl;
  clientId = id;
  cnx.reset(new BufferedConnection(*cc)); // ! \\ unreferencing shared_ptr !?
  clientConnected = true;
}
void onDisconnected(unsigned int id)
{
  cout << "Client has left!" << endl;
  if (id == clientId)
    clientConnected = false;
}

void initServer(unsigned short port)
{
  srv.reset(new TCPServer{ port });
  srv->start();
  srv->setConnectionListener(onConnected, onDisconnected);
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
    if (line.size() && clientConnected) {
      cnx->getOutputStream().write((const uint8_t*)line.c_str(), line.size());
    }
    if (clientConnected) {
      int size = cnx->getInputStream().read(buf, 1024 - 1);
      buf[size] = 0;
      cout << "\e[33m\"" << (char*)buf << "\"\e[0m" << endl;
    }
  }

  return 0;
}
