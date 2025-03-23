#include "zad3.h"
int phi(long int n){
	int wynik=1;
	for(int i=2;i<n;i++){
		if((NWD(i,n))==1){
			wynik++;
		}
	}
	
	return wynik;
	
	
}