#include <stdio.h>
int matchFromIndex(char* wzorzec, char* lancuch, int indexWzorca, int indexLancucha) {
	
    if (wzorzec[indexWzorca] == '\0' && lancuch[indexLancucha] == '\0') {
        return 1;
    }
    
    if (wzorzec[indexWzorca] == '\0' && lancuch[indexLancucha]!='\0') {
        return 0; 
    }

    if (wzorzec[indexWzorca] == lancuch[indexLancucha] || wzorzec[indexWzorca] == '?') {
        return matchFromIndex(wzorzec, lancuch, indexWzorca + 1, indexLancucha + 1);
    }

    if (wzorzec[indexWzorca] == '*') {
        while (lancuch[indexLancucha] != '\0') {
            if (matchFromIndex(wzorzec, lancuch, indexWzorca + 1, indexLancucha)) {
                return 1; 
            }
            indexLancucha++;
        }

        //pusty
        return matchFromIndex(wzorzec, lancuch, indexWzorca + 1, indexLancucha);
    }
    return 0;
}

int match(char* wzorzec, char* lancuch) {
    return matchFromIndex(wzorzec, lancuch, 0, 0);
}

int main() {
    printf("%d\n", match("a*b*a", "abababababababababababa"));
    printf("%d\n", match("a*b*a", "ababababababababababab") );           

}