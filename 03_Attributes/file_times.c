#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

static int file_times(const char *name) {
	struct stat st;
	if (stat(name, &st) < 0) {
		fprintf(stderr, "Fehler bei stat: %s\n",
			strerror(errno));
		return -1;
	}
	
	printf("Letzter Zugriff           : %s",
	       ctime(&st.st_atime));
	printf("Letzte Aenderung (Inhalt) : %s",
	       ctime(&st.st_mtime));
	printf("Letzte Aenderung (inode)  : %s",
	       ctime(&st.st_ctime));
	return 1;
}

int main(int argc, char** argv) {
	const char * ptr;
	
	if (argc < 2) {
		printf("usage: %s datei\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	ptr = argv[1];
	
	if (file_times(ptr) < 0)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

