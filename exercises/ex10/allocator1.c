#include "allocator1.h"
#include "dbg.h"
#include "list.h"
#include <pthread.h>

List* lst;
pthread_mutex_t lock;

typedef struct head {
	void* memptr;
	int free;
} head_t;

void* my_malloc(size_t size) {
	head_t* current_element;
	pthread_mutex_lock(&lock);
	void* result = NULL;
	LIST_FOREACH(lst, first, next, current) {
		current_element = current->value;
		if (current_element->free) {
			current_element->free = 0;
			result = current_element->memptr;
			break;
		}
	}
	pthread_mutex_unlock(&lock);
	return result;
}

void my_free(void* ptr) {
	unsigned long address = (unsigned long)&ptr;
	head_t* current_head;
	pthread_mutex_lock(&lock);
	LIST_FOREACH(lst, first, next, current) {
		current_head = current->value;
		unsigned long current_addr = (unsigned long)&(current_head->memptr);
		if (current_addr == address) {
			// if it is the same address
			size_t block_size = BLOCK_SIZE - sizeof(head_t);
			// reset the memory block
			memset(current_head->memptr, 0, block_size);
			// set free flag to true
			current_head->free = 1;
			break;
		}
	}
	pthread_mutex_unlock(&lock);
}

void my_allocator_init(size_t size) {
	int num_blocks = size / BLOCK_SIZE;
	int overflow = size % (num_blocks * BLOCK_SIZE);
	size_t final_size = 0;
	if (overflow) {
		// add one block if there is an overflow
		final_size = size - overflow + BLOCK_SIZE;
	} else {
		final_size = size;
	}
	// allocate
	void* mem = malloc(final_size);
	check_mem(mem);
	// initialize
	memset(mem, 0, final_size);
	lst = List_create();
	int rc = 0;
	size_t head_siz = sizeof(head_t);
	// split mempool into blocks
	for (int i = 0; i < num_blocks; i++) {
		head_t* head = malloc(sizeof(head));
		head->free = 1;
		// offset for memory (must not access head)
		head->memptr = mem + head_siz;
		rc = List_unshift(lst, head);
	}
	check(rc == 0, "Error adding memory block to the list");
error:
	return;
}

/**
leaks Report Version: 4.0
Process 11595: 262364 nodes malloced for 137232 KB
Process 11595: 0 leaks for 0 total leaked bytes.

BUT i don't know how to get rid of this error:
alloc_test(15586,0x1027d3d40) malloc: *** error for object 0x154800010: pointer being freed was not
allocated

if i don't free the mempool pointer i get a leak of 128M, but if i do, i get this error
*/
void my_allocator_destroy() {
	// void* mempool = ((head_t*)lst->first->value)->memptr;
	// if (mempool != NULL) free(mempool);
	List_clear_destroy(lst);
	pthread_mutex_destroy(&lock);
}