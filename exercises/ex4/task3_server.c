#define NDEBUG

#include "dbg.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 8

void parse_args(char* str, char* (*result)[MAX_ARGS]) {
	memset(*result, 0, sizeof(char*) * MAX_ARGS);
	for(int i = 0; i < MAX_ARGS - 1; ++i, str = NULL) {
		(*result)[i] = strtok(str, " ");
	}
}

/**
 * Executes a shell command
 * @param argv: argument vector with argv[0] = command
 */
int execute(char** argv) {
	pid_t pid;
	int status;

	if((pid = fork()) < 0) {
		return -1;
	} else if(pid == 0) {
		if(execvp(argv[0], argv) != 0) {
			return -1;
		}
		return 0;
	} else {
		while(wait(&status) != pid)
			;
	}
	return status;
}

int main() {
	struct sockaddr_un server, client;
	char buffer[BUFSIZ];
    // open socket
	int socket_filedescriptor = socket(PF_LOCAL, SOCK_STREAM, 0);
	check(socket_filedescriptor >= 0, "Error opening socket");
	server.sun_family = AF_UNIX;
	memset(server.sun_path, '\0', sizeof(server.sun_path));
	strcpy(server.sun_path, "/tmp/socket.path");

	int bind_rc = bind(socket_filedescriptor, (struct sockaddr*)&server, (socklen_t)sizeof(server));
	if(bind_rc < 0) {
		// if last time was a dirty exit
		unlink("/tmp/socket.path");
		bind_rc = bind(socket_filedescriptor, (struct sockaddr*)&server, (socklen_t)sizeof(server));
	}
	// if we still can't bind, let the user know
	check(bind_rc >= 0, "Error binding socket");
	debug("Socket binded");
	// allow one connection only
	listen(socket_filedescriptor, 1);
	debug("Listening...");

	socklen_t client_lenght = sizeof(client);
	// accept client connection
	int newsocketfd = accept(socket_filedescriptor, (struct sockaddr*)&client, &client_lenght);
	check(newsocketfd >= 0, "Accept failed");
	// redirect outputs
	dup2(newsocketfd, STDOUT_FILENO);
	dup2(newsocketfd, STDERR_FILENO);
	for(;;) {
		bzero(buffer, BUFSIZ);
		if(read(newsocketfd, buffer, BUFSIZ) < 0) {
			// if invalid, continue
			continue;
		}
		if(strcmp("", buffer) == 0) {
			// if empty break
			break;
		}
		char* args[MAX_ARGS];
		parse_args(buffer, &args);
		if(execute(args) != 0) {
			write(newsocketfd, "command failed", 15);
		}
	}
	write(newsocketfd, "exit", 5);
	close(socket_filedescriptor);
	unlink("/tmp/socket.path");
	return 0;
error:
	return -1;
}