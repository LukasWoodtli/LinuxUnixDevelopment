#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define FIFO "fifo0001.4"


int main(void) {
    int r_fifo, w_fifo;
    char puffer[] = "Dieser Text ist für die FIFO an den Elternprozess\n";
    char buf[100];
    pid_t pid;
    
    if ((mkfifo(FIFO, S_IRUSR | S_IWUSR)) == -1) {
        if (errno == EEXIST)
            perror("mkfifo()");
        else {
            perror("mkfifo()");
            exit(EXIT_FAILURE);
        }
    }
    
    pid = fork();
    if (pid == -1) {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
        if ((r_fifo = open(FIFO, O_RDONLY)) < 0) {
            perror("open()");
            exit(EXIT_FAILURE);
        }
        
        /* waiting until child ends */
        while (wait(NULL) != pid);
        
        read(r_fifo, buf, strlen(puffer));
        buf[strlen(puffer)] = '\0';
        
        printf(" --- Elternprozess ---\n");
        printf("%s", buf);
        close(r_fifo);
    }
    else {
        /* child writes to FIFO */
        printf(" --- Kindprozess ---\n");
        if ((w_fifo = open(FIFO, O_WRONLY)) < 0) {
            perror("open()");
            exit(EXIT_FAILURE);
        }
        
        write(w_fifo, puffer, strlen(puffer));
        close(w_fifo); /* EOF */
        
        exit(EXIT_SUCCESS);
    }
    
    printf(" --- Ende ---\n");
    return EXIT_SUCCESS;
}

