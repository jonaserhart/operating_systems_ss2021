#ifndef allocator1_h
#define allocator1_h

#include <stdlib.h>

#define BLOCK_SIZE 1024

void* my_malloc(size_t size);
void my_free(void* ptr);
void my_allocator_init(size_t size);
void my_allocator_destroy();

#endif