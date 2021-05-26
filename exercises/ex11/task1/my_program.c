#include <stdio.h>
#include <stdlib.h>

int this_function_should_increment_a_number_by_ten(int x) {
	return x + 10;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: ./my_program <number>\n");
		return 1;
	}

	int number = atoi(argv[1]);
	int result = this_function_should_increment_a_number_by_ten(number);
	printf("%d", result);
	return 0;
}