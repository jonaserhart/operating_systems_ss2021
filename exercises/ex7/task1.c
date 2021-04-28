#include "dbg.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int my_global = 1;

void* increment_my_global(){
    my_global++;
    return 0;
}

int main() {
	pid_t pid;
    pthread_t thread_id;

	pid = fork();
	check(pid != -1, "fork failed");
	if(pid == 0) {
		// child
        // increment copied global variable (this is not the original)
        debug_multi("incrementing...");
        increment_my_global();
		return 0;
	} else {
		// parent
        // wait for child
		wait(NULL);
        // this version of my_global is still not incremented
        log_info("after child process increment: %d", my_global);
	}
    
    // create a thread and give it a job
    int rc = pthread_create(&thread_id, NULL, &increment_my_global, NULL);
    check(rc >= 0, "Pthread create error");
    // wait for thread to finish
    pthread_join(thread_id, NULL);
    // threads shares global variables -> this one will be incremented
    log_info("after thread increment: %d", my_global);
    pthread_exit(NULL);
error:
	return -1;
}