#include "allocator1.h"
#include "allocator2.h"
#include "membench.h"

int main() {
	run_membench_global(my_allocator_init, my_allocator_destroy, my_malloc, my_free);
	run_membench_global(my_allocator_init2, my_allocator_destroy2, my_malloc2, my_free2);
}