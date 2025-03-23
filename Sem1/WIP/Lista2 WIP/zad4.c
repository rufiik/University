//zad 4 Rafal Wochna 279752
#include <stdio.h>
#include<time.h>
int przestepny(int r){
	if((r%4==0 && r%100!=0) ||( r%400==0))
		return 1;
	else
		return 0;	
}

int main(){
	//CZESC 1
	
	int d,m,r,liczba_dni=0,liczba_dni_u=0;
	scanf("%d %d %d",&d,&m,&r);
for(int i=1;i<m;i++){
	switch(i){
		case 2:
			liczba_dni+=28+przestepny(r);
			break;
		case 4:
		case 6:
		case 9:
		case 11:		
			liczba_dni+=30;
			break;
		default:
			liczba_dni+=31;
			break;

	
}
	
	}
	liczba_dni+=d;
	for(int i=1;i<r;i++){
			liczba_dni+=(365+przestepny(i));
		}
	printf("%s %d %s","Minelo: ",liczba_dni,"dni \n");

// CZESC 2
	time_t current_time;
    struct tm local_time;
    // Pobierz aktualny czas
    time(&current_time);
    // Przeksztalc czas na lokalny czas
    local_time = *localtime(&current_time);
   int d_day=local_time.tm_mday ,d_mon=local_time.tm_mon + 1,d_year=local_time.tm_year + 1900;
	
	for(int i=1;i<d_mon;i++){
	switch(i){
		case 2:
			liczba_dni_u+=28+przestepny(d_year);
			break;
		case 4:
		case 6:
		case 9:
		case 11:		
			liczba_dni_u+=30;
			break;
		default:
			liczba_dni_u+=31;
			break;

	
}
	}
	liczba_dni_u+=d_day;
	liczba_dni_u-=52;
	for(int i=2003;i<d_year;i++){
			liczba_dni_u+=(365+przestepny(i));
		}
		
		int liczba_s=liczba_dni_u*24*60*60;
		printf("%s %d %s","Od 21.02.2003 minelo: ",liczba_dni_u,"dni \n");
		
		printf("%s %d %s","Od 21.02.2003 minelo : ",liczba_s,"sekund \n");
	
	
	
	
	
	
	
	
	
	
	
	
	
}