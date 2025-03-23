#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = 1; // PID procesu init

    if (kill(pid, SIGKILL) == -1) {
        perror("Nie udało się wysłać SIGKILL do procesu init");
    } else {
        printf("Sygnał SIGKILL wysłany do procesu init przez root'a ale proces nie jest mozliwy do zabicia\n");
    }

    return 0;
}
