
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

all: produtor-consumidor

produtor-consumidor: produtor-consumidor.o
		$(CC) $(CFLAGS) -o produtor-consumidor produtor-consumidor.o

produtor-consumidor.o: produtor-consumidor.c
		$(CC) $(CFLAGS) -c produtor-consumidor.c

clean:
		rm -f *.o produtor-consumidor
		
