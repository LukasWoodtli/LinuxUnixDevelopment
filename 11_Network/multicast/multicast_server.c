#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int port = 1234;

int main(void) {
    int socket_descriptor;
    struct sockaddr_in address;
    
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_descriptor == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    
    memset(&address, 0, sizeof(address));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("224.0.0.1");
    address.sin_port = htons(port);
    
    printf("Server ist bereit ...\n");

    const char * BROADCAST_TXT = "broadcast test (hallo client)";
    while (1) {
        if (sendto(socket_descriptor,
                   BROADCAST_TXT,
                   strlen(BROADCAST_TXT),
                   0,
                   (struct sockaddr*)&address,
                   sizeof(address)) < 0) {
            perror("sendto()");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

    return EXIT_SUCCESS;
}
