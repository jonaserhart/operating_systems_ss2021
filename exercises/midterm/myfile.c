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
	debug("Got index arg %d", thread_index);
	for (;;) {
		// lock the mutex
		pthread_mutex_lock(&mutex);

		if (doors_to_produce <= 0) {
			break;
		}

		if (assignments[thread_index] == DOORS) {
			// check if there are enough knobs to make a door
			while (knobs_ready < 2 && doors_to_produce > 0)
				// wait for enough knobs to be available
				pthread_cond_wait(&cond, &mutex);
			// make door
			sleep((10 + 0.4 * 60) / 1000);
			// knobs are used to make a door
			knobs_ready -= 2;
			doors_to_produce--;
		} else {
			if (knobs_to_produce <= 0 || doors_to_produce <= 0) {
				break;
			}
			sleep((10 + 0.4 * 30) / 1000);
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
	if (number[0] == '-') i = 1;
	for (; number[i] != 0; i++) {
		if (!isdigit(number[i])) return 0;
	}
	return 1;
}

int main(int argc, char* argv[]) {
	check(argc == 3, "usage: ./factory <enable load balancing> <number of doors to produce>");
	check(pthread_mutex_init(&mutex, NULL) == 0, "Error initializing mutex");
	check(pthread_cond_init(&cond, NULL) == 0, "Error initializing pthread_condition");

	debug("Initialized mutexes");

	check(is_number(argv[1]), "Argument '%s' is not a number", argv[1]);
	check(is_number(argv[2]), "Argument '%s' is not a number", argv[2]);

	int no_doors = atol(argv[2]);
	int no_knobs = no_doors * 2;

	// assign workers and doors_to_produce
	pthread_mutex_lock(&mutex);
	doors_to_produce = no_doors;
	knobs_to_produce = no_knobs;
	for (int i = 0; i < MAX_THREADS; i++) {
		assignments[i] = i < 2 ? KNOBS : DOORS;
		debug("assigned worker %d", i);
	}
	pthread_mutex_unlock(&mutex);
	debug("Parsed argument no_doors as : %d", no_doors);

	pthread_t workers[MAX_THREADS];

	// initialize workers
	for (int i = 0; i < 10; i++) {
		int* arg = malloc(sizeof(*arg));
		arg = &i;
		check(pthread_create(&workers[i], NULL, &produce, (void*)arg) == 0,
		      "Error creating pthread");

		debug("Created worker %d", i);
	}
	for (int i = 10; i < MAX_THREADS; i++) {
		pthread_create(&workers[i], NULL, &produce, &i);

		debug("Created worker %d", i);
	}

	// production loop
	int elapsed = 0;
	for (;;) {
		if (doors_to_produce <= 0) {
			log_info("Finished!");
			break;
		}
		// assign workers
		elapsed++;
		pthread_mutex_lock(&mutex);
		int doors_produced = no_doors - doors_to_produce;
		int knobs_produced = no_knobs - knobs_to_produce;
		debug("Doors still to produce: %d", doors_to_produce);
		debug("Knobs still to produce: %d", knobs_to_produce);
		log_info("Producing doors: %.2f /s, knobs: %.2f / s",
		         ((double)(doors_produced) / (double)elapsed),
		         ((double)(knobs_produced) / (double)elapsed));
		if (knobs_produced / doors_produced >= 1.1 || doors_produced == 0) {
			for (int i = 0; i < MAX_THREADS; i++) {
				if (assignments[i] == KNOBS) {
					assignments[i] = DOORS;
					log_info("\t reassigned worker %d to make doors", i);
					break;
				}
			}
		}
		if (doors_produced / knobs_produced >= 1.1) {
			for (int i = 0; i < MAX_THREADS; i++) {
				if (assignments[i] == DOORS) {
					assignments[i] = KNOBS;
					log_info("\t reassigned worker %d to make knobs", i);
					break;
				}
			}
		}
		pthread_mutex_unlock(&mutex);

		sleep(1);
	}

	for (int i = 0; i < MAX_THREADS; i++) {
		if (pthread_join(workers[i], NULL) == 0) {
			debug("Joined pthread no %d", i);
		} else {
			log_err("Error joining pthread no %d", i);
		}
	}

	return 0;
error:
	return 1;
}