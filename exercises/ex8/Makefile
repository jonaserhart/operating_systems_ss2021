CFLAGS=-g -O2 -Wall -Wextra -pthread -DNDEBUG $(OPTFLAGS)

SOURCES=$(wildcard ./**/*.c *.c)
OBJECTS=$(patsubst %.c,%,$(SOURCES))

all: $(OBJECTS)

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra -pthread $(OPTFLAGS)
dev: all

clean:
	rm -rf $(OBJECTS)
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

check:
	@echo Files with potentially dangerous functions:
	@egrep -Hn '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || echo None
		
