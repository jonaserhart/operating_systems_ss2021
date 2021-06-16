#define NDEBUG

#include "dbg.h"
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		log_err("USAGE: ./task1_server <List of clients>\n");
		return -1;
	}
	int number_of_pipes = argc - 1;
	// polling file descriptors
	struct pollfd pollfds[number_of_pipes];
	// save files for later i this variable
	char* files[number_of_pipes];
	// intialize with NULL
	for (int i = 0; i < number_of_pipes; i++) {
		files[i] = NULL;
	}

	printf("Server is up\n");

	// make fifos for each client
	for (int i = 0; i < number_of_pipes; i++) {
		// calculate size of the filename with path
		char name[9 + (sizeof(argv[i + 1]))];
		// set string to all zeroes -> no undefined behaviour
		memset(&name, '\0', sizeof(name));
		// concat path
		strcat(name, "/tmp/fifo_");
		strcat(name, argv[i + 1]);

		// try to make fifo with rw-rw-rw
		check(mkfifo(name, 0666) >= 0, "Error creating fifo");
		files[i] = malloc(sizeof(name));
		// see that the name is actually copied not just pointed to
		strcpy(files[i], name);
		// debug the file that was made
		debug("made file %s", files[i]);
		// try to open the fifo
		int fifo = open(name, O_RDONLY);
		// if failed, return an error
		check(fifo >= 0, "Error opening fifo");
		// fifo was established
		printf("client '%s' connected\n", argv[i + 1]);
		pollfds[i].fd = fifo;
		// POLLIN: there is data to be read
		pollfds[i].events = POLLIN;
	}
	printf("everyone has connected\n");
	printf("----------------------------\n\n");
	char buffer[BUFSIZ];
	memset(buffer, '\0', sizeof(buffer));
	int num_pipes_left = number_of_pipes;
	while (num_pipes_left > 0) {
		check(poll(pollfds, number_of_pipes, -1) >= 0, "Polling error");
		for (int i = 0; i < number_of_pipes; i++) {
			// find the pipe with the data
			if (pollfds[i].revents & POLLIN) {
				// read the written pipe
				if (read(pollfds[i].fd, buffer, sizeof(buffer)) > 0) {
					if (strcmp(buffer, "\n") != 0) {
						printf("%s: %s \n", argv[i + 1], buffer);
						memset(buffer, '\0', sizeof(buffer));
					} else {
						printf("client %s disconnected\n", argv[i + 1]);
						num_pipes_left--;
					}
				}
			}
		}
	}
	// cleanup
	for (int i = 0; i < number_of_pipes; i++) {
		debug("unlinking fifo %s", files[i]);
		if (unlink(files[i]) < 0) {
			log_err("error unlinking fifo '%s', please unlink manually", files[i]);
		}
		free(files[i]);
	}

	return 0;
// cleanup on error
error:
	for (int i = 0; i < number_of_pipes; i++) {
		if (files[i] != NULL) {
			debug("unlinking fifo %s", files[i]);
			if (unlink(files[i]) < 0) {
				log_err("error unlinking fifo '%s', please unlink manually", files[i]);
			}
			free(files[i]);
		}
	}
	return -1;
}