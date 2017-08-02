#define _BSD_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static char *host_name = "244.0.0.1";
static int port = 1234;
static struct ip_mreq command;

static int setup_multicast_socket(void) {
    int loop = 1;
    int socket_descriptor;
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr =htonl(INADDR_ANY);
    sin.sin_port = htons(port);
    
    if ((socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    
    loop = 1;
    if (setsockopt(socket_descriptor,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &loop, sizeof(loop)) < 0) {
        perror("setsockopt()");
        exit(EXIT_FAILURE);
    }
    
    if (bind(socket_descriptor,
             (struct sockaddr*)&sin,
             sizeof(sin)) < 0) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }
    
    // allow broadcast on this machine
    
    loop = 1;
    if (setsockopt(socket_descriptor,
                   IPPROTO_IP,
                   IP_MULTICAST_LOOP,
                   &loop,
                   sizeof(loop)) < 0) {
        perror("bind(): IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);
    }


    // join broadcast group
    command.imr_multiaddr.s_addr = inet_addr(host_name);
    command.imr_interface.s_addr = htonl(INADDR_ANY);
    if (command.imr_multiaddr.s_addr == 0) {
        perror("244.0.0.1 ist keine Multicast-Adresse\n");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(socket_descriptor,
                   IPPROTO_IP,
                   IP_ADD_MEMBERSHIP,
                   &command, sizeof(command)) < 0) {
        perror("setsockopt():IP_ADD_MEMBERSHIP");
    }
    
    return socket_descriptor;
}

int main(void) {
    int iter = 0;
    socklen_t sin_len;
    char message[256];
    int socket;
    struct sockaddr_in sin;
    struct hostent *server_host_name;
    
    if ((server_host_name = gethostbyname(host_name)) == 0) {
        perror("gethostbyname()");
        exit(EXIT_FAILURE);
    }
    
    socket = setup_multicast_socket();
    
    while (iter++ < 10) {
        sin_len = sizeof(sin);
        if (recvfrom(socket, message, 256, 0,
                     (struct sockaddr*)&sin,
                     &sin_len) == -1) {
            perror("recvfrom()");
        }
        
        printf("Antwort #%-2d vom Server: %s\n", iter, message);
        sleep(2);
    }
    
    if (setsockopt(socket,
                   IPPROTO_IP,
                   IP_DROP_MEMBERSHIP,
                   &command, sizeof(command)) < 0) {
        perror("setsockopt():IP_DROP_MEMBERSHIP");
    }
    
    close(socket);
    
    return EXIT_SUCCESS;
}
