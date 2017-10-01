#include "TCPClient.hpp"

#include <iostream>

using namespace OpenSofa;

std::unique_ptr<Client> client;
int main() {
  uint8_t buf[1024];
  std::string line;

  std::string host;
  unsigned short port;
  std::cout << "Server hostname/IP" << std::endl;
  std::cin >> host;
  std::cout << "Port" << std::endl;
  std::cin >> port;
  client.reset(new TCPClient("localhost", port));
  auto cnx = client->connect();
  while (getline(std::cin, line)) {
    cnx->getOutputStream().write((const uint8_t*)line.c_str(), line.size()+1);
    int size = cnx->getInputStream().read(buf, 1024-1);
    buf[size] = 0;
    std::cout << "\e[33m\"" << (char*)buf << "\"\e[0m" << std::endl;
  }
}
