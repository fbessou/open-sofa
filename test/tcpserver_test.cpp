#include "TCPServer.hpp"
#include <gtest/gtest.h>

using namespace OpenSofa;

namespace {
TEST(TCPServerTest, CanBeCreated)
{
  Server* server = new TCPServer();
  delete server;
}
}
