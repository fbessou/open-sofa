#include "io/TCPClient.hpp"

#include "io/BufferedConnection.hpp"
#include <iostream>

using namespace OpenSofa::io;

std::unique_ptr<Client> client;
int main(int argc, char** argv)
{
  uint8_t buf[1024];
  std::string line;

  std::string host;
  unsigned short port;
  if(argc > 2) {
    host = argv[1];
    port = (unsigned short) atoi(argv[2]);
  }
  else {
    std::cout << "Server hostname/IP" << std::endl;
    std::cin >> host;
    std::cout << "Port" << std::endl;
    std::cin >> port;
  }
  client.reset(new TCPClient(host, port));
  auto cnx_nobuf = client->connect();
  auto cnx = new BufferedConnection(*cnx_nobuf);
  while (getline(std::cin, line)) {
    cnx->getOutputStream().write((const uint8_t*)line.c_str(), line.size());
    int size = cnx->getInputStream().read(buf, 1024 - 1);
    buf[size] = 0;
    std::cout << "\e[33m\"" << (char*)buf << "\"\e[0m" << std::endl;
  }
}
