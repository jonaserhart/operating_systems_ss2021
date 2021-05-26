#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>

static void* (*default_malloc)(size_t) = NULL;

void print_number(size_t number) {
	if (number > 9) {
		print_number(number / 10);
	}
	const char digit = '0' + number % 10;
	write(STDOUT_FILENO, &digit, 1);
}

void print_size(size_t size) {
	write(STDOUT_FILENO, "<allocating ", 1);
	print_number(size);
	write(STDOUT_FILENO, " bytes>", 1);
}

void* malloc(size_t size) {
	if (default_malloc == NULL) {
		default_malloc = dlsym(RTLD_NEXT, "malloc");
	}
	print_size(size);
	return default_malloc(size);
}