#define NDEBUG
#include "dbg.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	int socket_filedescriptor;
	char value[BUFSIZ];
	struct sockaddr_un server;
	server.sun_family = AF_UNIX;
	memset(server.sun_path, '\0', sizeof(server.sun_path));
	strcpy(server.sun_path, "/tmp/socket.path");

    // open socket
	socket_filedescriptor = socket(PF_LOCAL, SOCK_STREAM, 0);
	check(socket_filedescriptor >= 0, "Error opening socket");

    // check if connection is successful
	check(connect(socket_filedescriptor, (struct sockaddr*)&server, sizeof(server)) == 0,
	      "Could not connect");

	while(1) {
		bzero(value, BUFSIZ);
		printf("\n> ");
		fgets(value, BUFSIZ, stdin);
		// remove trailing '\n'
		value[strcspn(value, "\n")] = 0;
		write(socket_filedescriptor, value, BUFSIZ);
		if(strcmp(value, "") == 0) {
            // print exit
			printf("exiting...\n");
            // wait for server to clean up
			read(socket_filedescriptor, value, BUFSIZ);
            // break
			break;
		}
		ssize_t read_size = 0;
        // loop for reading input greater than BUFSIZ
		do {
			bzero(value, BUFSIZ);
			read_size = read(socket_filedescriptor, value, BUFSIZ);
			printf("%s", value);
		} while(read_size >= BUFSIZ);
		printf("\n");
	}
	return 0;
error:
	return -1;
}