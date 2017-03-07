#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    char buf[64];
    int pid;
    memset(buf, '\0', sizeof(buf));
    if (-1 == readlink("/proc/self", buf, sizeof(buf) - 1)) {
        perror("readlink()");
        return EXIT_FAILURE;
    }
    sscanf(buf, "%d", &pid);
    printf("Meine Process-ID lautet: %d\n", pid);
    return EXIT_SUCCESS;
}
