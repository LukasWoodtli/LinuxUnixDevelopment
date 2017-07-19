#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


#define MAX_ZEICHEN 1024
#define PORT_NUMMER 1234



typedef void (*signalhandler_t)(int);

static signalhandler_t my_signal(int signr, signalhandler_t signalhandler) {
    struct sigaction neu_sig, alt_sig;
    neu_sig.sa_handler = signalhandler;
    sigemptyset(&neu_sig.sa_mask);
    neu_sig.sa_flags = SA_RESTART;
    
    if (sigaction(signr, &neu_sig, &alt_sig) < 0)
        return SIG_ERR;
    
    return alt_sig.sa_handler;
}

static void no_zombie(int signr) {
    (void)signr;
    pid_t pid;
    int ret;
    
    while ((pid = waitpid(-1, &ret, WNOHANG)) > 0)
        printf("Child-Server mit pid-%d hat sich beendet\n", pid);

}



int main(void) {
    int sockfd, connfd, fd, j, n, ngesammt;
    struct sockaddr_in adresse;
    socklen_t addrlaenge = sizeof(struct sockaddr_in);
    static const int y = 1;
    
    char puffer[MAX_ZEICHEN];
    char path_file[MAX_ZEICHEN];
    
    pid_t pid;
    
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Fehler bei socket() ... (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    printf("Socket erfolgreich angelegt\n");
    
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(PORT_NUMMER);
    memset(&adresse.sin_addr, 0, sizeof(adresse.sin_addr));

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
               &y, sizeof(int));
    
    if (bind(sockfd, (struct sockaddr*)&adresse, sizeof(adresse))) {
        printf("Fehler bei bind() ... (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    printf("Server bereit und wartet ...\n");
    
    if (listen(sockfd, 5) != 0) {
	    printf("Fehler bei listen() ... (%s)\n", strerror(errno));
	    exit(EXIT_FAILURE);
    }
    
    my_signal(SIGCHLD, no_zombie);

    while (1) {

        connfd = accept(sockfd, (struct sockaddr*)&adresse, &addrlaenge);
        
        if (connfd < 0) {
            if (errno == EINTR)
                continue;
            else {
                printf("Fehler bei accept() ... (%s)\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        
        
        printf("... Daten empfangen\n");
        
        if ((pid = fork()) == 0) {
            close(sockfd);
            j = 0;
        
            while ((n = read(connfd, &puffer[j], 1)) > 0) {
                if (puffer[j] == '\n') {
                    puffer[j] = 0;
                    break;
                }
                ++j;
            }
            
            if (n < 0) {
                printf("Fehler bei read() ...\n");
                exit(EXIT_FAILURE);
            }
        
            printf("Dateiname \"%s\" wird kopiert nach ", puffer);
            strcpy(path_file, getenv("HOME"));
            strcat(path_file, "/tmp/");
            strcat(path_file, puffer);
            printf("%s\n", path_file);
        
            if ((fd = open(path_file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
            printf("... kann %s nicht oeffnen (%s)\n", puffer, strerror(errno));
            close(connfd);
            continue;
            }
        
            ngesammt = 0;
        
            while ((n = read(connfd, puffer, sizeof(puffer))) > 0) {
                if (write(fd, puffer, n) != n) {
                    printf("Fehler bei write() ... (%s)\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            
                ngesammt += n;
            }
        
            if (n < 0) {
                printf("Fehler bei read() ...\n");
                exit(EXIT_FAILURE);
            }
        
            printf("Beendet (%d Bytes)\n", ngesammt);
            close(fd);
            close(connfd);
        } // end fork
    
        close(connfd);
    }
    
    close(sockfd);

    return EXIT_SUCCESS;
}
