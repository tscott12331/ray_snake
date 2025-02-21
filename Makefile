CC=gcc
CFLAGS=-I include/ -L lib/ -lraylib -lgdi32 -lwinmm


.PHONY: all

CFILES := $(shell find . -name "*.c")

OBJS := $(patsubst %.c,%.o,$(CFILES))
MAIN_EXE := snake

all: $(OBJS)
	$(CC) $^ -o $(MAIN_EXE) $(CFLAGS)


%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm -f $(OBJS)
