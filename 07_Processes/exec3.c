#include <stdio.h>
#include <unistd.h>

int main(void) {
	char *arguments[4] = {"ls", "-l", "/usr/bin", NULL};
	return execv("/bin/ls", arguments);
}
