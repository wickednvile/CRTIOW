CC=gcc
CFLAGS=-O3 -Wall -Werror --std=c17 -I include
LIBS=-lm

all:
	$(CC) src/main.c -o out $(CFLAGS) $(LIBS) && time ./out > a.ppm
