#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#define PROGRAM "/usr/bin/find"
#define PRIORITY 19
#define MAX_ARGS_LEN 1024

int main (int argc, char** argv) {
    int i;
    pid_t pid;
    char *args[MAX_ARGS_LEN + 1] = {PROGRAM, NULL};
    
    seteuid(getuid()); /* privilegien entfernen */
    
    for (i = 1; i < argc && i < MAX_ARGS_LEN; ++i) {
        args[i] = argv[i];
    }
    args[i] = NULL;
    
    pid = fork();
    
    if (pid == 0) {
        execvp(PROGRAM, args);
    }
    else {
        seteuid(0); /* privilegien wieder einfuehren */
        setpriority(PRIO_PROCESS, pid, PRIORITY);
    }
    return EXIT_SUCCESS;
}

