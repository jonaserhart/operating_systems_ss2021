#define NDEBUG

#include "dbg.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define MAX_THR 30

char client_message[2000];

int main(int argc, char* argv[]) {
	int socket_desc, client_size, client_socket;
	struct sockaddr_in addr, client_addr;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5556);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(socket_desc, (struct sockaddr*)&addr, sizeof(addr));

	listen(socket_desc, 5);
	debug("listening...");

	for (;;) {
		client_size = sizeof(client_addr);
		client_socket = accept()
	}

	return 0;
error:
	return -1;
}