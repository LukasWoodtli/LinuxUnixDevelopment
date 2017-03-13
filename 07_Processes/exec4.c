#include <stdio.h>
#include <unistd.h>

int main(void) {
	char *env[4];
	env[0] = "SHELL=/bin/bash";
	env[1] = "LOGNAME=heino";
	env[2] = "OSTYPE=LiNuX";
	env[3] = NULL;
	
	return execle("/bin/ls", "ls", "-d", "/proc", "/", NULL, env);
}

