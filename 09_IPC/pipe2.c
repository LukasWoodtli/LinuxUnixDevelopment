#define _GNU_SOURCE 
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char** argv) {

	int fd[2];
	pid_t pid;
	char puffer[PIPE_BUF];
	FILE *reading, *writing, *newfile;
	
	
	if (argc != 2) {
		printf("Usage: %s Datei\n", *argv);
		exit(EXIT_FAILURE);
	}
	
	if ((newfile = fopen(argv[1], "a+")) == NULL) {
		perror("fopen()");
		exit(EXIT_FAILURE);
	}
	
	/* create pipe */
	if (pipe(fd) < 0) {
		perror("pipe()");
		exit(EXIT_FAILURE);
	}
	
	/* create new process */
	if ((pid = fork()) < 0) {
		perror("fork()");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0) { /* parent process */
		close(fd[0]); /* closing reading part of pipe */
		if ((writing = fdopen(fd[1], "w")) == NULL) {
			perror("fdopen()");
			exit(EXIT_FAILURE);
		}
		/* read bytes */
		fgets(puffer, PIPE_BUF, stdin);
		/* write to pipe */
		fputs(puffer, writing);
	} else { /* child */
		close(fd[1]); /* close writing part of pipe */
		if ((reading = fdopen(fd[0], "r")) == NULL) {
			perror("fdopen()");
			exit(EXIT_FAILURE);
		}
		
		/* read from pipe */
		fgets(puffer, PIPE_BUF, reading);
		fputs(puffer, newfile);
	}
	
	exit(EXIT_SUCCESS);
}
