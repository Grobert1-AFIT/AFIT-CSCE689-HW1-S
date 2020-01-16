#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Server.h"

class TCPServer : public Server 
{
public:
   TCPServer();
   ~TCPServer();

   void bindSvr(const char *ip_addr, short unsigned int port);
   void listenSvr();
   void shutdown();
   void sendMenu(int socket);
   void sendText(int socket, char* text);
   void handleInput(int latest_conn, int index);
   unsigned long input_hash(char* str);

private:

int server_socket = 0;
fd_set set_of_sockets;
int client_sockets[30];
char* client_inputs[30] = { 0 };
int num_clients = 30;
int latest_conn = 0;
int new_socket = 0;
int active_socket = 0;
int incoming_conn = 0;
int read_msg = 0;
char read_buffer[1025];

};


#endif
