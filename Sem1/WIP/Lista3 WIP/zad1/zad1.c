//zad1 Rafal Wochna 279752
#include <stdio.h>
#include <stdbool.h>
#include "zad1.h"
int main(void){
	char napis[100];
	scanf("%s",napis);
if(palindrom(napis)==1)	
	printf("%s","Napis jest palindromem \n");
else
	printf("%s","Napis nie jest palindromem \n");
	
}
