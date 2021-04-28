CFLAGS = -std=gnu11 -Wall -Werror -Wextra -O2 -g -pthread -DNDEBUG $(OPTFLAGS)
FILESTOMAKE = task1 task2 philosophers

all: $(FILESTOMAKE)

dbg: CFLAGS = -std=gnu11 -Wall -Werror -Wextra -O2 -g -pthread $(OPTFLAGS)
dbg: all

clean:
	rm -rf $(FILESTOMAKE) *.dSYM