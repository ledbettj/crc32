CFLAGS= -std=c99 -Wall
CC=gcc

all: crc32-opt crc32-def


crc32-opt: crc32.c
	gcc crc32.c $(CFLAGS) -O3 -o $@

crc32-def: crc32.c
	gcc crc32.c $(CFLAGS) -o $@


check-syntax:
	$(CC) $(CFLAGS) -fsyntax-only $(CHK_SOURCES)

clean:
	rm -f crc32-opt crc32-def
