#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

int main(void) {
    
    if ((mkfifo("fifo0001", S_IRUSR | S_IWUSR)) == -1) {
        if (errno == EEXIST)
            perror("mkfifo()");
        else {
            perror("mkfifo()");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Es können max. %ld Bytes in die FIFO geschrieben werden\n",
           pathconf("fifo0001", _PC_PIPE_BUF));
    
    printf("Ausserdem können max %ld FIFOs geöffnet sein\n",
           sysconf(_SC_OPEN_MAX));
    
    return EXIT_SUCCESS;
}

