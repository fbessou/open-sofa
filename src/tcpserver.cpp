#include "tcpserver.hpp"
#define	SOCKET int
#define	SOCKET_ERRNO errno
#define	ERRNO errno
#define closesocket close
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>

namespace OpenSofa
{

  TCPServer::TCPServer(unsigned short port): port(port) {}

  void TCPServer::start()
  {
    struct sockaddr_in addr;
    int len = sizeof(addr);    
    int nPort = 1234;
    addr.sin_family = AF_INET ;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons ((unsigned short)port);
    listenSocket = socket( PF_INET, SOCK_STREAM, 0 );
    if (bind(listenSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
      printf( "bind() error %d\n", SOCKET_ERRNO );
    }
    if( listenSocket == -1 ) {
      printf( "socket() error %d\n", SOCKET_ERRNO );
    }
    if ( listen( listenSocket, 100) == -1 ) {
        printf( "listen() error %d\n", SOCKET_ERRNO );
        exit(1);
    }
    while (true) {
      unsigned short hAccept = accept(listenSocket, nullptr, nullptr);
      connectionListener->onConnected(hAccept);
    }
  }

  void TCPServer::stop()
  {
    //
  }

  void TCPServer::send(const void * buffer, std::size_t bufferSize, unsigned int dst)
  {
      ::send(dst, buffer, bufferSize, 0);
  }

  void TCPServer::setConnectionListener(ConnectionListener *listener)
  {
    connectionListener = listener;
  }
}
