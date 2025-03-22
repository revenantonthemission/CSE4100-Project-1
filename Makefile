main : main.o hash.o list.o bitmap.o list.h hash.h bitmap.h
	gcc -o main main.o hash.o list.o
main.o : hash.h list.h bitmap.h main.c
	gcc -o main.o -c main.c
bitmap.o : bitmap.h bitmap.c
	gcc -o bitmap.o -c bitmap.c
hash.o : hash.h list.h hash.c
	gcc -o hash.o list.o -c hash.c
list.o : list.h list.c
	gcc -o list.o -c list.c
