CC := gcc
CFLAGS := -std=gnu11 -Wall -ggdb -lcurl

LDFLAGS := -lcurl

SRC_DIR = ./src

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:%.c=%.o)

dgrabber: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f dgrabber *.out **/*.o *.o

.PHONY: all clean
