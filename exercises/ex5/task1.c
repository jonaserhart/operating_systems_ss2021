#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

	if (argc != 3) {
		return -1;
	}
	u_int64_t n, b, sharedMemory;
	u_int64_t* myPtr;
	u_int64_t result = 0;

	n = strtoull(argv[1], NULL, 10);
	b = strtoull(argv[2], NULL, 10);

	// memory
	sharedMemory = shm_open("/myptr", O_RDWR | O_CREAT, 0600);
	ftruncate(sharedMemory, sizeof(u_int64_t) * (b + 1));

	// map file into memory
	myPtr =
	    mmap(0, sizeof(u_int64_t) * (b + 1), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemory, 0);

	if (fork() == 0) {
		printf("Produer number: %llu\n", n);
		for (u_int64_t i = 0; i < n; i++) {
			myPtr[(i % b)] = i + 1;
		}
		myPtr[b] = result;
		exit(0);
	}
	if (fork() == 0) {
		printf("Consumer number: %llu\n", b);
		for (u_int64_t i = 0; i < b; i++) {
			result += myPtr[(i % b)];
		}
		myPtr[b] = result;
		exit(0);
	}

	while (wait(NULL) > 0)
		;

	printf("Result: %llu", myPtr[b]);

	// cleanup
	munmap(myPtr, sizeof(u_int64_t) * (b + 1));
	close(sharedMemory);
	shm_unlink("/myptr");
	return 0;
}