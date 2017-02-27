#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>

int main(int argc, char** argv) {
	const char * ptr;
	struct utimbuf zeit;

	if (argc < 2) {
		printf("usage: %s datei\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	ptr = argv[1];
	zeit.actime = 0;
	zeit.modtime = 0;

	if (utime(ptr, &zeit) < 0) {
		perror("utime");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

