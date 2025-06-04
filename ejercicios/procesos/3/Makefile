CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -pthread
TARGET = main
TARGET_SRC = main.c
TARGET_OBJ = $(TARGET_SRC: %.c= %.o)

all: $(TARGET)

%.o: %.c makefile
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(TARGET_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(TARGET_OBJ) $(LIBS)

.PHONY: clean

clean:
	-rm $(TARGET_OBJ) $(TARGET)