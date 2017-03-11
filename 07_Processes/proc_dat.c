#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    printf("Meine PID: %d\n", getpid());
    printf("Mein Elternprozess (PPID): %d\n", getppid());
    printf("UID effektiv : %d --> real : %d\n", geteuid(), getuid());
    printf("GID effektiv : %d --> real : %d\n", getegid(), getgid());
    return EXIT_SUCCESS;
}
