//Rafal Wochna 279752
#include <stdio.h>
int main(){
float a,b,c,pierwiastek1,pierwiastek2;
scanf("%g %g %g",&a,&b,&c);
float delta;
delta=b*b-4*a*c;

if(delta<0){
	printf("%s","Nie ma rozwiazania w zbiorze liczb rzeczywistych");
}
else if(delta==0){
	pierwiastek1=-b/(2*a);
	printf("%s %g","Pierwiastek wynosi = ",pierwiastek1);
}
else{
	pierwiastek1=(-b-sqrt(delta))/(2*a);
	pierwiastek2=(-b+sqrt(delta))/(2*a);
	printf("%s %g %g","Pierwiastki wynosza = ",pierwiastek1,pierwiastek2);
}



}