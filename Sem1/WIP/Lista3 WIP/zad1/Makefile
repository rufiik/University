all: zad1

zad1: zad1.o palindrom.o 
	gcc zad1.o palindrom.o -o zadanie1
zad1.o: zad1.c
	gcc -c zad1.c
palindrom.o: palindrom.c
	gcc -c palindrom.c
clean:
	rm -f zadanie1 *.o *~
