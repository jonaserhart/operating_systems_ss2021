#include "dbg.h"
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 20
#define DOORS 1
#define KNOBS 2

// tells compiler to not run any optimization on these variables
volatile int doors_produced;
volatile int knobs_ready;

pthread_mutex_t mutex;
// used to interrupt the process when all doors are produced
pthread_mutex_t quit;
pthread_cond_t cond;

typedef struct worker {
	pthread_t* thread;
	int assigned;
} worker_t;

int finished(pthread_mutex_t* quit) {
	switch (pthread_mutex_trylock(quit)) {
		case 0: pthread_mutex_unlock(quit); return 1;
		case EBUSY: return 0;
	}
	return 1;
}

void* produce_door(void* arg) {
	// get laziness value
	unsigned seed = (unsigned)pthread_self();
	srand(seed);
	double lazinenss = (double)rand() / RAND_MAX;

	while (!finished(&quit)) {
		// lock the mutex
		pthread_mutex_lock(&mutex);
		// check if there are enough knobs to make a door
		while (knobs_ready < 2)
			// wait for enough knobs to be available
			pthread_cond_wait(&cond, &mutex);
		// make door
		sleep((10 + lazinenss * 60) / 1000);
		// knobs are used to make a door
		knobs_ready -= 2;
		doors_produced++;
		// CS end
		pthread_mutex_unlock(&mutex);
	}

	// exit
	pthread_exit(NULL);
}

void* produce_knob(void* arg) {
	debug("Got quit mutex argument");
	// get laziness value
	unsigned seed = (unsigned)pthread_self();
	srand(seed);
	double lazinenss = (double)rand() / RAND_MAX;

	debug("Got laziness %ld", lazinenss);

	while (!finished(&quit)) {
		// lock the critical section
		pthread_mutex_lock(&mutex);
		debug("Producing knobs with laziness %ld", lazinenss);
		// make a knob
		knobs_ready++;
		// if there are two knobs ready, signal to the door workers
		if (knobs_ready >= 2) {
			pthread_cond_signal(&cond);
		}
		// end of critical section
		pthread_mutex_unlock(&mutex);
		sleep((10 + lazinenss * 30) / 1000);
		debug("Sleeping with laziness %ld", lazinenss);
	}

	// exit
	pthread_exit(NULL);
}

// this code is from my solution of ex11 task2 in 'number_fun'
int is_number(char number[]) {
	int i = 0;
	if (number[0] == '-') i = 1;
	for (; number[i] != 0; i++) {
		if (!isdigit(number[i])) return 0;
	}
	return 1;
}

int main(int argc, char* argv[]) {
	check(argc == 3, "usage: ./factory <enable load balancing> <number of doors to produce>");
	check(pthread_mutex_init(&mutex, NULL) == 0, "Error initializing mutex");
	check(pthread_mutex_init(&quit, NULL) == 0, "Error initializing mutex");
	check(pthread_cond_init(&cond, NULL) == 0, "Error initializing pthread_condition");

	debug("Initialized mutexes");

	pthread_mutex_lock(&quit);

	check(is_number(argv[1]), "Argument '%s' is not a number", argv[1]);
	check(is_number(argv[2]), "Argument '%s' is not a number", argv[2]);

	int no_doors = atol(argv[2]);
	debug("Parsed argument no_doors as : %d", no_doors);

	worker_t* workers[MAX_THREADS];

	// initialize workers
	for (int i = 0; i <= 10; i++) {
		workers[i] = malloc(sizeof(worker_t));
		debug("Malloced worker %d", i);
		workers[i]->assigned = KNOBS;
		debug("Assigned worker %d to KNOBS", i);
		pthread_create((workers[i]->thread), NULL, produce_knob, NULL);
	}
	for (int i = 11; i <= MAX_THREADS; i++) {
		workers[i] = malloc(sizeof(worker_t));
		debug("Malloced worker %d", i);
		workers[i]->assigned = DOORS;
		pthread_create((workers[i]->thread), NULL, produce_door, NULL);
	}

	// production loop
	int elapsed = 0;
	for (;;) {
		sleep(1);
		elapsed++;
		pthread_mutex_lock(&mutex);
		// print result
		log_success("Doors produced: %d in %d seconds", doors_produced, elapsed);
		if (doors_produced >= no_doors) {
			// signal 'quit'
			pthread_mutex_unlock(&quit);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}

	int errors = 0;

	for (int i = 0; i <= MAX_THREADS; i++) {
		if (pthread_join(*(workers[i]->thread), NULL) != 0) {
			errors = 1;
		}
	}

	check(errors == 0, "There were errors joining phtreads");

	for (int i = 0; i <= MAX_THREADS; i++) {
		if (workers[i]) free(workers[i]);
	}

	return 0;
error:
	for (int i = 0; i <= MAX_THREADS; i++) {
		if (workers[i]) free(workers[i]);
	}
	return 42;
}