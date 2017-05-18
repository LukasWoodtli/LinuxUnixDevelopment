#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define BUF 4096

static void an_den_drucker(const char *text) {
	FILE *p;
	
	p = popen ("lpr", "w");
	
	if (p == NULL) {
		perror("popen()");
		exit(EXIT_FAILURE);
	}
	
	printf("Sende Auftrag an den Drucker\n");
	// uncomment this to send text to printer
	// fprintf(p, "%s", text);
	printf("%s", text);
	pclose(p);
}

int main(void) {
	char puffer[BUF];
	int fd;
	
	umask(0);
	if (mkfifo("fifo.polling", 0666) < 0) {
		if (errno == EEXIST)
			printf("Versuche vorhandene FIFO zu verwenden.\n");
		else {
			perror("mkfifo()");
			exit(EXIT_FAILURE);
		}
	}
	
	fd = open("fifo.polling", O_RDONLY);
	if (fd == -1) {
		perror("open()");
		exit(EXIT_FAILURE);
	}
	
	while(1) {
		if (read(fd, puffer, BUF))
			an_den_drucker(puffer);
		sleep(2);
	}
	
	return EXIT_SUCCESS;
}

