#include "dbg.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	char message[1000];
	char buffer[1024];
	int client;
	char* name;
	struct sockaddr_in server_addr;
	socklen_t addr_size;
	name = argv[1];
	client = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;

	server_addr.sin_port = htons(12345);

	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	// Connect the socket to the server using the address
	addr_size = sizeof server_addr;
	connect(client, (struct sockaddr*)&server_addr, addr_size);

	for (;;) {
		if (fgets(message, 60, stdin) != NULL) {
			printf(message);
			if (send(client, message, strlen(message), 0) < 0) {
				printf("Send failed\n");
			}
		}
	}

	close(client);
	return 0;
}