int NWD(int a,int b){
	int tym;
	while(b!=0){
		tym=b;
		b=a%b;
		a=tym;
	}
	return a;	
}