#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
const char *signal_names[] = {
    "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1",
    "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP",
    "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGIO", "SIGPWR",
    "SIGSYS"};
void signal_handler(int sig) {
    printf("Otrzymano sygnał: %d\n", sig);
}

int main() {
    for (int i = 1; i <= 31; i++) {
        // Próbujemy ustawić obsługę sygnału dla każdego z sygnałów
        if (signal(i, signal_handler) == SIG_ERR) {
 printf("Nie można obsłużyć sygnału: %d (%s)\n", i, signal_names[i - 1] ? signal_names[i - 1] : "nieznany");
        } else {
            printf("Obsługuje sygnał: %d (%s)\n", i, signal_names[i - 1] ? signal_names[i - 1] : "nieznany");
        }
    }
    return 0;
}
