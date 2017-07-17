#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX_ZEICHEN 1024
#define PORT_NUMMER 1234

int main(int argc, char** argv) {
	int sockfd, i, n, fd;
	ssize_t name_len;
	struct sockaddr_in adresse;
	struct in_addr inadr;
	struct hostent *rechner;

	char puffer[MAX_ZEICHEN];
	char* help;

	if (argc < 3) {
		printf("Usage: %s rechner datei(en)\n", *argv);
		exit(EXIT_FAILURE);
	}
    
	if (inet_aton(argv[1], &inadr))
		rechner = gethostbyaddr((const void*)&inadr,
				    sizeof(inadr),
				    AF_INET);
	else
		rechner = gethostbyname(argv[1]);


	if (rechner == NULL) {
		herror("Fehler beim Suchen des Rechners\n");
		exit(EXIT_FAILURE);
	}

	adresse.sin_family = AF_INET;
	adresse.sin_port = htons(PORT_NUMMER);
	memcpy(&adresse.sin_addr, rechner->h_addr_list[0], sizeof(adresse.sin_addr));
	
	for (i = 2; i < argc; ++i) {
		if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
			printf("Fehler bei socket() ... (%s)\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		
		if (connect(sockfd, (struct sockaddr*)&adresse, sizeof(adresse))) {
			printf("Fehler bei connect() ... (%s)\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		
		// open file for copying
		if ((fd = open(argv[i], O_RDONLY)) < 0) {
			printf("Kann Datei (%s) nicht oeffnen ... (%s)\n", argv[i], strerror(errno));
			continue;
		}
		
		strcpy(puffer, argv[i]);
		strcat(puffer, "\n");
		
		
		help = strrchr(puffer, '/');
		if (help == NULL)
			help = puffer;
		else
			help++;
		name_len = strlen(help);
		
		if (write(sockfd, help, name_len) != name_len) {
			printf("Konnte \"%s\" nicht versenden?\n", argv[i]);
			close(fd);
			close(sockfd);
			continue;
		}
		
		// send content to server
		while ((n = read(fd, puffer, sizeof(puffer))) > 0) {
			puffer[n] = 0; // important for binary files!
			if (write(sockfd, puffer, n) != n) {
				printf("Fehler bei write() ... (%s)\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		
		if (n < 0) {
			printf("Fehler bei read() ... (%s)\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		
		close(sockfd);
		sleep(2);
	}
	
	printf("Datei(en) erfolgreich versendet\n");

	return EXIT_SUCCESS;
}
