#include "TCPServer.h"

TCPServer::TCPServer() {

}


TCPServer::~TCPServer() {

}

/**********************************************************************************************
 * bindSvr - Creates a network socket and sets it nonblocking so we can loop through looking for
 *           data. Then binds it to the ip address and port
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/
//int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout
//int inet_pton(int af, const char *src, void *dst)
//int ioctl(inet fd, unsigned long request, ...);
//ssize_t write(int fd, const void *buf, size_t count);
//ssize_t read(int fd, void *buf, size_t count);

//Heavily utilized tutorial at: https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/ for programming entire class

void TCPServer::bindSvr(const char *ip_addr, short unsigned int port) {
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip_addr, &(address.sin_addr));
	address.sin_port = htons(port);
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("failed to create socket");
		exit(EXIT_FAILURE);
	}
	if (bind(server_socket, (struct sockaddr*) & address, sizeof(address)) < 0) {
		perror("Failed to bind to port");
		exit(EXIT_FAILURE);
	}
	std::cout << "Server listening on port " << port << "\n";

}

/**********************************************************************************************
 * listenSvr - Performs a loop to look for connections and create TCPConn objects to handle
 *             them. Also loops through the list of connections and handles data received and
 *             sending of data. 
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::listenSvr() {

	struct sockaddr_in address;
	auto addrlen = sizeof(address);

	for (int i = 0; i < num_clients; i++) {
		client_sockets[i] = 0;
	}
	if (listen(server_socket, 3) < 0) {
		perror("Too many pending connections");
		exit(EXIT_FAILURE);
	}


	std::cout << "Waiting for client connections\n";
	while (true) {
		FD_ZERO(&set_of_sockets);
		FD_SET(server_socket, &set_of_sockets);
		latest_conn = server_socket;

		for (int i = 0; i < num_clients; i++) {
			new_socket = client_sockets[i];
			if (new_socket > 0) { FD_SET(new_socket, &set_of_sockets); }
			if (new_socket > 0) {
				latest_conn = new_socket;
			}
		}
		active_socket = select(latest_conn + 1, &set_of_sockets, NULL, NULL, NULL);

		if ((active_socket < 0) && (errno != EINTR)) {
			std::cout << "Error reading from sockets";
		}

		//Incoming connection attempt
		if (FD_ISSET(server_socket, &set_of_sockets)) {
			if ((incoming_conn = accept(server_socket, (struct sockaddr*) &address, (socklen_t*)&addrlen)) < 0) {
				perror("New connection error");
				exit(EXIT_FAILURE);
			}

			const char* message = "Welcome to the Server\n";

			std::cout << "User connected from IP: " << inet_ntoa(address.sin_addr) << " Port: " << ntohs(address.sin_port) << "\n";
			if (send(incoming_conn, message, strlen(message), 0) != strlen(message))
			{
				perror("Failure to send welcome message");
			}

			for (int i = 0; i < num_clients; i++)
			{
				if (client_sockets[i] == 0)
				{
					client_sockets[i] = incoming_conn;
					std::cout << "Adding new connection to connection list\n";
					break;
				}
			}
		}

		//Client socket sending data
		for (int i = 0; i < num_clients; i++)
		{
			latest_conn = client_sockets[i];

			if (FD_ISSET(latest_conn, &set_of_sockets))
			{
				//Client has disconnected
				if ((read_msg = read(latest_conn, read_buffer, 1024)) == 0)
				{
					getpeername(latest_conn, (struct sockaddr*) & address, (socklen_t*)&addrlen);
					std::cout << "Client disconnected from IP: " << inet_ntoa(address.sin_addr) << " Port: " << ntohs(address.sin_port) <<  "\n",
					//Close the file descriptor and mark the connection index for re-use
					close(latest_conn);
					client_sockets[i] = 0;
				}

				//We received incoming message from client
				else
				{
					read_buffer[read_msg] = '\0';
					std::cout << read_buffer;
				}
			}
		}
	}

}

/**********************************************************************************************
 * shutdown - Cleanly closes the socket FD.
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::shutdown() {
}
