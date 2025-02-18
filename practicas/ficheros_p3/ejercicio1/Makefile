SRC = $(wildcard *.c)
TARGETS = $(SRC:%.c=%)

CC = gcc
CFLAGS = -g
LDFLAGS = 
LIBS = 

all: $(TARGETS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETS): %: %.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm $(TARGETS) $(SRC:%.c=%.o)

