#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

static int global_var = 1;

int main(void) {
    pid_t pid;
    int lokal_var = 1;
    
    switch (pid = fork()) {
        case -1:
            printf("Fehler bei fork()\n");
            break;
        
        case 0:
            sleep(1);
            
            printf("--- Im Kindprocess (%d) ---\n", getpid());
            printf("global_var = %d; Speicheradresse: %p\n", global_var, &global_var);
            printf("lokal_var  = %d; Speicheradresse: %p\n", lokal_var, &lokal_var);
            fflush(stdout);
            
            ++global_var;
            ++lokal_var;
            
            if (fork() == -1)
                printf("Fehler bei fork() (2)\n");
            else {
                printf("--- Im Kind-Kindprocess (%d) ---\n", getpid());
                printf("global_var = %d; Speicheradresse: %p\n", global_var, &global_var);
                printf("lokal_var  = %d; Speicheradresse: %p\n", lokal_var, &lokal_var);
                fflush(stdout);
                break;
            }
            
            break;
        
        default:
            printf("--- Im Elternprozess (%d) ---\n", getpid());
            printf("global_var = %d; Speicheradresse: %p\n", global_var, &global_var);
            printf("lokal_var  = %d; Speicheradresse: %p\n", lokal_var, &lokal_var);
            fflush(stdout);
            
    }
    
    return EXIT_SUCCESS;
}
