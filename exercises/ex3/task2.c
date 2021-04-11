// don't print debug messages
#define NDEBUG
#include "dbg.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t running = 1;

void on_finished() {
	running = 0;
}

int main(int argc, char* argv[]) {
	if(argc != 2) {
		// prints usage
		log_warn("USAGE: task2 <seconds for child to sleep>");
		return -1;
	}
	// parse argument 'T'
	int seconds_to_sleep = atoi(argv[1]);
	check(seconds_to_sleep > 0,
	      "'seconds to sleep' could not be parsed as an integer greater than 0, got: %d",
	      seconds_to_sleep);

	// define the sigaction
	struct sigaction sigact = { 0 };
	// define the signal handler
	sigact.sa_handler = on_finished; 

	// create the child process
	pid_t pid = fork();
	// check if the fork was successful
	check(pid >= 0, "Fork error");
	if(pid == 0) {
		// is child
		debug("%ld", time(0));
		log_info("Child %d sleeping for %d seconds...", getpid(), seconds_to_sleep);
		sleep(seconds_to_sleep);
		debug("%ld", time(0));
		log_info("Child done");
		// exit (sends sigchld)
		return 0;
	} else {
		// is parent
		// register signal action with handler
		sigaction(SIGCHLD, &sigact, NULL);
		// loop while running is 1
		while(running);
		log_info("Parent done");
	}
	return 0;
error:
	return -1;
}