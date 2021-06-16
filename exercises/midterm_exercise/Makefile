CFLAGS=-g -O2 -Wall -Wextra -pthread -std=c99 $(OPTFLAGS)

SOURCES=$(wildcard ./**/*.c *.c)
OBJECTS=$(patsubst %.c,%,$(SOURCES))

# all header files
HEADERS=$(wildcard *.h)

# get all files to zip
FILESTOZIP= $(SOURCES) $(HEADERS) Makefile .clang-format README.md

all: CFLAGS+= -DNDEBUG
all: objs

# compile without NDEBUG file for debugging
dev: objs

objs: $(OBJECTS)

clean:
	rm -rf $(OBJECTS)
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

check:
	@echo Files with potentially dangerous functions:
	@egrep -Hn '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || echo None

# make a zip file for submssion
zip:
	mkdir -p ./submission
	zip -r ./submission/submission-time-`date +"%H%M"`$(COMMENT).zip $(FILESTOZIP)

LEAKCALL=./factory 1 100

leakdoc:
	rm -f leakdoc.txt
	touch leakdoc.txt
	leaks --atExit -- $(LEAKCALL) > leakdoc.txt

valgrinddoc:
	rm -f leakdoc.txt
	touch leakdoc.txt
	valgrind --leak-check=yes $(LEAKCALL)