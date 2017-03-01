#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char **argv)
{
	struct stat attribut;
	
	if (argc < 2)
		printf("usage: %s datei(en)\n", *argv);
	while (*++argv != NULL) {
		if (stat(*argv, &attribut) < 0) {
			fprintf(stderr, "stat(): %s\n", strerror(errno));
			return EXIT_FAILURE;
		}

		printf("%-18s\n", *argv);
		printf("USER-ID  : %d\n", attribut.st_uid);
		printf("GROUP-ID : %d\n", attribut.st_gid);
	}

	return EXIT_SUCCESS;
}

