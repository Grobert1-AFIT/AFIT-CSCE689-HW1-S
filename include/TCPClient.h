#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include "Client.h"
#include "Server.h"

// The amount to read in before we send a packet
const unsigned int stdin_bufsize = 50;
const unsigned int socket_bufsize = 1024;

class TCPClient : public Client
{
public:
   TCPClient();
   ~TCPClient();

   virtual void connectTo(const char *ip_addr, unsigned short port);
   virtual void handleConnection();

   virtual void closeConn();

private:
	int sock = 0, valread;
	struct sockaddr_in serv_addr;

};


#endif
