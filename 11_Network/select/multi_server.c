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
#include <unistd.h>
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


void bind_socket(socket_t *sock, unsigned long adress, unsigned short port) {
    struct sockaddr_in server;

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(adress);
    server.sin_port = htons(port);
    
    if (bind(*sock, (struct sockaddr*)&server, sizeof(server)) < 0)
        error_exit("Kann Socket nicht \"binden\"");
}


void listen_socket(socket_t *sock) {
    if (listen(*sock, 5) == -1)
        error_exit("Fehler bei listen()");
}


void accept_socket(socket_t *socket, socket_t *new_socket) {
    struct sockaddr_in client;
    socklen_t len;
    
    len = (socklen_t)sizeof(client);
    *new_socket = accept(*socket, (struct sockaddr*)&client, &len);
    
    if (*new_socket == -1)
        error_exit("Fehler bei accept()");
}


void TCP_recv(socket_t *sock, char *data, size_t size) {
    int len;
    len = recv(*sock, data, size, 0);
    
    if (len > 0 || len != -1)
        data[len] = '\0';
    else
        error_exit("Fehler bei recv()");
}

void close_socket(socket_t *sock) {
    close(*sock);
}



int main(void) {
    socket_t sock1, sock2, sock3;
    int i, ready, sock_max, max = -1;
    int client_sock[FD_SETSIZE];
    fd_set gesamt_sock, lese_sock;
    char *buffer = (char*)malloc(BUF);
    sock_max = sock1 = create_socket(AF_INET, SOCK_STREAM, 0);
    bind_socket(&sock1, INADDR_ANY, 1500);
    listen_socket(&sock1);
    
    for (i = 0; i < FD_SETSIZE; ++i) {
        client_sock[i] = -1;
    }
    
    FD_ZERO(&gesamt_sock);
    FD_SET(sock1, &gesamt_sock);
    
    for (;;) {
        lese_sock = gesamt_sock;
        ready = select(sock_max + 1, &lese_sock, NULL, NULL, NULL);
        
        if (FD_ISSET(sock1, &lese_sock)) {
            accept_socket(&sock1, &sock2);
            
            for (i = 0; i < FD_SETSIZE; ++i) {
                if (client_sock[i] < 0) {
                    client_sock[i] = sock2;
                    break;
                }
            }
            
            if (i == FD_SETSIZE)
                error_exit("Server ueberlaset -- zu viele Clients");
            
            FD_SET(sock2, &gesamt_sock);
            
            if (sock2 > sock_max)
                sock_max = sock2;
            
            if (i > max)
                max = i;
            
            if (--ready >= 0)
                continue;
        }
        
        
        for (i = 0; i <= max; ++i) {
            if ((sock3 = client_sock[i]) < 0)
                continue;
            
            if (FD_ISSET(sock3, &lese_sock)) {
                TCP_recv(&sock3, buffer, BUF - 1);
                printf("Nachricht empfangen: %s\n", buffer);
                
                if (strcmp(buffer, "quit\n") == 0) {
                    close_socket(&sock3);
                    FD_CLR(sock3, &gesamt_sock);
                    client_sock[i] = -1;
                    printf("Ein Client hat sich beendet\n");
                }
                
                if (--ready <= 0)
                    break; // no file descriptors left: exit
            }
        }
    }

    return EXIT_SUCCESS;
}
