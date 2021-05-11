#include "dbg.h"
#include "my_mutex.h"
#include "myqueue.h"
#include <pthread.h>
#include <sched.h>
#include <time.h>

#define NUMTHREADS 50
#define SUM 100000
// hack for 'converting' an integer into a void pointer
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

volatile my_mutex my_mut = ATOMIC_FLAG_INIT;
pthread_mutex_t pt_mut;
myqueue* queue;

#define CONSUME_INTO_SUM(lock, unlock, mutex, sum) \
	int val = -1; \
	while (val != 0) { \
		lock(&mutex); \
		if (!myqueue_is_empty(queue)) { \
			val = myqueue_pop(queue); \
			sum += val; \
		} \
		unlock(&mutex); \
		sched_yield(); \
	}

#define PRODUCE(lock, unlock, mutex) \
	for (int i = 1; i < SUM; i++) { \
		lock(&mutex); \
		myqueue_push(queue, 1); \
		unlock(&mutex); \
	} \
	for (int i = 0; i < NUMTHREADS; i++) { \
		lock(&mutex); \
		myqueue_push(queue, 0); \
		unlock(&mutex); \
	}

static void* consume_my() {
	int sum = 0;
	CONSUME_INTO_SUM(my_mutex_lock, my_mutex_unlock, my_mut, sum);
	return INT2VOIDP(sum);
}

static void* consume_phread() {
	int sum = 0;
	CONSUME_INTO_SUM(pthread_mutex_lock, pthread_mutex_unlock, pt_mut, sum);
	pthread_exit(INT2VOIDP(sum));
}

static void produce_my() {
	PRODUCE(my_mutex_lock, my_mutex_unlock, my_mut);
}

static void produce_pthread() {
	PRODUCE(pthread_mutex_lock, pthread_mutex_unlock, pt_mut);
}

int main(int argc, char* argv[]) {
	clock_t start, end;
	int sum = 0, print_consumers = 0;
	queue = malloc(sizeof(myqueue));
	myqueue_init(queue);
	myqueue_push(queue, 1);
	pthread_t threads[NUMTHREADS];
	check(pthread_mutex_init(&pt_mut, NULL) == 0, "error initializing mutex");
	if (argc == 2) {
		check(
		    argv[1][0] == '-' && argv[1][1] == 'c',
		    "Wrong argument: usage: task2 -c (optional parameter for printing consumer messages)");
		print_consumers = 1;
	}
	// my_mutex
	start = clock();
	log_info("Starting threads with my_mutex");
	for (size_t i = 0; i < NUMTHREADS; i++) {
		pthread_create(&threads[i], NULL, &consume_my, NULL);
	}
	produce_my();
	for (size_t i = 0; i < NUMTHREADS; i++) {
		void* ret = NULL;
		pthread_join(threads[i], &ret);
		if ((size_t)ret < 0) {
			if (print_consumers) {
				log_err("consumer %zu exited with code %zu", i, (size_t)ret);
			}
			continue;
		}
		sum += (size_t)ret;
		if (print_consumers) {
			log_info("consumer %zu sum: %zu", i, (size_t)ret);
		}
	}
	end = clock();
	log_success("Final sum: %d, computed in %.2f seconds", sum,
	            ((double)end - start) / CLOCKS_PER_SEC);

	// pthread_mutex
	log_info("Starting threads with pthread_mutex");
	start = clock();
	for (size_t i = 0; i < NUMTHREADS; i++) {
		pthread_create(&threads[i], NULL, &consume_phread, NULL);
	}
	produce_pthread();
	sum = 0;
	for (size_t i = 0; i < NUMTHREADS; i++) {
		void* ret = NULL;
		pthread_join(threads[i], &ret);
		if ((size_t)ret < 0) {
			if (print_consumers) {
				log_err("consumer %zu exited with code %zu", i, (size_t)ret);
			}
			continue;
		}
		sum += (size_t)ret;
		if (print_consumers) {
			log_info("consumer %zu sum: %zu", i, (size_t)ret);
		}
	}
	end = clock();
	log_success("Final sum: %d, computed in %.2f seconds", sum,
	            ((double)end - start) / CLOCKS_PER_SEC);

	// clean up
	if (pthread_mutex_destroy(&pt_mut) != 0) {
		log_err("error cleaning up phtread_mutex");
	}
	if (queue) {
		free(queue);
	}

	return 0;
error:
	if (queue) {
		free(queue);
	}
	return -1;
}
