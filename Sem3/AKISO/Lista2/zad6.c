#include <stdio.h>
#include <unistd.h>

int main() {
    for (int i = 0; i < 256; i++) {
        // Sekwencja escape "\033[..." ustawia kolor tekstu
        printf("\033[38;5;%dm%s\n", i,"Hello, World!"); // 38;5;X -> kolor tekstu z palety 256
        usleep(10000);
    }
    
    // Resetuj kolor do domyślnego
    printf("\033[0m");
    return 0;
}

