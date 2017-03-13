#include <stdio.h>
#include <unistd.h>

int main(void) {
	char *args[] = {"printenv", NULL};
	char *env[] = {
		"SHELL=/bin/bash",
		"LOGNAME=a_username",
		"OSTYPE=L1NuX", NULL
	};
	
	return execve("/usr/bin/printenv", args, env);
}