
int execute(const int input) {
	if (input == 0) {
		return 0;
	}
	if (input == 1) {
		return 1;
	}
	if (input > 1) {
		return execute(input - 1) + execute(input - 2);
	}
	return 0;
}