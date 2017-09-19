#include <gtest/gtest.h>
#include "TCPServer.hpp"

using namespace OpenSofa;

namespace
{
  TEST(TCPServerTest, CanBeCreated)
  {
    Server * server = new TCPServer();
    delete server;
  }
}
