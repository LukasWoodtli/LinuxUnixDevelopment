#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FIFO "fifo0001.1"


int main(void) {
    int fd_fifo;
    char puffer[] = "Dieser Text ist für die FIFO\n";
    char buf[100];
    int n;
    
    umask(0);
    
    if ((mkfifo(FIFO, 0666)) == -1) {
        if (errno == EEXIST)
            perror("mkfifo()");
        else {
            perror("mkfifo()");
            exit(EXIT_FAILURE);
        }
    }
    
    if ((fd_fifo = open(FIFO, O_RDWR)) == -1) {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    
    n = write(fd_fifo, puffer, strlen(puffer));
    
    if (read(fd_fifo, &buf, n) == -1) {
        perror("read()");
        exit(EXIT_FAILURE);
    }
    
    printf("Gelesen aus der FIFO: %s\n", buf);
    buf[0] = '\0';
    
    return EXIT_SUCCESS;
}

