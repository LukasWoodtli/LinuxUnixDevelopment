#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>

#define EXIT(s) {fprintf(stderr, "%s", s); exit(EXIT_FAILURE);}

enum { ERROR = -1, SUCCESS = 0 };

int main(int argc, char** argv) {
    int fd[2];
    pid_t pid;
    FILE *pipe_writer, *file;
    char puffer[PIPE_BUF];
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s DateiZumLesen\n", *argv);
        exit(EXIT_FAILURE);
    }
    
    if ((file = fopen(argv[1], "r")) == NULL)
        EXIT("Fehler bei fopen() ...\n");
    if (pipe(fd) == ERROR)
        EXIT("Fehler bei pipe() ...\n");
    if ((pid = fork()) == ERROR)
        EXIT("Fehler bei fork() ...\n");
    
    if (pid > 0) { /* parent */
        close(fd[0]); /* close reading side */
        if ((pipe_writer = fdopen(fd[1], "w")) == NULL)
            EXIT("Fehler bei fdopen() ...\n");
    
        fread(&puffer, PIPE_BUF, 1, file);
        fputs(puffer, pipe_writer);
        fclose(pipe_writer);
    
        /* closing writing side indicates to child that we are finished */
        if (waitpid(pid, NULL, 0) == ERROR)
            EXIT("Fehler bei waitpid() ...\n");
    
        exit(EXIT_SUCCESS);

    } else { /* child */
        sleep(1);
        close(fd[1]);
        
        if (fd[0] != STDIN_FILENO) {
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
                EXIT("Fehler bei dup2() ...\n");
            close(fd[0]);
        }
        
        if (execl("/usr/bin/less", "less", NULL) == ERROR)
            EXIT("Fehler bei execl() ...\n");
    }
    
    return EXIT_SUCCESS;
}
