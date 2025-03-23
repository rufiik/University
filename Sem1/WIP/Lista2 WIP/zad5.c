//zad 5 Rafal Wochna 279752
#include <stdio.h>

int main(){
	int suma=0;
	printf("%s","Liczby doskonale: \n");
	for(int i=1;i<1000;i++){
		for(int j=1;j<=(i/2);j++){
			if(i%j==0)
			suma+=j;
			}
		if(suma==i){
			printf("%d%s",i,"\n");
			}
			suma=0;		
	}
	int tab[1000]={0};
	for(int i=1;i<1000;i++){
		for(int j=1;j<=(i/2);j++){
			if(i%j==0)
			tab[i]+=j;
			}
			suma=0;		
	}
//		for(int i=1;i<1000;i++){
//	 printf("%d %s %d %s",i," ",tab[i],"\n");
//}
	printf("%s","Liczby zaprzyjaznione:\n");
	
	for(int i=1;i<1000;i++){
		if(tab[tab[i]]==i && tab[tab[i]]!=tab[i]){
			printf("%d %d %s",i,tab[i],"\n");
			tab[i]=-1;
		}
	}	
	

	
	
	
}