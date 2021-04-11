#define NDEBUG
#include "dbg.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define WRITE 1
#define READ 0

#define MAX_ARGS 8
void parse_args(char* str, char* (*result)[MAX_ARGS]) {
	memset(*result, 0, sizeof(char*) * MAX_ARGS);
	for(int i = 0; i < MAX_ARGS - 1; ++i, str = NULL) {
		(*result)[i] = strtok(str, " ");
	}
}

// ./task2 "ls -la" "grep Makefile"

int main(int argc, char * argv[])
{
    int pipes[2];
    pid_t pid;
    if (argc < 3) {
        return -1;
    }
    pipe(pipes);
    pid = fork();
    if (pid == 0) {
        // child
        char* args[MAX_ARGS];
        parse_args(argv[2], &args);
        close(pipes[WRITE]);
        dup2(pipes[READ], STDIN_FILENO);
        execvp(args[0], args);
        return 0;
    } else {
        // parent
        close(pipes[READ]);
        char* args[MAX_ARGS];
        dup2(pipes[WRITE], STDOUT_FILENO);
        parse_args(argv[1], &args);
        execvp(args[0], args);

        wait(NULL);
    }
    return 0;
}