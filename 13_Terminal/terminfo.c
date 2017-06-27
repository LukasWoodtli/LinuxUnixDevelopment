#include <stdio.h>
#include <stdlib.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>

#define CAPS 4


int main(void) {
	int i, ret = 0;
	char* buf;

	const char *boolcaps[CAPS] = {"am", "bm", "bce", "km"};
	const char *numcaps[CAPS] = {"cols", "lines", "colors", "pairs"};
	const char *strcaps[CAPS] = {"cup", "kf2", "flash", "hpa"};

	ret = setupterm(NULL, STDIN_FILENO, NULL);
	if (ret != 0) {
		perror("setupterm()");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < CAPS; ++i) {
		ret = tigetflag(boolcaps[i]);
		if (ret == -1)
			printf("%s wird nicht unterstuetzt\n", boolcaps[i]);
		else
			printf("%s wird unterstuetzt\n", boolcaps[i]);
	}
	
	printf("\n");
	
	for (i = 0; i < CAPS; ++i) {
		ret = tigetnum(numcaps[i]);
		if (ret == -1)
			printf("%s wird nicht unterstuetzt\n", numcaps[i]);
		else
			printf("%s ist %d\n", numcaps[i], ret);
	}
	
	printf("\n");

	for (i = 0; i < CAPS; ++i) {
		buf = tigetstr(strcaps[i]);
		if (buf == NULL)
			printf("%s wird nicht unterstuetzt\n", strcaps[i]);
		else
			printf("%s ist \\E%s\n", strcaps[i], &buf[1]);
	}

	return EXIT_SUCCESS;
}
