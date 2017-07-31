#define _BSD_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define BUF 1024

int main(void) {
    int sv[2], c;
    char buf1[BUF + 1], buf2[BUF + 1];
    
    if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sv) == -1) {
        perror("Fehler bei socketpair");
        exit(EXIT_FAILURE);
    }
    
    printf("Eingabe machen: ");
    fgets(buf1, BUF, stdin);
    
    write(sv[0], buf1, sizeof(buf1));
    
    c = read(sv[1], buf2, sizeof(buf2));
    buf2[2] = '\n';
    
    printf("sv[0] --> sv[1] : %s\n", buf2);
    
    printf("Eingabe machen: ");
    fgets(buf1, BUF, stdin);
    
    write(sv[1], buf1, sizeof(buf1));
    
    c = read(sv[0], buf2, sizeof(buf2));
    buf2[c] = '\0';
    
    printf("sv[1] --> sv[0] : %s\n", buf2);
    
    close(sv[0]);
    close(sv[1]);
    
    return EXIT_SUCCESS;
}
