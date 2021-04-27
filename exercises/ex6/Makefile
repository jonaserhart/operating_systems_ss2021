CFLAGS = -std=gnu11 -Wall -Werror -Wextra -pedantic
LDLIBS = -lm

.PHONY: all clean

all: scheduling_sim

clean:
	$(RM) scheduling_sim

scheduling_sim: main.c scheduling_sim.c scheduling_utility.c my_array.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o scheduling_sim