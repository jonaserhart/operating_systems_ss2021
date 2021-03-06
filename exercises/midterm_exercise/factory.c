#include "dbg.h"
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 20
#define DOORS 1
#define KNOBS 2

// tells compiler to not run any optimization on these variables
volatile int doors_to_produce;
volatile int knobs_to_produce;
volatile int knobs_ready;
volatile int assignments[MAX_THREADS];

pthread_mutex_t mutex;
pthread_cond_t cond;

void* produce(void* arg) {
	int thread_index = *((int*)arg);
	unsigned seed = (unsigned)pthread_self();
	srand(seed);
	double laziness = (double)rand() / (double)RAND_MAX;
	debug("Got index arg %d", thread_index);
	debug("Got laziness %.2f", laziness);
	for (;;) {
		// lock the mutex
		pthread_mutex_lock(&mutex);

		if (doors_to_produce <= 0) {
			break;
		}

		if (assignments[thread_index] == DOORS) {
			// check if there are enough knobs to make a door
			struct timespec* timeout = malloc(sizeof(struct timespec));
			timeout->tv_sec = 1;
			if (knobs_ready < 2)
				// wait for enough knobs to be available
				pthread_cond_timedwait(&cond, &mutex, timeout);
			// make door
			free(timeout);
			sleep(((10 + laziness) * 90) / 1000);
			// knobs are used to make a door
			knobs_ready -= 2;
			doors_to_produce--;
		} else {
			if (knobs_to_produce <= 0) {
				break;
			}
			sleep(((10 + laziness) * 30) / 1000);
			// make a knob
			knobs_ready++;
			knobs_to_produce--;
			// if there are two knobs ready, signal to the door workers
			if (knobs_ready >= 2) {
				pthread_cond_signal(&cond);
			}
		}
		// CS end
		pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_unlock(&mutex);

	// exit
	pthread_exit(NULL);
}

// this code is from my solution of ex11 task2 in 'number_fun'
int is_number(char number[]) {
	int i = 0;
	if (number[0] == 0) return 0;
	if (number[0] == '-') i = 1;
	for (; number[i] != 0; i++) {
		if (!isdigit(number[i])) return 0;
	}
	return 1;
}

int main(int argc, char* argv[]) {
	clock_t start, end;
	check(argc == 3, "usage: ./factory <enable load balancing> <number of doors to produce>");
	check(pthread_mutex_init(&mutex, NULL) == 0, "Error initializing mutex");
	check(pthread_cond_init(&cond, NULL) == 0, "Error initializing pthread_condition");

	debug("Initialized mutexes");

	check(is_number(argv[1]), "Argument '%s' is not a number", argv[1]);
	check(is_number(argv[2]), "Argument '%s' is not a number", argv[2]);

	int reassign = atol(argv[1]);
	int no_doors = atol(argv[2]);
	int no_knobs = no_doors * 2;

	// just to show reassignment
	int no_knob_workers = 10;
	int no_door_workers = MAX_THREADS - no_knob_workers;

	// assign workers and doors_to_produce
	pthread_mutex_lock(&mutex);
	doors_to_produce = no_doors;
	knobs_to_produce = no_knobs;
	for (int i = 0; i < MAX_THREADS; i++) {
		assignments[i] = i < no_knob_workers ? KNOBS : DOORS;
		debug("assigned worker %d", i);
	}
	pthread_mutex_unlock(&mutex);
	debug("Parsed argument no_doors as : %d", no_doors);

	pthread_t workers[MAX_THREADS];

	// initialize workers
	for (int i = 0; i < 10; i++) {
		check(pthread_create(&workers[i], NULL, &produce, &i) == 0, "Error creating pthread");

		debug("Created worker %d", i);
	}
	for (int i = 10; i < MAX_THREADS; i++) {
		check(pthread_create(&workers[i], NULL, &produce, &i) == 0, "Error creating pthread");

		debug("Created worker %d", i);
	}

	// production loop
	int elapsed = 0;
	start = clock();
	for (;;) {
		sleep(1);
		// assign workers
		elapsed++;
		pthread_mutex_lock(&mutex);
		// begin critical section
		if (doors_to_produce <= 0) {
			log_success("Finished!");
			break;
		}
		int doors_produced = no_doors - doors_to_produce;
		int knobs_produced = no_knobs - knobs_to_produce;
		debug("Doors still to produce: %d", doors_to_produce);
		debug("Knobs still to produce: %d", knobs_to_produce);
		log_info("Producing doors: %.2f /s, knobs: %.2f / s",
		         doors_to_produce == 0 ? 0.0 : ((double)(doors_produced) / (double)elapsed),
		         knobs_to_produce == 0 ? 0.0 : ((double)(knobs_produced) / (double)elapsed));

		// end critical section
		pthread_mutex_unlock(&mutex);
		int reassigned = 0;
		if (reassign) {
			if (((float)knobs_produced / (float)doors_produced >= 1.1)) {
				if (no_knob_workers <= 1) {
					goto cont;
				}
				for (int i = 0; i < MAX_THREADS; i++) {
					if (assignments[i] == KNOBS) {
						assignments[i] = DOORS;
						no_door_workers++;
						no_knob_workers--;
						debug("\t reassigned worker %d to make doors", i);
						reassigned = 1;
						break;
					}
				}
			}
			if (((float)doors_produced / (float)knobs_produced) >= 1.1) {
				if (no_door_workers <= 1) {
					goto cont;
				}
				// reversed loop, because door workers are in the back
				for (int i = (MAX_THREADS - 1); i >= 0; i--) {
					if (assignments[i] == DOORS) {
						assignments[i] = KNOBS;
						no_knob_workers++;
						no_door_workers--;
						debug("\t reassigned worker %d to make knobs", i);
						reassigned = 1;
						break;
					}
				}
			}
			// assign all workers to produce doors if all knobs are produced
			if (knobs_to_produce == 0) {
				// reassign all knob producers to produce doors
				for (int i = 0; i < MAX_THREADS; i++) {
					if (assignments[i] == KNOBS) {
						assignments[i] = DOORS;
						no_door_workers++;
						no_knob_workers--;
						reassigned = 1;
						debug("\t reassigned worker %d to make doors", i);
					}
				}
			}
		}
	cont:
		if (reassigned) {
			log_info("\t Reassigned workers: %d making doors, %d making knobs", no_door_workers,
			         no_knob_workers);
		}
	}
	end = clock();
	log_success("Took: %.2f seconds", (((float)(end - start)) / (2 * CLOCKS_PER_SEC)));

	for (int i = 0; i < MAX_THREADS; i++) {
		if (pthread_join(workers[i], NULL) == 0) {
			debug("Joined pthread no %d", i);
		} else {
			log_err("Error joining pthread no %d", i);
		}
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
error:
	if (&mutex != NULL) {
		pthread_mutex_destroy(&mutex);
	}
	if (&cond != NULL) {
		pthread_cond_destroy(&cond);
	}
	return 1;
}