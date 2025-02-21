CC=gcc
CFLAGS=-I include/ -L lib/ -lraylib -lgdi32 -lwinmm

default: 
	$(CC) main.c -o main.exe $(CFLAGS)
