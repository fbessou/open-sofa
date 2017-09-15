#include <gtest/gtest.h>
#include "tcpserver.hpp"

using namespace OpenSofa;

namespace
{
  TEST(TCPServerTest, CanBeCreated)
  {
    Server * server = new TCPServer();
    delete server;
  }
}
