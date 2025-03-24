CC = gcc
CFLAGS = -Wall -g

# Object files
OBJS = main.o hash.o list.o bitmap.o hex_dump.o

# Target
main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Dependencies
main.o: main.c list.h hash.h bitmap.h
	$(CC) $(CFLAGS) -c main.c

hash.o: hash.c hash.h list.h
	$(CC) $(CFLAGS) -c hash.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

bitmap.o: bitmap.c bitmap.h hex_dump.h
	$(CC) $(CFLAGS) -c bitmap.c

hex_dump.o: hex_dump.c hex_dump.h
	$(CC) $(CFLAGS) -c hex_dump.c	
# Clean
clean:
	rm -f $(OBJS) main
