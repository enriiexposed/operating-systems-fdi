CC = gcc
CFLAGS = -Wall -g

PROG = student-records
HEADERS = defs.h
OBJECTS = $(PROG).o

all : $(PROG)

$(PROG) : $(OBJECTS)
	gcc -g -o $(PROG) $(OBJECTS)

%.o : %.c $(HEADERS)
	gcc -c $(CFLAGS) $< -o $@

clean :
	-rm -f $(OBJECTS) $(PROG)
