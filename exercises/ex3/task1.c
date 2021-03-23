#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

double mc_pi(int64_t S) {
	int64_t in_count = 0;
	for(int64_t i = 0; i < S; ++i) {
		const double x = rand() / (double)RAND_MAX;
		const double y = rand() / (double)RAND_MAX;
		if(x * x + y * y <= 1.f) {
			in_count++;
		}
	}
	return 4 * in_count / (double)S;
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		log_warn("USAGE: task1 <Number of Processes> <Number of computations>");
		return -1;
	}
	// N
	int number_of_processes = atoi(argv[1]);
	// S
	int number_of_computations = atoi(argv[2]);
	int i;
	pid_t pids[number_of_processes];

	// checks if the arguments are provided correctly
	check(number_of_processes > 0,
	      "Could not parse 'number of processes' as an integer greater than 0, got: %d",
	      number_of_processes);
	check(number_of_computations > 0,
	      "Could not parse 'number of computations' as an integer greater than 0, got: %d",
	      number_of_computations);

	for(i = 0; i < number_of_processes; i++) {
		pids[i] = fork();
		check(pids[i] >= 0, "Fork error");
		if(pids[i] == 0) {
			// is a child process that was successfully created
			// seed random number
			srand(getpid());
			double result = mc_pi(number_of_computations);
			log_info("Child %d PID = %d. mc_pi(%d) = %f", i, getpid(), number_of_computations,
			         result);
			return 0;
		}
	}
	while(number_of_processes > 0) {
		wait(NULL);
		number_of_processes--;
	}
	log_info("Done");
	return 0;
error:
	return -1;
}
