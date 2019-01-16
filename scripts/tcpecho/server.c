/*
 * TCP echo client.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <WS2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <sys/types.h>
#include <stdint.h>

#define BUFFER_SIZE 1024
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }


static void run(int port)
{
	int server_fd, client_fd, err;
	struct sockaddr_in server, client;
	char buf[BUFFER_SIZE];

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) on_error("Could not create socket\n");

	server.sin_family = AF_INET;
	server.sin_port = htons((uint16_t)port);
	server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	char opt_val = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

	err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
	if (err < 0) on_error("Could not bind socket\n");

	err = listen(server_fd, 128);
	if (err < 0) on_error("Could not listen on socket\n");

	printf("Server is listening on %d\n", port);

	while (1) {
		socklen_t client_len = sizeof(client);
		memset(buf, 0, sizeof(buf));
		client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);

		if (client_fd < 0) on_error("Could not establish new connection\n");

		while (1) {
			int read = recv(client_fd, buf, BUFFER_SIZE, 0);

			if (!read)
				break;

			if (read < 0)
				on_error("Client read failed\n");

			printf("Received: %s\n", buf);
			err = send(client_fd, buf, read, 0);

			if (err < 0)
				on_error("Client write failed\n");
		}
	}
}

int main (int argc, char *argv[])
{
	char *ptr;
	long port;

	if (argc < 2)
		on_error("Usage: %s [port]\n", argv[0]);

	port = strtol(argv[1], &ptr, 10);

	if(port == 0)
		on_error("Usage: %s [port]\n", argv[0]);

	run(port);
	return -EXIT_SUCCESS;
}
