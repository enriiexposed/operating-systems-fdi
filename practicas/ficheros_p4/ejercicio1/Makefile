SRC = run_commands.c
BIN = $(SRC:%.c=%)
OBJ = $(SRC:%.c=%.o)
CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -g 
LIBS = 

all: $(BIN)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN): %: %.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm -f $(BIN) $(OBJ)
