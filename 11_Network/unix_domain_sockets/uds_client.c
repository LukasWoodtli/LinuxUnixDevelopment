#define _BSD_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUF 1024
#define UDS_FILE "/tmp/sock.uds"

int main(void) {
    int create_socket;
    char buffer[BUF + 1];
    struct sockaddr_un address;
    int size;
    
    printf("\e[2J");
    
    if ((create_socket = socket(PF_LOCAL, SOCK_STREAM, 0)) > 0)
        printf("Socket wurde angelegt\n");
    
    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, UDS_FILE);
    
    if (connect(create_socket, (struct sockaddr*)&address, sizeof(address)) == 0)
        printf("Verbindung zum Server hergestellt\n");
    
    do {
        size = recv(create_socket, buffer, BUF-1, 0);
        if (size > 0)
            buffer[size] = '\0';
        printf("Nachricht erhalten:%s\n", buffer);
        if (strcmp(buffer, "quit\n")) {
            printf("Nachricht zum versenden: ");
            fgets(buffer, BUF, stdin);
            send(create_socket, buffer, strlen(buffer), 0);
        }
    } while (strcmp(buffer, "quit\n") != 0);
    
    close(create_socket);
    
    return EXIT_SUCCESS;
}
