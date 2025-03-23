//zad 1 Rafal Wochna 279752
#include <stdio.h>
int main(){
	int n;
	scanf("%d",&n);
	if((n%4==0 && n%100!=0) ||( n%400==0)){
		printf("%s %d %s","Rok",n,"jest przestepny \n");
	}
		else{
			printf("%s %d %s","Rok",n,"nie jest przestepny \n");
	}
}
