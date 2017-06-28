#include <stdio.h>
#include <stdlib.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>

#define CAPS 4

static void clrscr(void) {
	char *ptr = tigetstr("clear");
	putp(ptr);
}

static void mvcursor(int zeile, int spalte) {
	char *cap = tigetstr("cup");
	putp(tparm(cap, zeile, spalte));
}

static void myflash(void) {
	char* flash = tigetstr("flash");
	putp(flash);
	putp(flash);
}

int main(void) {
	int i, ret = 0;
	char* buf;

	char *boolcaps[CAPS] = {"am", "bm", "bce", "km"};
	char *numcaps[CAPS] = {"cols", "lines", "colors", "pairs"};
	char *strcaps[CAPS] = {"cup", "kf2", "flash", "hpa"};

	ret = setupterm(NULL, STDIN_FILENO, NULL);
	if (ret != 0) {
		perror("setupterm()");
		exit(EXIT_FAILURE);
	}
	
	clrscr();

	for (i = 0; i < CAPS; ++i) {
		ret = tigetflag(boolcaps[i]);
		mvcursor(i + 2, 10);
		if (ret == -1)
			printf("%s wird nicht unterstuetzt\n", boolcaps[i]);
		else
			printf("%s wird unterstuetzt\n", boolcaps[i]);
	}
	
	mvcursor(10, 10);
	printf("Weiter mit ENTER\n");
	getchar();
	clrscr();
	
	for (i = 0; i < CAPS; ++i) {
		ret = tigetnum(numcaps[i]);
		mvcursor(i + 2, 10);
		if (ret == -1)
			printf("%s wird nicht unterstuetzt\n", numcaps[i]);
		else
			printf("%s ist %d\n", numcaps[i], ret);
	}

	mvcursor(10, 10);
	printf("Weiter mit ENTER\n");
	getchar();
	clrscr();
	
	for (i = 0; i < CAPS; ++i) {
		buf = tigetstr(strcaps[i]);
		mvcursor(i + 2, 10);
		if (buf == NULL)
			printf("%s wird nicht unterstuetzt\n", strcaps[i]);
		else
			printf("%s ist \\E%s\n", strcaps[i], &buf[1]);
	}

	mvcursor(10, 10);
	printf("Weiter mit ENTER\n");
	getchar();
	myflash();
	clrscr();

	return EXIT_SUCCESS;
}
