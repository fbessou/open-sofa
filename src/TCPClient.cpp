#include "TCPClient.hpp"
#include "NetworkConnection.hpp"
#include "TCPServer.hpp"

#include <fcntl.h>
#include <functional>
#include <netdb.h>
#include <unistd.h>

namespace OpenSofa {

TCPClient::TCPClient(const std::string& hostname, unsigned short port)
    : hostname_(hostname), port_(port)
{
}

std::shared_ptr<Connection> TCPClient::connect()
{
  addrinfo hints = {
    AI_NUMERICSERV, // flags
    AF_UNSPEC, // family
    SOCK_STREAM, // socktype
    0, // protocol
    0, 0, 0, 0 // only useful for results
  };

  char str_port[6]; // maximum is "65536"
  sprintf(str_port, "%d", port_);

  addrinfo* result_list;
  int resolution_status = getaddrinfo(hostname_.c_str(), str_port, &hints, &result_list);
  if (resolution_status != 0) {
    // TODO throw an exception
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(resolution_status));
  }
  std::unique_ptr<addrinfo, decltype(freeaddrinfo)*> safe_results(result_list, &::freeaddrinfo);
  for (auto result = safe_results.get(); result != NULL; result = result->ai_next) {
    int sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sfd == -1)
      continue;

    if (::connect(sfd, result->ai_addr, result->ai_addrlen) != -1) {
      using namespace std::placeholders;
      auto sendFunc = std::bind(&TCPClient::sendNetwork, this, _1, _2, sfd);
      auto recvFunc = std::bind(&TCPClient::recvNetwork, this, _1, _2, sfd);
      return std::shared_ptr<NetworkConnection>(new NetworkConnection(sendFunc, recvFunc));
    }
    ::close(sfd);
  }
  return std::shared_ptr<NetworkConnection>(nullptr);
}

std::size_t TCPClient::sendNetwork(const uint8_t* buf, std::size_t count, unsigned int dst)
{
  ::send(dst, buf, count, 0);
}

std::size_t TCPClient::recvNetwork(uint8_t* buf, std::size_t count, unsigned int dst)
{
  ssize_t rs = ::recv(dst, buf, count, 0);
  // FIXME Handle error where rs < 0
  return rs;
}
}
