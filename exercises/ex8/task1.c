#include "dbg.h"
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define START 100000
#define NOTHREADS 10000
#define NOITER 1000

_Atomic int atomicint = START;
int normalint = START;

pthread_mutex_t pthread_mutex;

static void* mutex_decrease() {
	for (int i = 0; i < NOITER; i++) {
		pthread_mutex_lock(&pthread_mutex);
		normalint--;
		pthread_mutex_unlock(&pthread_mutex);
	}
	return NULL;
}

static void* atomic_decrease() {
	for (int i = 0; i < NOITER; i++) {
		atomicint--;
	}
	return NULL;
}

int main() {
	pthread_t threads[NOTHREADS];
	clock_t start, end;
	pthread_mutex_init(&pthread_mutex, NULL);

	log_info("Starting interations with mutext lock");
	start = clock();
	for (int i = 0; i < NOTHREADS; i++) {
		usleep(1);
		check(pthread_create(&threads[i], NULL, &mutex_decrease, NULL) == 0,
		      "Error creating pthread");
	}
	for (int i = 0; i < NOTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	end = clock();
	log_success("Time elapsed: %f seconds", ((double)(end - start)) / CLOCKS_PER_SEC);

	log_info("Starting interations using atomics");
	start = clock();
	for (int i = 0; i < NOTHREADS; i++) {
		usleep(1);
		check(pthread_create(&threads[i], NULL, &atomic_decrease, NULL) == 0,
		      "Error creating pthread");
	}
	for (int i = 0; i < NOTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	end = clock();
	log_success("Time elapsed: %.2f seconds", ((double)(end - start)) / CLOCKS_PER_SEC);

	// clean up
	check(pthread_mutex_destroy(&pthread_mutex) == 0, "Error cleaning up pthread_mutex");
	return 0;

error:
	return -1;
}