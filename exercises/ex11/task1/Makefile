CFLAGS=-g -O2 -Wall -Wextra $(OPTFLAGS)

some_library:
	$(CC) $(CFLAGS) -rdynamic -shared -o some_library.so -fPIC some_library.c

my_program:
	$(CC) $(CFLAGS) -L. -static  -o my_program my_program.c

clean:
	rm my_program