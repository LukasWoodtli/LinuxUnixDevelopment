#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF 4096

int main(void) {
	char puffer[BUF], inhalt[BUF], delete_fifo[BUF];
	int fd, fdx;
	
    sprintf(puffer, "fifo.%d", getpid());
	inhalt[0] = '\0';
    
    /* first line contains PID */
    sprintf(inhalt, "%d\n", getpid());
    
    umask(0);
    
    if (mkfifo(puffer, O_RDWR | 0666) < 0) {
        if (errno == EEXIST)
            printf("Verwende vorhandene FIFO\n");
        else {
            perror("mkfifo()");
            exit(EXIT_FAILURE);
        }
    }
    
    fd = open("fifo.polling", O_WRONLY);
    fdx = open(puffer, O_RDWR);
    
    if (fd == -1 || fdx == -1) {
        perror("open()");
        exit(EXIT_FAILURE);
    }

    strcmp(delete_fifo, puffer);
    
    printf("Bitte Nachricht eingeben (CTR+D zum Beenden)\n>");
    while (fgets(puffer, BUF, stdin) != NULL) {
        if (strlen(inhalt) + strlen(puffer) < BUF) {
            strcat(inhalt, puffer);
            printf(">");
        }
        else
            break;
    }
    
    inhalt[BUF - 1] = '\0';
    write(fd, inhalt, BUF);
    
    if (read(fdx, puffer, BUF))
        printf("%s\n", puffer);
    
    /* remove fifo for answer */
    unlink(delete_fifo);
    
	return EXIT_SUCCESS;
}

