#define _BSD_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

int main(void) {
	struct servent *service_info = NULL;
    int port = 0;
    
    while (port++ < 100) {
        service_info = getservbyport(htons(port), "tcp");
        
        if (service_info == NULL)
            printf("Port %d : Keine Infos\n", port);
        else {
            printf("Service   : %10s ", service_info->s_name);
            printf("    Port    : %3d ", ntohs(service_info->s_port));
            printf("    Protokoll : %5s\n", service_info->s_proto);
        }
    }
	return EXIT_SUCCESS;
}
