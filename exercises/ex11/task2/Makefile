CFLAGS=-g -O2 -Wall -Wextra -rdynamic -DNDEBUG $(OPTFLAGS)

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.so,$(SOURCES))
BINS=$(patsubst %.c,%,$(SOURCES))

all: number_fun libs

libs: double negate square fib

double: 
	$(CC) $(CFLAGS) -shared -o double.so -fPIC double.c

negate: 
	$(CC) $(CFLAGS) -shared -o negate.so -fPIC negate.c

square: 
	$(CC) $(CFLAGS) -shared -o square.so -fPIC square.c

fib:
	$(CC) $(CFLAGS) -shared -o fib.so -fPIC fib.c

clean:
	rm -rf $(OBJECTS) $(BINS)
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

check:
	@echo Files with potentially dangerous functions:
	@egrep -Hn '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || echo None
		
