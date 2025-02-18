CC = gcc
CFLAGS = -Wall -g

PROGS = write_strings read_strings

all : $(PROGS) 

$(PROGS) : % : %.o
	gcc -g -o $@ $^
	
%.o : %.c 
	gcc -c $(CFLAGS) $< -o $@

clean :
	-rm -f *.o $(PROGS) 
