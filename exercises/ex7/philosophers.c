#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EAT_DURATION 500
#define NUM_PHILOSOPHER 5
#define EAT_ITERATIONS 1000
#define RIGHT_CHOPSTICK(n) (n)
#define LEFT_CHOPSTICK(n) (((n) + 1) % NUM_PHILOSOPHER)

#define LOCK_SUCCESS 0

pthread_t philosopher[NUM_PHILOSOPHER];
pthread_mutex_t chopstick[NUM_PHILOSOPHER];
// thread condition
pthread_cond_t done_eating;

void* dine(void* id) {
	int n = (int)(intptr_t)id;
	for(int i = 0; i < EAT_ITERATIONS; ++i) {
		// this is where a deadlock could have occured
		// 1. 1st philosopher takes the right chopstick
		// 2. 2nd philosopher takes the left chopstick
		// 3. they wait on each other for eternity

		// these next two lines fix the problem
		if(pthread_mutex_trylock(&chopstick[RIGHT_CHOPSTICK(n)]) != LOCK_SUCCESS) {
			// if lock of the first stick was not successful
			// wait for another philosopher to finish eating, then take the chopstick
			pthread_cond_wait(&done_eating, &chopstick[RIGHT_CHOPSTICK(n)]);
		}
		pthread_mutex_lock(&chopstick[LEFT_CHOPSTICK(n)]);
		usleep(EAT_DURATION);
		pthread_mutex_unlock(&chopstick[LEFT_CHOPSTICK(n)]);
		pthread_mutex_unlock(&chopstick[RIGHT_CHOPSTICK(n)]);
		// signal that the chopsticks are ready
		pthread_cond_signal(&done_eating);
	}
	printf("Philosopher %d is done eating!\n", n);

	return NULL;
}

int main() {
	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_mutex_init(&chopstick[i], NULL);
	}

	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_create(&philosopher[i], NULL, dine, (void*)(intptr_t)i);
	}

	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_join(philosopher[i], NULL);
	}

	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_mutex_destroy(&chopstick[i]);
	}

	return EXIT_SUCCESS;
}
