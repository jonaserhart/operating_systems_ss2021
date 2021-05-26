CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)

my_malloc: 
	$(CC) -shared -o my_malloc.so -fPIC my_malloc.c

clean:
	rm -rf my_malloc.so
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`