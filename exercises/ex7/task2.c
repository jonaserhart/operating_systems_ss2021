#include "dbg.h"
#include "myqueue.h"
#include <pthread.h>

#define NUMTHREADS 5
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

pthread_mutex_t mutex;
pthread_cond_t cond;
myqueue* queue;

void* consumer() {
	int sum = 0;
	int val = -1;
	while(val != 0) {
		pthread_mutex_lock(&mutex);
		while(myqueue_is_empty(queue)) {
			pthread_cond_wait(&cond, &mutex);
		}
		val = myqueue_pop(queue);
		pthread_mutex_unlock(&mutex);
		sum += val;
	}
	pthread_exit(INT2VOIDP(sum));
}

void produce() {
	for(int i = 0; i < 9999; i++) {
		pthread_mutex_lock(&mutex);
		myqueue_push(queue, 1);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	for(int i = 0; i < 5; i++) {
		pthread_mutex_lock(&mutex);
		myqueue_push(queue, 0);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
}

int main() {
	queue = malloc(sizeof(myqueue));
	myqueue_init(queue);
	myqueue_push(queue, 1);
	pthread_t threads[NUMTHREADS];
	check(pthread_mutex_init(&mutex, NULL) == 0, "error initializing mutex");
	check(pthread_cond_init(&cond, NULL) == 0, "error initializing cond");

	for(size_t i = 0; i < NUMTHREADS; i++) {
		pthread_create(&threads[i], NULL, &consumer, NULL);
	}

	produce();

	int sum = 0;

	for(size_t i = 0; i < NUMTHREADS; i++) {
		void* ret = NULL;
		pthread_join(threads[i], &ret);
		if((size_t)ret < 0) {
			log_err("consumer %zu exited with code %zu", i, (size_t)ret);
			continue;
		}
		sum += (size_t)ret;
		log_info("consumer %zu sum: %zu", i, (size_t)ret);
	}

	log_success("Final sum: %d", sum);

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
