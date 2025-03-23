#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int wszystkie(int mozliwosci[1296][4]) {
    int count = 0;
    int pasujace=0;
    for (int i = 1; i <= 6; i++) {
        for (int j = 1; j <= 6; j++) {
            for (int k = 1; k <= 6; k++) {
                for (int l = 1; l <= 6; l++) {
                	if(mozliwosci[count][0]!=-1){
                		mozliwosci[count][0] = i;
                    	mozliwosci[count][1] = j;
                    	mozliwosci[count][2] = k;
                    	mozliwosci[count][3] = l;
                    	pasujace++;
						
					}
                   count++;
                  
                    
                    
                }
            }
        }
    }
    return pasujace;
}
void zmniejszanie(int mozliwosci[][4], int kod[], int red, int white) {
    int l_m = 1296;
    for (int i = 0; i < l_m; ++i) {
        if (mozliwosci[i][0] == -1) {
            continue;
        }
        int currentRed = 0;
        int currentWhite = 0;

        for (int j = 0; j < 4; ++j) {
            if (mozliwosci[i][j] == kod[j]) {
                ++currentRed;
            } else {
                for (int k = 0; k < 4; ++k) {
                    if (mozliwosci[i][j] == kod[k] && mozliwosci[i][j] != kod[j]) {
                        ++currentWhite;
                        break;
                    }
                }
            }
        }
        
        if (currentRed != red || currentWhite != white) {
            mozliwosci[i][0] = -1;
        }
    }
	

}


void sprawdz(int mozliwosci[1296][4],int kod[4]){
	int j=0;
	while(mozliwosci[j][0]==-1){
		j++;
	}
		for(int i=0;i<4;i++){
		kod[i]=mozliwosci[j][i];
	}
	
	
}
void mastermind(){
	int mozliwosci[1296][4]={0};
	int kod[4];
	int wynik=wszystkie(mozliwosci);
	int red,white;
	while(red!=4 || wynik!=1){
	printf("%s %d\n","Liczba mozliwosci: ",wynik);
	wszystkie(mozliwosci);
	sprawdz(mozliwosci,kod);
	
	printf("[%d] [%d] [%d] [%d] \n",kod[0],kod[1],kod[2],kod[3]);
	
	printf("%s", "red: ");
	scanf("%d", &red);
	printf("%s","white: ");
	scanf("%d", &white);
	zmniejszanie(mozliwosci,kod,red,white);
	
	wynik=wszystkie(mozliwosci);
	
	if(wynik==0){
		printf("%s","You are cheating!");
		break;
		}
	}
	if(wynik==1){
		printf("%s","I win!");	
	}
	
}


int main(){
	mastermind();


	
	
	

}