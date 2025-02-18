CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: disco

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: disco.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm disco disco.o
