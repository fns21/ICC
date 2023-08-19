CFLAGS = -Wall -g
all: main

main: analiseInterv.o operacoes.o main.o
	gcc -o main analiseInterv.o operacoes.o main.o -lm

analiseInterv.o: analiseInterv.c analiseInterv.h 
	gcc -c analiseInterv.c -o analiseInterv.o
	
operacoes.o: operacoes.c operacoes.h
	gcc -c operacoes.c -o operacoes.o

main.o: main.c
	gcc -c main.c -o main.o

clean:
	rm -f *.o main

purge: clean
	rm -f main