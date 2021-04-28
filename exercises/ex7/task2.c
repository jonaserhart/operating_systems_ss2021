#include "dbg.h"
#include "myqueue.h"
#include <pthread.h>

#define NUMTHREADS 5
// hack for 'converting' an integer into a void pointer
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

pthread_mutex_t mutex;
pthread_cond_t cond;
myqueue* queue;

// consumes values from the queue
static void* consume() {
	int sum = 0;
	int val = -1;
	while(val != 0) {
		// lock the mutex so other threads cant access the queue
		pthread_mutex_lock(&mutex);
		while(myqueue_is_empty(queue)) {
			// if empty wait for another value to be inserted into the queue
			pthread_cond_wait(&cond, &mutex);
		}
		// increment sum
		val = myqueue_pop(queue);
		pthread_mutex_unlock(&mutex);
		sum += val;
	}
	// return sum as void pointer on '0' received
	pthread_exit(INT2VOIDP(sum));
}

// produces values into the queue
static void produce() {
	// produce the '1' values
	for(int i = 0; i < 9999; i++) {
		pthread_mutex_lock(&mutex);
		myqueue_push(queue, 1);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	// produce the stop values
	for(int i = 0; i < 5; i++) {
		pthread_mutex_lock(&mutex);
		myqueue_push(queue, 0);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
}

int main() {
	// initialize the queue
	queue = malloc(sizeof(myqueue));
	myqueue_init(queue);
	myqueue_push(queue, 1);
	pthread_t threads[NUMTHREADS];
	// initializations with error checks
	check(pthread_mutex_init(&mutex, NULL) == 0, "error initializing mutex");
	check(pthread_cond_init(&cond, NULL) == 0, "error initializing cond");

	// create all threads
	for(size_t i = 0; i < NUMTHREADS; i++) {
		pthread_create(&threads[i], NULL, &consume, NULL);
	}

	// produce the values into the queue
	produce();

	int sum = 0;

	// wait for all threads and print the result
	for(size_t i = 0; i < NUMTHREADS; i++) {
		void* ret = NULL;
		pthread_join(threads[i], &ret);
		if((size_t)ret < 0) {
			// hack for converting a void pointer into an integer
			log_err("consumer %zu exited with code %zu", i, (size_t)ret);
			continue;
		}
		sum += (size_t)ret;
		log_info("consumer %zu sum: %zu", i, (size_t)ret);
	}

	// print the final result
	log_success("Final sum: %d", sum);

	// clean up
	if(pthread_mutex_destroy(&mutex) != 0) {
		log_err("error cleaning up phtread_mutex");
	}
	if(pthread_cond_destroy(&cond) != 0) {
		log_err("error cleaning up phtread_cond");
	}

	return 0;
error:
	return -1;
}
