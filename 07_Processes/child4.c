#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 4094

int main(void) {
    char log[MAX];
    printf("1. fork()\n"); fflush(stdout);
    fork();
    printf("2. fork()\n"); fflush(stdout);
    fork();
    printf("3. fork()\n"); fflush(stdout);
    fork();
    sleep(2);
    snprintf(log, MAX, "PID: %d PPID: %d\n", getpid(), getppid());
    printf("%s", log);
    fflush(stdout);
    return EXIT_SUCCESS;
}
