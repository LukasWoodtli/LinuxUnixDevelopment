#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF 8192

static void an_den_drucker(const char *text) {
	FILE *p;

	p = popen("lpr", "w");
	if (p == NULL) {
		perror("popen()");
		exit(EXIT_FAILURE);
	}

	/* send to printer */
	printf("Sende Auftrag an Drucker...\n");
	fprintf(p, "%s", text);
	fflush(p);
	pclose(p);
}


int main(void) {
	char puffer[BUF], inhalt[BUF];
	inhalt[0] = '\0';

	printf("Bitte Text zum  Drucken schreiben (Beenden mit CTR+D)!\n->");
	while (fgets(puffer, BUF, stdin) != NULL) {
		if (strlen(inhalt) + strlen(puffer) < BUF) {
			strcat(inhalt, puffer);
			printf("->");
		}
		else {
			break;
		}
	}

	inhalt[BUF - 1] = '\0';
	an_den_drucker(inhalt);
	return EXIT_SUCCESS;
}