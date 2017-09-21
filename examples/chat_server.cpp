#include "TCPServer.hpp"
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

using namespace std;
using namespace OpenSofa;

class CallbackConnection : public Server::ConnectionListener {
public:
  CallbackConnection()
      : clientConnected(false)
      , clientId(0)
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
  bool clientConnected;
  unsigned int clientId;
};

std::unique_ptr<Server> srv;
std::shared_ptr<CallbackConnection> cc(new CallbackConnection);

void initServer()
{
  unsigned short port;
  cout << "Server port: " << endl;
  cin >> port;
  srv.reset(new TCPServer{ port });
  srv->start();
  srv->setConnectionListener(cc);
}

int main(int argc, char** argv)
{
  initServer();
  string line;
  while (getline(std::cin, line)) {
    if (line.size() && cc->clientConnected) {
      RawBuffer buf(line.size() + 1);
      strcpy((char*)buf.getData(), line.c_str());
      ((char*)buf.getData())[line.size()] = '\0';
      srv->send(buf, cc->clientId);
    }
    if (cc->clientConnected) {
      RawBuffer buf;
      if (srv->recv(buf, cc->clientId)) {
        const char* s = (const char*)buf.getData();
        cout << "\e[33m\"" << s << "\"\e[0m" << endl;
      }
    }
  }

  return 0;
}
