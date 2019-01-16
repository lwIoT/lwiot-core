//
// Created by michel on 5-9-18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifdef WIN32
#include <WS2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#endif


int main(int argc, char **argv) {
	const char* server_name;
	int server_port;
	int sock;
	struct sockaddr_in server_address;
	const char* data_to_send = "Hello, World";

	if(argc != 3) {
		fprintf(stderr, "Usage: %s [host] [port]\n", argv[0]);
		return -EXIT_FAILURE;
	}

	server_name = argv[1];
	server_port = atoi(argv[2]);

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, server_name, &server_address.sin_addr);
	server_address.sin_port = htons((uint16_t)server_port);

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Could not create socket\n");
		return -EXIT_FAILURE;
	}

	if (connect(sock, (struct sockaddr*)&server_address,
	            sizeof(server_address)) < 0) {
		printf("Could not connect to server\n");
		return -EXIT_FAILURE;
	}

	send(sock, data_to_send, strlen(data_to_send), 0);

	int n = 0;
	int len = 0, maxlen = 100;

	char *buffer = malloc(maxlen);
	char* pbuffer = buffer;

	while ((n = recv(sock, pbuffer, maxlen, 0)) > 0) {
		pbuffer += n;
		maxlen -= n;
		len += n;

		buffer[len] = '\0';
		printf("Received: '%s'\n", buffer);
	}

	free(buffer);
	closesocket(sock);
	return 0;
}
