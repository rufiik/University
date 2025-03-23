#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#define MAX_LINE 1024 
//struct do procesow w tle
typedef struct {
    pid_t pid;
    char *command;
    int status;  
} BackgroundJob;
pid_t foreground_pid = -1;
BackgroundJob background_jobs[MAX_LINE];  
int num_jobs = 0;  
// Funkcja do usuwania bialych znaków z konca stringa
void trim_newline(char *line) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
}
// Funkcja do dodawania procesow
void add_background_job(pid_t pid, char *command) {
    // Sprawdź, czy proces już istnieje
    for (int i = 0; i < num_jobs; i++) {
        if (background_jobs[i].pid == pid) {
            return; 
        }
    }
    background_jobs[num_jobs].pid = pid;
    background_jobs[num_jobs].command = strdup(command);  
    background_jobs[num_jobs].status = 0;  
    num_jobs++;
}
// Funkcja do czyszczenia procesow z tablicy (z jobs)
void clean_up_jobs() {
    for (int i = 0; i < num_jobs; i++) {
        int status;
        pid_t result = waitpid(background_jobs[i].pid, &status, WNOHANG);
        if (result > 0) {
            // Proces zakończony naturalnie
            if (WIFEXITED(status)) {
                printf("Proces w tle o PID %d zakończony z kodem %d.\n",
                       background_jobs[i].pid, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Proces w tle o PID %d zakończony sygnałem %d.\n",
                       background_jobs[i].pid, WTERMSIG(status));
            }
            for (int j = i; j < num_jobs - 1; j++) {
                background_jobs[j] = background_jobs[j + 1];
            }
            num_jobs--;  
            i--; 
        } else if (result == -1 && errno != ECHILD) {
            perror("Błąd podczas sprawdzania zakończonych procesów");
        }
    }
}
// Funkcja do obslugi fg
void fg_command(int job_index) {
    if (job_index < 0 || job_index >= num_jobs) {
        printf("Niepoprawny indeks procesu\n");
        return;
    }
    pid_t pid = background_jobs[job_index].pid;
    printf("Przywracanie procesu o PID %d na pierwszy plan\n", pid);
    background_jobs[job_index].status = 0;  
    foreground_pid = pid;
    kill(pid, SIGCONT);
    for (int i = job_index; i < num_jobs - 1; i++) {
        background_jobs[i] = background_jobs[i + 1];
    }
    num_jobs--;
    int status;
    waitpid(pid, &status, WUNTRACED);
    if (WIFSTOPPED(status)) {
        add_background_job(pid, "foreground process");
        background_jobs[num_jobs - 1].status = 1;
    }
    foreground_pid = -1;  
}
// Funkcja do obslugi bg
void bg_command(int job_index) {
    if (job_index < 0 || job_index >= num_jobs) {
        printf("Niepoprawny indeks procesu\n");
        return;
    }
    pid_t pid = background_jobs[job_index].pid;
    printf("Wznawianie procesu w tle o PID %d\n", pid);
    background_jobs[job_index].status = 0;  
    kill(pid, SIGCONT);
}
// Funkcja do obslugi jobs
void jobs() {
    clean_up_jobs();  
    if (num_jobs == 0) {
        printf("Brak procesów w tle.\n");
        return;
    }

    for (int i = 0; i < num_jobs; i++) {
        if (background_jobs[i].status == 0) {
            printf("[%d] %d %s (running)\n", i, background_jobs[i].pid, background_jobs[i].command);
        } else if (background_jobs[i].status == 1) {
            printf("[%d] %d %s (stopped)\n", i, background_jobs[i].pid, background_jobs[i].command);
        }
    }
}
// Funkcja do obslugi ctrl-z
void handle_sigstp(int sig) {
    if (foreground_pid > 0) {
        if (kill(foreground_pid, SIGTSTP) == -1) {
            perror("Nie można wysłać SIGTSTP");
            return;
        }
        int found = 0;
        for (int i = 0; i < num_jobs; i++) {
            if (background_jobs[i].pid == foreground_pid) {
                background_jobs[i].status = 1; 
                found = 1;
                break;
            }
        }
        if (!found) {
            add_background_job(foreground_pid, "foreground process");
            background_jobs[num_jobs - 1].status = 1; 
        }
        foreground_pid = -1;  
    } else {
        printf("\nBrak procesu na pierwszym planie.\n");
        printf("lsh> ");
        fflush(stdout);
    }

}
// F
void handle_sigchld(int sig) {
clean_up_jobs();
    int saved_errno = errno;
    int status;
    pid_t pid;

    // Obsługa zakończonych procesów w tle
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            // Proces zakończony poprawnie
            for (int i = 0; i < num_jobs; i++) {
                if (background_jobs[i].pid == pid) {
                    background_jobs[i].status = 1; // Oznacz jako zakończony
                    printf("\nProces w tle o PID %d zakończony z kodem %d.\n", pid,WEXITSTATUS(status));
                    break;
                }
            }
        } else if (WIFSIGNALED(status)) {
            // Proces zakończony sygnałem
            printf("\nProces o PID %d zakończony sygnałem %d.\n", pid, WTERMSIG(status));
        }
    }

    errno = saved_errno;
}


// Funkcja do obsługi wbudowanej komendy 'cd'
int execute_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "błąd: cd oczekuje ścieżki\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("błąd cd");
        }
    }
    return 1;
}

// Funkcja do obsługi wbudowanej komendy 'exit'
int execute_exit(char **args) {
    return 0; 
}
// Funkcja do wywolywania komend i przekierowan
pid_t execute_command(char **args) {
    pid_t pid = fork();

    if (pid == 0) { 
        int i = 0;
        while (args[i] != NULL) {
            if (strcmp(args[i], "<") == 0) {
                // Przekierowanie standardowego wejścia
                FILE *input_file = fopen(args[i + 1], "r");
                if (!input_file) {
                    perror("błąd otwierania pliku wejściowego");
                    exit(EXIT_FAILURE);
                }
                dup2(fileno(input_file), STDIN_FILENO);
                fclose(input_file);
                for (int j = i; args[j - 1] != NULL; j++) {
                    args[j] = args[j + 2];
                }
                continue;
            } else if (strcmp(args[i], ">") == 0) {
                // Przekierowanie standardowego wyjścia
                FILE *output_file = fopen(args[i + 1], "w");
                if (!output_file) {
                    perror("błąd otwierania pliku wyjściowego");
                    exit(EXIT_FAILURE);
                }
                dup2(fileno(output_file), STDOUT_FILENO);
                fclose(output_file);
                for (int j = i; args[j - 1] != NULL; j++) {
                    args[j] = args[j + 2];
                }
                continue;
            } else if (strcmp(args[i], "2>") == 0) {
                // Przekierowanie standardowego wyjścia błędów
                FILE *error_file = fopen(args[i + 1], "w");
                if (!error_file) {
                    perror("błąd otwierania pliku wyjściowego błędów");
                    exit(EXIT_FAILURE);
                }
                dup2(fileno(error_file), STDERR_FILENO);
                fclose(error_file);
                for (int j = i; args[j - 1] != NULL; j++) {
                    args[j] = args[j + 2];
                }
                continue;
            }
            i++;
        }
        if (execvp(args[0], args) == -1) {
            perror("błąd execvp");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {  
        perror("błąd fork");
    }
    return pid;
}
// Funkcja do obslugi pipow |
void handle_pipe(char *line) {
    char *commands[MAX_LINE / 2 + 1]; 
    int num_commands = 0;
    // Rozdzielamy komendy na podstawie '|'
    char *command = strtok(line, "|");
    while (command != NULL) {
        commands[num_commands] = command;
        num_commands++;
        command = strtok(NULL, "|");
    }
    commands[num_commands] = NULL;

    int i;
    int fd[2], in_fd = 0; 
    pid_t pid;

    for (i = 0; i < num_commands; i++) {
        pipe(fd); 
        pid = fork();
        if (pid == 0) { 
            if (in_fd != 0) { 
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (i < num_commands - 1) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }

            close(fd[0]); 
            char *args[MAX_LINE / 2 + 1];
            int j = 0;
            char *arg = strtok(commands[i], " \t");
            while (arg != NULL) {
                args[j++] = arg;
                arg = strtok(NULL, " \t");
            }
            args[j] = NULL;
            if (execvp(args[0], args) == -1) {
                perror("błąd execvp");
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            perror("błąd fork");
            return;
        }

        close(fd[1]); 
        in_fd = fd[0]; 
    }
    for (i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}

// Funkcja do obslugi komend wbudowanych i nie tylko
void parse_and_execute(char *line) {
    // Sprawdzamy, czy w linii jest potok ('|')
    if (strchr(line, '|') != NULL) {
        // Obsługuje potoki
        handle_pipe(line);
        return;
    }
    char *args[MAX_LINE / 2 + 1]; 
    int i = 0;
    int background = 0; 
    char *token = strtok(line, " \t");
    while (token != NULL) {
        args[i] = token;
        i++;
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;
    // Sprawdzamy, czy komenda kończy się na '&' (proces w tle)
    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        background = 1;
        args[i - 1] = NULL; 
    }
    // Obsługa komend wbudowanych
    if (args[0] == NULL) {
        return; 
    } else if (strcmp(args[0], "cd") == 0) {
        execute_cd(args);
        return; 
    } else if (strcmp(args[0], "exit") == 0) {
        execute_exit(args);
        exit(0);
    } else if (strcmp(args[0], "jobs") == 0) {
        jobs();  
        return;
    } else if (strcmp(args[0], "fg") == 0) {
        if (args[1]) {
            fg_command(atoi(args[1]));  
        } else {
            fg_command(num_jobs - 1);  
        }
        return;
    } else if (strcmp(args[0], "bg") == 0) {
        if (args[1]) {
            bg_command(atoi(args[1]));  
        } else {
            bg_command(num_jobs - 1); 
        }
        return;
    }
    // Uruchamianie komendy z systemu
    pid_t pid = execute_command(args);
  if (pid > 0) {
    if (background) {
        add_background_job(pid, args[0]);  
        printf("Proces w tle o PID %d\n", pid);
    } else {
        foreground_pid = pid;  
        int status;
        waitpid(pid, &status, WUNTRACED);  

        if (WIFSTOPPED(status)) {
            printf("Proces o PID %d został zatrzymany.\n", pid);
            add_background_job(pid, args[0]); 
        } 

        foreground_pid = -1; 
    }
}
}

// Funkcja do obslugi ctrl-D
void sigint_handler(int sig) {
    printf("\nMożesz wyjść za pomocą komendy 'exit' lub Ctrl-D.\n");
    printf("lsh> ");
    fflush(stdout);
}

// Funkcja główna
int main() {
    char line[MAX_LINE];
    int status;
signal(SIGINT, sigint_handler);
signal(SIGTSTP, handle_sigstp);
 signal(SIGCHLD, handle_sigchld);
    // Pętla główna powłoki
    while (1) {
        printf("lsh> ");
        fflush(stdout);
        // Odczytujemy linię wejściową
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            if (feof(stdin)) {
                // Jeśli Ctrl+D, kończymy powłokę
                printf("\n");
                exit(0);
            } else {
                perror("błąd podczas odczytu");
                continue;
            }
        }
// Obsługujemy komendy
   	trim_newline(line); 
        parse_and_execute(line);
        clean_up_jobs();
    }
    return 0;
}
