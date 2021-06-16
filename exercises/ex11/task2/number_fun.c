#include "dbg.h"
#include <ctype.h>
#include <dlfcn.h>
#include <stdlib.h>

#define EXEC_FUN_NAME "execute"

typedef int (*lib_function)(const int input);

int is_number(char number[]) {
	int i = 0;
	if (number[0] == '-') i = 1;
	for (; number[i] != 0; i++) {
		if (!isdigit(number[i])) return 0;
	}
	return 1;
}

int main(int argc, char* argv[]) {
	int number;
	check(argc > 2,
	      "Not enough arguments. USAGE: ./number_fun <number> <list of plugin libraries>");
	check(is_number(argv[1]), "Provided argument '%s' is not a number", argv[1]);
	debug("Hello %d", 5);
	number = atoi(argv[1]);
	for (int i = 2; i < argc; i++) {
		void* lib = NULL;
		char* lib_name = argv[i];
		// open library
		lib = dlopen(lib_name, RTLD_NOW);
		check(lib != NULL, "Could not open library '%s', Error: '%s'", lib_name, dlerror());
		// get 'execute' function
		lib_function func = dlsym(lib, EXEC_FUN_NAME);
		check(func != NULL, "Function with name '%s' was not found in library '%s', Error: '%s'",
		      EXEC_FUN_NAME, lib_name, dlerror());
		// execute function
		int result = func(number);
		// print result
		log_success("%s: %d", lib_name, result);
		// close the library
		check(dlclose(lib) == 0, "Error closing library '%s': Error: '%s'", lib_name, dlerror());
	}
	return 0;
error:
	return -1;
}