#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


#define BUF 1024
#define socket_t int

void error_exit(char *error_message) {
    fprintf(stderr, "%s: %s\n", error_message, strerror(errno));
    exit(EXIT_FAILURE);
}


int create_socket(int af, int type, int protocol) {
    socket_t sock;
    const int y = 1;
    
    sock = socket(af, type, protocol);
    if (sock < 0)
        error_exit("Fehler beim Anlegen des Sockets");
    
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
    
    return sock;
}


void connect_socket(socket_t *sock, char *serv_addr, unsigned short port) {
    struct sockaddr_in server;
    struct hostent *host_info;
    unsigned long addr;

    memset(&server, 0, sizeof(server));
    
    if ((addr = inet_addr(serv_addr)) != INADDR_NONE) {
        memcpy((char*)&server.sin_addr, &addr, sizeof(addr));
    }
    else {
        host_info = gethostbyname(serv_addr);
        
        if (NULL == host_info)
            error_exit("Unknown Server");
        
        memcpy((char*)&server.sin_addr,
               host_info->h_addr,
               host_info->h_length);
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    
    if (connect(*sock, (struct sockaddr*)&server, sizeof(server)) < 0)
        error_exit("Kann Socket nicht \"binden\": keine Verbindung zum Server");
}


void TCP_send(socket_t *sock, char *data, size_t size) {
    if (send(*sock, data, size, 0) == -1) {
        error_exit("Fehler send()");
    }
}

void close_socket(socket_t *sock) {
    close(*sock);
}



int main(int argc, char** argv) {
    socket_t sock;
    char *buffer = (char*)malloc(BUF);
    
    if (argc < 2) {
        printf("Usage: %s serverAdress\n", *argv);
        exit(EXIT_FAILURE);
    }
    
    sock = create_socket(AF_INET, SOCK_STREAM, 0);
    connect_socket(&sock, argv[1], 15000);
    
    do {
        buffer[0] = '\0';
        printf("Nachricht zum versenden: ");
        fgets(buffer, BUF, stdin);
        TCP_send(&sock, buffer, strlen(buffer));
    } while (strcmp(buffer, "quit\n") != 0);
    
    close_socket(&sock);
    
    return EXIT_SUCCESS;
}
