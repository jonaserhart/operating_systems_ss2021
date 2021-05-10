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

volatile my_mutex mutex = ATOMIC_FLAG_INIT;
myqueue* queue;

// consumes values from the queue
static void* consume(void* args) {
	int sum = 0;
	int val = -1;
	while (val != 0) {
		my_mutex_lock(&mutex);
		if (!myqueue_is_empty(queue)) {
			val = myqueue_pop(queue);
			sum += val;
		}
		my_mutex_unlock(&mutex);
		// yield to anyone else at the same priority
		sched_yield();
	}
	pthread_exit(INT2VOIDP(sum));
}

// produces values into the queue
static void produce() {
	// produce the '1' values
	for (int i = 1; i < SUM; i++) {
		my_mutex_lock(&mutex);
		myqueue_push(queue, 1);
		my_mutex_unlock(&mutex);
	}
	// produce the stop values
	for (int i = 0; i < NUMTHREADS; i++) {
		my_mutex_lock(&mutex);
		myqueue_push(queue, 0);
		my_mutex_unlock(&mutex);
	}
}

int main() {
	clock_t start, end;
	queue = malloc(sizeof(myqueue));
	myqueue_init(queue);
	myqueue_push(queue, 1);
	pthread_t threads[NUMTHREADS];

	start = clock();
	for (size_t i = 0; i < NUMTHREADS; i++) {
		pthread_create(&threads[i], NULL, &consume, NULL);
	}
	produce();

	int sum = 0;
	for (size_t i = 0; i < NUMTHREADS; i++) {
		void* ret = NULL;
		pthread_join(threads[i], &ret);
		if ((size_t)ret < 0) {
			log_err("consumer %zu exited with code %zu", i, (size_t)ret);
			continue;
		}
		sum += (size_t)ret;
		log_info("consumer %zu sum: %zu", i, (size_t)ret);
	}
	end = clock();

	log_success("Final sum: %d, computed in %.2f seconds", sum,
	            ((double)end - start) / CLOCKS_PER_SEC);

	return 0;
}
