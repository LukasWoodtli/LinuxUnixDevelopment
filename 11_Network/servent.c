#define _BSD_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

int main(int argc, char** argv) {
	struct servent *service_info = NULL;
	char **ptr;
	
	if (argc != 3) {
		printf("Usage: %s dienst protokoll\n", *argv);
		exit(EXIT_FAILURE);
	}
    
    service_info = getservbyname(argv[1], argv[2]);
    
    if (service_info == NULL) {
        herror("Konnte Service nicht finden\n");
        exit(EXIT_FAILURE);
    }
    
    
    printf("Service    : %s\n", service_info->s_name);
    printf("Port       : %d\n", ntohs(service_info->s_port));
    
    printf("Aliase     : \n");
    ptr = service_info->s_aliases;
    while (*ptr != NULL) {
        printf("%s\n", *ptr++);
    }
    
    printf("Protokoll  : %s\n", service_info->s_proto);
	
	return EXIT_SUCCESS;
}
