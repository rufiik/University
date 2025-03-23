//zad3 Rafal Wochna 279752
#include <stdio.h>
#include <math.h>
int main(){ 
double n=1,wynik=1;

while(n<=1000){
	wynik*=pow(n,0.001);
	n++;
}

printf("Wynik wynosi: %g",wynik);
}
