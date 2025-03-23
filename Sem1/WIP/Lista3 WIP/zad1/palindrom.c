#include <stdbool.h>
#include <string.h>
bool palindrom(char napis[]){
int dl=strlen(napis)-1;	
for(int i=0;i<=(dl/2);i++){
	if(napis[i]==napis[dl-i]){
	}
	else{
		return false;
	}
}
	return true;
	
	
}
