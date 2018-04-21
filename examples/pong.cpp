#include "io/BufferedConnection.hpp"
#include "io/TCPServer.hpp"
#include <algorithm>
#include <cstdio>
#include <unistd.h>
using namespace std;
using namespace OpenSofa::io;

vector<Connection*> cnx;
void onConnected(unsigned int id, const std::shared_ptr<Connection>& cc)
{
  printf("\a\n");
  cnx.push_back(new BufferedConnection(*cc)); // ! \\ unreferencing shared_ptr !?
}
int main(int argc, char** argv)
{
  TCPServer srv(atoi(argv[1]));
  srv.setConnectionListener(onConnected, [](unsigned int) {});
  srv.start();
  bool ended = false;
  const int pw = 5;
  int h = 20, w = 50;
  int p1 = h / 2, p2 = h / 2;
  int bx = 1, by = h / 2;
  int bvx = 1, bvy = 1;

  while (!ended) {
    volatile int up1 = 0, down1 = 0, up2 = 0, down2 = 0;
    system("clear");
    bool a = true;
    while (a) {
      a = false;
      for (auto& c : cnx) {
        uint8_t buf[2];
        int s = c->getInputStream().read(buf, 2);
        if (!s)
          continue;
        *(&up1 + buf[0] + buf[1] * 2) = 1;
        *(&up1 + (1 - buf[0]) + buf[1] * 2) = 0;
        a = true;
      }
    }
    p1 = min(max(pw / 2 + 1, p1 - up1 + down1), h - 1 - pw / 2);
    p2 = min(max(pw / 2 + 1, p2 - up2 + down2), h - 1 - pw / 2);
    bx += bvx;
    by += bvy;
    if (by == -1)
      by = bvy = 1;
    else if (by == h)
      by = h + 2 * (bvy = -1);
    if (bx == 0)
      bvx = bx = 1;
    else if (bx == w - 1)
      bx = w + 2 * (bvx = -1);

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        if (x == 0 && abs(y - p1) <= pw / 2)
          printf("\e[31;1m|\e[0m");
        else if (x == w - 1 && abs(y - p2) <= pw / 2)
          printf("\e[32;1m|\e[0m");
        else if (x == bx && y == by)
          printf("\e[33;1m@\e[0m");
        else
          printf("\e[37;2m+\e[0m");
      }
      printf("\n");
    }
    usleep(1000 * 100 * 1);
  }

  return 0;
}
