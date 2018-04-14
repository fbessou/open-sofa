#include "io/TCPServer.hpp"
#include <gtest/gtest.h>

using namespace OpenSofa::io;

namespace {
TEST(TCPServerTest, CanBeCreated)
{
  Server* server = new TCPServer();
  delete server;
}
}
