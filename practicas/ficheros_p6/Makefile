SRC = $(wildcard *.c)
BIN = $(SRC:%.c=%)
OBJ = $(SRC:%.c=%.o)
CC = gcc
CFLAGS = -g -pthread -O0
LDFLAGS = -g -pthread
LIBS = -lrt

all: $(BIN)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN): %: %.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm $(BIN) $(OBJ)
