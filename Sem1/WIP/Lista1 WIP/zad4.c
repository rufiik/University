//Rafal Wochna 279752
#include <stdio.h>
int main(){
int n;
scanf("%d",&n);


for(int j=0;j<n;j++){
	for(int k=n-j;k>1;k--)
	printf("%c",' ');	
for(int i=0;i<=j+j;i++){
	printf("%c",'*');
	
}
printf("%c",'\n');

}

}