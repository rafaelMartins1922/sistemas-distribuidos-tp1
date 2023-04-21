CC = gcc
CFLAGS = -Wall -Wextra -g

all: produtor consumidor

produtor: produtor.c
	$(CC) $(CFLAGS) $< -o $@

consumidor: consumidor.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f produtor consumidor