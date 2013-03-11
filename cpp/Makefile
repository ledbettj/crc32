CPPFLAGS=  -Wall
CPP=g++

all: crc32-opt crc32-def


crc32-opt: crc32.cc
	$(CPP) crc32.cc $(CPPFLAGS) -O3 -o $@

crc32-def: crc32.cc
	$(CPP) crc32.cc $(CPPFLAGS) -o $@


check-syntax:
	$(CPP) $(CFLAGS) -fsyntax-only $(CHK_SOURCES)

clean:
	rm -f crc32-opt crc32-def
