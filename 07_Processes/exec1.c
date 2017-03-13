#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

#define MAX 255

int main(void) {
	char wd[MAX];
	wd[MAX - 1] = '\0';
	if (getcwd(wd, MAX - 1) == NULL)
		printf("Konnte Arbeitsverzeichnis nicht ermitteln!");
	else
		return execl("/bin/ls", "ls", "-l", wd, NULL);

}