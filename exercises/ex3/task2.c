// don't print debug messages
#define NDEBUG
#include "dbg.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void on_finished() {
	// log parent done
	log_info("Parent done");
	exit(0);
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
	// get pid of parent to send signal to
	pid_t parent = getpid();

	// define the sigaction
	struct sigaction sigact;
	// fill the sigaction struct with zeroes (reduces undefined behavoir)
	memset(&sigact, 0, sizeof(sigact));
	// define the signal handler
	sigact.__sigaction_u.__sa_handler = on_finished;
	;

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
        
		// send signal to parent
		// SIGCHLD: child status has changed
		kill(parent, SIGCHLD);
		// exit
		return 0;
	} else {
		// is parent
		// register signal action with handler
		sigaction(SIGCHLD, &sigact, NULL);
		// loop endlessly (signal handler will quit the program)
		for(;;)
			;
	}
	return 0;
error:
	return -1;
}