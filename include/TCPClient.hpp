#include "Client.hpp"
#include "Connection.hpp"

namespace OpenSofa {

class TCPClient : public Client{
public:
  TCPClient(const std::string& hostname, unsigned short port = 9876);
  std::shared_ptr<Connection> connect();
private:
  const std::string& resolve_ip();

  std::string hostname_;
  unsigned short port_; 
  std::size_t sendNetwork(const uint8_t* buf, std::size_t count, unsigned int dst);
  std::size_t recvNetwork(uint8_t* buf, std::size_t count, unsigned int dst);
};
}
