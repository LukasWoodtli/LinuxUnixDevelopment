#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define PROGRAMM "./my_programm"

int main(void) {
    pid_t pid;
    
    switch (pid = fork()) {
        case -1:
            perror("fork()");
            return EXIT_FAILURE;
            break;
            
        case 0:
            execlp(PROGRAMM, PROGRAMM, "Hallo", "Welt", NULL);
            printf("Wird niemals erreicht ...\n");
            break;

        default:
            if (waitpid(pid, NULL, 0) != pid) {
                perror("waitpid()");
                return EXIT_FAILURE;
            }
    }
    return EXIT_SUCCESS;
}
