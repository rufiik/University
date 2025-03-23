all: zad3
zad3: zad3.o nwd.o phi.o
	gcc zad3.o nwd.o phi.o -o zadanie3
zad3.o: zad3.c
	gcc -c zad3.c
nwd.o: nwd.c
	gcc -c nwd.c
phi.o: phi.c
	gcc -c phi.c
clean:
	rm -f zadanie3 *.o *~
