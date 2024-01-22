CC = gcc
CFLAGS = -ansi -pedantic -Wall

mainmat: index.o
	$(CC) $(CFLAGS) index.o -o index

index.o: index.c
	$(CC) -c $(CFLAGS) index.c -o index.o

clean:
	rm -f index.o