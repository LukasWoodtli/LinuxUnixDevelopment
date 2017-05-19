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

#define TEXT "Habe Anfrage erhalten\n"


static void an_den_drucker(const char* text) {
    FILE *p;
    
    p = popen("lpr", "w");
    if (p == NULL) {
        perror ("popen()");
        exit (EXIT_FAILURE);
    }

    printf("Sende Auftrag an Drucker ...\n");
    printf("%s", text);
    // uncomment this for sending text to printer
    //fprintf(p, "%s", text);
    fflush(p);
    pclose(p);
}

int main(void) {
	char puffer[BUF], antwort[BUF], pid[6];
	int r_fd, w_fd, n, i;

    umask(0);
    
    if (mkfifo("fifo.polling", O_RDWR | 0666) < 0) {
        if (errno == EEXIST)
            printf("Verwende vorhandene FIFO\n");
        else {
            perror("mkfifo()");
            exit(EXIT_FAILURE);
        }
    }
    
    r_fd = open("fifo.polling", O_RDONLY);
    if (r_fd == -1) {
        perror("open(1)");
        exit(EXIT_FAILURE);
    }

    
    while (1) {
        if (read(r_fd, puffer, BUF) != 0) {
            an_den_drucker(puffer);
            n = 0;
            i = 0;
            
            while (puffer[n] != '\n') {
                pid[i++] = puffer[n++];
            }
            pid[++i] = '\n';
            strcpy(antwort, "fifo.");
            strncat(antwort, pid, i);
            w_fd = open(antwort, O_WRONLY);
            if (w_fd == -1) {
                perror("open(2)");
                exit(EXIT_FAILURE);
            }
            
            write (w_fd, TEXT, sizeof(TEXT));
            close(w_fd);
        }
        
        sleep(1);
    }
    
    return EXIT_SUCCESS;
}

