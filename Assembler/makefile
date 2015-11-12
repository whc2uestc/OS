CC = gcc
CFLAGS = -g -std=c99

assembler:main.o sub_asbler.o linklist.o
	$(CC) $(CFLAGS) $^ -o $@
main.o:main.c sub_asbler.h linklist.h
	$(CC) -c $(CFLAGS) $< -o $@
sub_asbler.o:sub_asbler.c sub_asbler.h
	$(CC) -c $(CFLAGS) $< -o $@
linklist.o:linklist.c linklist.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
