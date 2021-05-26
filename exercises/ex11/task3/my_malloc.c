#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>

// USAGE: LD_PRELOAD=./my_malloc.so <executable (eg.: ls)>

static void* (*default_malloc)(size_t) = NULL;

void print_number(size_t number) {
	if (number > 9) {
		print_number(number / 10);
	}
	const char digit = '0' + number % 10;
	write(STDOUT_FILENO, &digit, 1);
}

void print_size(size_t size) {
	write(STDOUT_FILENO, "<allocating ", 12);
	print_number(size);
	write(STDOUT_FILENO, " bytes>\n", 8);
}

void* malloc(size_t size) {
	if (default_malloc == NULL) {
		// man: RTLD_NEXT: 	Find the next occurrence of the desired symbol in the
		// 					search order after the current object. (Will search for 'malloc' outside
		// 					this file)
		// this may show an error in an IDE, because _GNU_SOURCE needs to be defined, which i did in
		// my Makefile, so with using 'make', this will work
		default_malloc = dlsym(RTLD_NEXT, "malloc");
	}
	print_size(size);
	return default_malloc(size);
}