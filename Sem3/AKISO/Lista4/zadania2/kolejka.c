#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t signal_count = 0;

void signal_handler(int sig) {
    signal_count++;
}

int main() {
    signal(SIGUSR1, signal_handler);
    for (int i = 0; i < 52; i++) {
        kill(getpid(), SIGUSR1);
    }
    printf("Otrzymano %d sygnałów SIGUSR1\n", signal_count);
    return 0;
}
