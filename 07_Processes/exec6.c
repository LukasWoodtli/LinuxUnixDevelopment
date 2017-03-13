#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	char *args[4];
	args[0] = "ls";
	args[1] = "-l";
	args[2] = getenv("HOME");
	args[3] = NULL;
	
	return execvp("/bin/ls", args);
}

