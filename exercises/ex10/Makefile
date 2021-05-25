CFLAGS=-g -O3 -Wall -Werror -pthread -DNDEBUG

all:
	$(CC) -o alloc_test allocator_test.c membench.c list.c allocator1.c allocator2.c $(CFLAGS) 


clean:
	rm -rf alloc_test
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

check:
	@echo Files with potentially dangerous functions:
	@egrep -Hn '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || echo None
		
