#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	return execlp("ls", "ls", "-l", getenv("HOME"), NULL);
}

