#include "TCPClient.h"


/**********************************************************************************************
 * TCPClient (constructor) - Creates a Stdin file descriptor to simplify handling of user input. 
 *
 **********************************************************************************************/

TCPClient::TCPClient() {
}

/**********************************************************************************************
 * TCPClient (destructor) - No cleanup right now
 *
 **********************************************************************************************/

TCPClient::~TCPClient() {

}

/**********************************************************************************************
 * connectTo - Opens a File Descriptor socket to the IP address and port given in the
 *             parameters using a TCP connection.
 *
 *    Throws: socket_error exception if failed. socket_error is a child class of runtime_error
 **********************************************************************************************/

void TCPClient::connectTo(const char *ip_addr, unsigned short port) {
	{

		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Socket creation error");
			exit(EXIT_FAILURE);
		}

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port);

		// Convert IPv4 and IPv6 addresses from text to binary form 
		if (inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr) <= 0)
		{
			perror("Invalid address/ Address not supported");
			exit(EXIT_FAILURE);
		}

		if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) < 0)
		{
			perror("Connection Failed");
			exit(EXIT_FAILURE);
		}
	}

}

/**********************************************************************************************
 * handleConnection - Performs a loop that checks if the connection is still open, then 
 *                    looks for user input and sends it if available. Finally, looks for data
 *                    on the socket and sends it.
 * 
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPClient::handleConnection() {
	char input[stdin_bufsize];
	char output[socket_bufsize];
	fd_set read_set;
	struct timeval timeout;
	int readData;

	timeout.tv_sec = 10; // Time out after a minute
	timeout.tv_usec = 0;

	while (true) {
		char* rawInput = 0;
		std::cout << "Input message to send to server:\n";
		fgets(input, stdin_bufsize, stdin);
		send(sock, input, strlen(input), 0);
		FD_ZERO(&read_set);
		FD_SET(sock, &read_set);

		int readData = select(sock + 1, &read_set, NULL, NULL, &timeout);

		if (readData < 0) {
			perror("Error reading from server");
			exit(EXIT_FAILURE);
		}

		if (readData == 0) {
			std::cout << "Timeout waiting for server";
			perror("Disconnected from server");
			exit(EXIT_FAILURE);
		}

		if (readData > 0) {
			valread = read(sock, output, socket_bufsize);
			printf("%s\n", output);
		}
		if (strcmp("exit\n", input) == 0) { break; }
		//reset things between loops
		std::cout << std::flush;
		memset(output, 0, sizeof output);
		memset(input, 0, sizeof input);
	}






   
}

/**********************************************************************************************
 * closeConnection - Your comments here
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPClient::closeConn() {
}


