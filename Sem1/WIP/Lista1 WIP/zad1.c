//Rafal Wochna 279752
#include <stdio.h>
int main(){
	char s[]="ABRAKADABRA";	
	for(int j=0;j<11;j++){
	for(int k=0;k<j;k++){
	printf("%c",' ');	
	}	
	
	for(int i=0;i<11-j;i++){
		printf("%c",s[i]);
		printf("%c",' ');
		
	}
	printf("%c",'\n');

	}
	
//	printf("%s","A B R A K A D A B R A");
}