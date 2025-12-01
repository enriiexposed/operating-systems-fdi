SRC = $(wildcard *.c)
TARGETS_SRC = $(wildcard ej*.c)
TARGETS = $(TARGETS_SRC:%.c=%)

CC = gcc
CFLAGS = -pthread -g $(CPPFLAGS)
LDFLAGS = -pthread
LIBS =

all: $(TARGETS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETS): %: %.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm $(TARGETS) $(SRC:%.c=%.o)

