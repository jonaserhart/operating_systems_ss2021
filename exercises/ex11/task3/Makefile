CFLAGS=-g -O2 -Wall -Wextra -rdynamic -D_GNU_SOURCE -shared -fPIC $(OPTFLAGS)

my_malloc: 
	$(CC) $(CFLAGS) -o my_malloc.so my_malloc.c

clean:
	rm -rf my_malloc.so
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`