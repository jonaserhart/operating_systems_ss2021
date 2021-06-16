CFLAGS=-g -O2 -Wall -Wextra -pthread -std=c99 $(OPTFLAGS)

# get all .c files
SOURCES=$(wildcard *.c)
# get all object files to corresponding c files
OBJECTS=$(patsubst %.c,%,$(SOURCES))

# all header files
HEADERS=$(wildcard *.h)

# get all files to zip
FILESTOZIP= $(SOURCES) $(HEADERS) Makefile .clang-format README.md

# make all c files into object files
all: CFLAGS+= -DNDEBUG
all: objs

# compile without NDEBUG file for debugging
dev: objs

# make objects
objs: $(OBJECTS)

# clear up all object files, *.dSYM files and submission folder
clean:
	rm -rf $(OBJECTS)
	rm -rf `find . -name "*.dSYM" -print`

# clean submission folder
cleansub:
	rm -rf ./submission

# clean everything
cleanall: clean
cleanall: cleansub

# check for dangerous (string) functions
check:
	@echo Files with potentially dangerous functions:
	@egrep -Hn '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || echo None

# make a zip file for submssion
zip:
	mkdir -p ./submission
	zip -r ./submission/submission-time-`date +"%H%M"`$(COMMENT).zip $(FILESTOZIP)

LEAKCALL=./program arg1 arg2

leakdoc:
	rm -f leakdoc.txt
	touch leakdoc.txt
	leaks --atExit -- $(LEAKCALL) > leakdoc.txt

valgrinddoc:
	rm -f leakdoc.txt
	touch leakdoc.txt
	valgrind --leak-check=yes $(LEAKCALL) > leakdoc.txt