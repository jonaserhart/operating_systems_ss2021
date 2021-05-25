#ifndef allocator2_h
#define allocator2_h

#include <stdlib.h>

#define BLOCK_SIZE 1024

void* my_malloc2(size_t size);
void my_free2(void* ptr);
void my_allocator_init2(size_t size);
void my_allocator_destroy2();

#endif