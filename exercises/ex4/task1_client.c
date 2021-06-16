#include "dbg.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("USAGE: ./task_client <name>");
		return -1;
	}
	char name[9 + (sizeof(argv[1]))];
	memset(&name, '\0', sizeof(name));
	strcat(strcat(name, "/tmp/fifo_"), argv[1]);

	int fifo;
	check((fifo = open(name, O_WRONLY)) >= 0,
	      "Error opening fifo: your name may not be listed on the server");

	char value[BUFSIZ];
	while (1) {
		memset(value, '\0', BUFSIZ);
		printf("\n> ");
		fgets(value, BUFSIZ, stdin);
		if (strcmp(value, "\n") == 0) {
			printf("exiting...\n");
			break;
		}
		write(fifo, value, BUFSIZ);
	}
	check(close(fifo) == 0, "Error closing fifo");
	return 0;
error:
	return -1;
}