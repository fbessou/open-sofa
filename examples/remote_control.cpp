#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "io/BufferedConnection.hpp"
#include "io/TCPClient.hpp"

using namespace OpenSofa::io;

enum class Key {
  UP,
  DOWN,
  UNKNOWN,
};

int getInput()
{
  fd_set set;
  struct timeval tv;

  FD_ZERO(&set);
  FD_SET(fileno(stdin), &set);

  int res = select(fileno(stdin) + 1, &set, NULL, NULL, NULL);

  if (res > 0) {
    char c;
    read(fileno(stdin), &c, 1);
    return c;
  } else if (res < 0) {
    perror("select error");
  } else {
    printf("Select timeout\n");
  }
}

Key getKey()
{
  char c = getInput();
  switch (c) {
  case 'z':
    return Key::UP;
  case 's':
    return Key::DOWN;
  default:
    printf("no input");
    return Key::UNKNOWN;
  }
}

std::tuple<std::string, unsigned short> getRemoteService(int argc, char** argv)
{
  if (argc > 2) {
    return std::make_pair<>(argv[1], atoi(argv[2]));
  } else {
    std::string hostname;
    unsigned short port;
    std::cout << "Server hostname/IP" << std::endl;
    std::cin >> hostname;
    std::cout << "Port" << std::endl;
    std::cin >> port;
    std::make_pair(hostname, port);
  }
}

std::unique_ptr<Client> client;

int main(int argc, char** argv)
{
  struct termios oldSettings, newSettings;

  tcgetattr(fileno(stdin), &oldSettings);
  newSettings = oldSettings;
  newSettings.c_lflag &= (~ICANON & ~ECHO);
  tcsetattr(fileno(stdin), TCSANOW, &newSettings);
  auto service = getRemoteService(argc, argv);
  printf("Test\n");
  client.reset(new TCPClient(std::get<0>(service), std::get<1>(service)));
  auto cnx_nobuf = client->connect();
  auto cnx = new BufferedConnection(*cnx_nobuf);
  uint8_t player = atoi(argv[3]);
  printf("Test2\n");
  while (true) {
    uint8_t dir;
    uint8_t msg[2] = { dir, player };
    switch (getKey()) {
    case Key::UP:
      dir = 0;
      break;
    case Key::DOWN:
      dir = 1;
      break;
    default:
      continue;
    }
    // send direction
    printf("%d\n", dir);
    cnx->getOutputStream().write(msg, 2);
  }

  tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
  return 0;
}
