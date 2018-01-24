CC := gcc
CFLAGS := -std=c11 -Wall -ggdb

LDFLAGS := -lcurl

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=%.o)

dgrabber: $(OBJECTS)
	$(CC) -o $@ $(LDFLAGS) $^

clean:
	rm -f dgrabber *.out *.o

.PHONY: all clean
