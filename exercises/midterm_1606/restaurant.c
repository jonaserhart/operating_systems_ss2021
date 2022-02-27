#include "dbg.h"
#include "myqueue.h"
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

myqueue* orders_queue;

// this is for checking if there are customers to serve left
volatile int orders_to_serve;
// this is the order on the table
volatile int order_on_table;
// use notifications
volatile int notify_customers;

// mutex for orders
pthread_mutex_t order_mutex;
// cond: guest has given an order
pthread_cond_t order_given;
// at least one order is ready
pthread_cond_t order_ready;
// the counter is free to place an order on
pthread_cond_t counter_free;

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

void* cook(void* arg) {
	int cook_index = *((int*)arg);
	for (;;) {
		// cook or wait for order
		pthread_mutex_lock(&order_mutex);
		// wait for order
		if (orders_to_serve <= 0) {
			pthread_mutex_unlock(&order_mutex);
			pthread_exit(NULL);
		}
		struct timespec* timeout = malloc(sizeof(struct timespec));
		timeout->tv_sec = 1;

		// if the queue is empty, wait until there are elements
		if (myqueue_is_empty(orders_queue)) {
			pthread_cond_timedwait(&order_given, &order_mutex, timeout);
			// if still empty, try agin 'continue'
			if (myqueue_is_empty(orders_queue)) {
				pthread_mutex_unlock(&order_mutex);
				continue;
			}
		}
		// free timeout
		free(timeout);
		// queue is not empty -> cook
		srand(time(0));
		int cook_time = rand() % 400 + 100;
		// cook a meal order
		// remove order from queue
		int order = myqueue_pop(orders_queue);
		log_info("Cook %d is prepairing meal order %d", cook_index, order);
		usleep(cook_time * 1000);
		log_info("Cook %d has finished cooking order %d", cook_index, order);
		// if there is an order on the table, we cannot put it there, so wait for someone to take it
		while (order_on_table != -1)
			pthread_cond_wait(&counter_free, &order_mutex);
		// place order on the table
		order_on_table = order;
		log_info("Cook %d has placed order %d on the counter", cook_index, order_on_table);
		// this part is for task2
		if (notify_customers) {
			pthread_cond_signal(&order_ready);
		}

		pthread_mutex_unlock(&order_mutex);
	}
	pthread_exit(NULL);
}

void* placeorder_and_wait(void* arg) {
	int guest_index = *((int*)arg);
	pthread_mutex_lock(&order_mutex);
	// place order
	myqueue_push(orders_queue, guest_index);
	pthread_cond_signal(&order_given);
	log_info("Guest %d has made order %d", guest_index, guest_index);
	pthread_mutex_unlock(&order_mutex);
	// measure time
	clock_t start, end;
	start = clock();
	if (notify_customers) {
		// if they will be notified, wait for notification
		while (1) {
			pthread_mutex_lock(&order_mutex);
			while (order_on_table == -1)
				pthread_cond_wait(&order_ready, &order_mutex);
			if (order_on_table != guest_index) {
				pthread_mutex_unlock(&order_mutex);
				continue;
			}
			// set that there is no order on the table
			order_on_table = -1;
			orders_to_serve--;
			// signal that the counter is free
			pthread_cond_signal(&counter_free);
			pthread_mutex_unlock(&order_mutex);
			break;
		}
	} else {
		// wait in loop
		while (1) {
			// check every 100 ms if there is an order on the table
			usleep(100 * 1000);
			pthread_mutex_lock(&order_mutex);
			debug("comparing %d and %d", order_on_table, guest_index);
			if (order_on_table == guest_index) {
				// set that there is no order on the table
				order_on_table = -1;
				orders_to_serve--;
				// signal that the counter is free
				pthread_cond_signal(&counter_free);
				pthread_mutex_unlock(&order_mutex);
				// leave with order
				break;
			}
			pthread_mutex_unlock(&order_mutex);
		}
	}

	end = clock();
	float waiting_time = 1000 * (((float)(end - start)) / (CLOCKS_PER_SEC));
	log_info("Guest %d has picked up order %d after %.2f ms", guest_index, guest_index,
	         waiting_time);

	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		log_err("usage: ./restaurant <enable notifications> <number of guests> <number of cooks>");
		return 1;
	}

	// sanity checks

	if (!is_number(argv[1])) {
		log_err("Argument '%s' is not a number", argv[1]);
		return 1;
	}
	if (!is_number(argv[2])) {
		log_err("Argument '%s' is not a number", argv[2]);
		return 1;
	}
	if (!is_number(argv[2])) {
		log_err("Argument '%s' is not a number", argv[2]);
		return 1;
	}

	debug("passed checks");

	// initialize queue, mutex and conds
	orders_queue = malloc(sizeof(myqueue));
	myqueue_init(orders_queue);
	debug("initialized queue");
	pthread_mutex_init(&order_mutex, NULL);
	pthread_cond_init(&order_ready, NULL);
	pthread_cond_init(&order_given, NULL);
	pthread_cond_init(&counter_free, NULL);

	debug("initialized mutex and conds");

	// assume all of the arguments are numbers because of the checks
	int notify = atol(argv[1]);
	int no_guests = atol(argv[2]);
	int no_cooks = atol(argv[2]);

	debug("parsed args");

	pthread_t cooks[no_cooks];
	pthread_t guests[no_guests];

	// check if queue initialization failed
	check_mem(orders_queue);

	// initialize counter
	pthread_mutex_lock(&order_mutex);
	notify_customers = notify;
	order_on_table = -1;
	orders_to_serve = no_guests;
	pthread_mutex_unlock(&order_mutex);

	for (int i = 0; i < no_cooks; i++) {
		pthread_create(&cooks[i], NULL, &cook, &i);
	}
	for (int i = 0; i < no_guests; i++) {
		pthread_create(&guests[i], NULL, &placeorder_and_wait, &i);
	}

	// cleanup
	for (int i = 0; i < no_guests; i++) {
		pthread_join(guests[i], NULL);
	}
	for (int i = 0; i < no_cooks; i++) {
		pthread_join(cooks[i], NULL);
	}
	pthread_cond_destroy(&order_ready);
	pthread_cond_destroy(&order_given);
	pthread_cond_destroy(&counter_free);

	pthread_mutex_destroy(&order_mutex);

	free(orders_queue);

	log_success("Finished");

	return 0;
error:
	if (&order_ready) {
		pthread_cond_destroy(&order_ready);
	}
	if (&order_given) {
		pthread_cond_destroy(&order_given);
	}
	if (&counter_free) {
		pthread_cond_destroy(&counter_free);
	}
	if (&order_mutex) {
		pthread_mutex_destroy(&order_mutex);
	}
	if (orders_queue) {
		free(orders_queue);
	}
	return 1;
}
