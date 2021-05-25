#include "allocator2.h"
#include "dbg.h"
#include "list.h"
#include <pthread.h>

List* free_list;
pthread_mutex_t lock;

typedef struct head {
	void* memptr;
	int free;
} head_t;

void* my_malloc2(size_t size) {
	head_t* new_head;
	pthread_mutex_lock(&lock);
	void* result = NULL;
	head_t* current_element;
	LIST_FOREACH(free_list, first, next, current) {
		current_element = current->value;
		// free element in the list found
		if (current_element->free) {
			// set free to false
			current_element->free = 0;
			// return the resulting mempointer
			result = current_element->memptr;
			// add a new head to the list
			// which starts at the next free memory
			// (old_memory_start + allocated size + sizeof(head))
			new_head = current_element->memptr + size;
			new_head->free = 1;
			new_head->memptr = new_head + sizeof(head_t);
			List_push(free_list, new_head);
			break;
		}
	}
	pthread_mutex_unlock(&lock);
	return result;
}
void my_free2(void* ptr) {
	unsigned long address = (unsigned long)&ptr;
	head_t* current_head;
	pthread_mutex_lock(&lock);
	LIST_FOREACH(free_list, first, next, current) {
		current_head = current->value;
		unsigned long current_addr = (unsigned long)&(current_head->memptr);
		if (current_addr == address) {
			// if it is the same address
			size_t block_size = BLOCK_SIZE - sizeof(head_t);
			// reset the memory block
			memset(current_head->memptr, 0, block_size);
			// set free flag to true
			current_head->free = 1;
			// merge with other free blocks

			// check the next block
			current_head = current->next->value;
			if (current_head && current_head->free) {
				current->next = current->next->next;
			}
			// check the previous block
			current_head = current->prev->value;
			if (current_head && current_head->free) {
				// set the next of the previous to the next of this node
				current->prev->next = current->next;
			}
			break;
		}
	}
	pthread_mutex_unlock(&lock);
}
void my_allocator_init2(size_t size) {
	void* mempool = malloc(size);
	check_mem(mempool);
	pthread_mutex_init(&lock, NULL);
	free_list = List_create();
	head_t* head = mempool;
	head->free = 1;
	// start of memory pool is after the head
	head->memptr = mempool + sizeof(head_t);
	check(List_unshift(free_list, mempool) == 0, "Error initializing free list");
error:
	return;
}
void my_allocator_destroy2() {
	// mempool starts at head of the first element in the list
	void* mempool = free_list->first->value;
	free(mempool);
	List_destroy(free_list);
	pthread_mutex_destroy(&lock);
}