#define NDEBUG
#include "dbg.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static volatile sig_atomic_t signal_received = 0;

void on_signal(int sig) {
	signal_received = sig;
}

int main(void) {
	// define signals to block
	sigset_t signals;
	sigaddset(&signals, SIGINT);
	// add signal handler
	signal(SIGINT, on_signal);
	for (;;) {
		pid_t pid = fork();
		check(pid >= 0, "Fork failed");
		if (pid == 0) {
			sigprocmask(SIG_BLOCK, &signals, NULL);
			// child
			log_info("Child %d started.", getpid());
			sleep(5);
			log_info("Child %d done.", getpid());
			return 0;
		} else {
			// parent
			wait(NULL);
			if (signal_received == 0) {
				log_info("Did not receive SIGINT. Continuing.");
			} else {
				log_info("Received SIGINT. Exiting.");
				exit(0);
			}
		}
	}
error:
	return -1;
}