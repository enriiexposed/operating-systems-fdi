CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all : auxiliar.o init.o auxiliar.h
	gcc $(CFLAGS) -o prueba auxiliar.o init.o $(LIBS)

.PHONY: clean

clean:
	-rm *.o prueba
