#define _GNU_SOURCE 
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char** argv) {

	int fd[2], fd1, n;
	pid_t pid;
	char puffer[PIPE_BUF];
	
	if (argc != 2) {
		printf("Usage: %s Datei\n", *argv);
		exit(EXIT_FAILURE);
	}
	
	if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
		perror("open()");
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
		/* read file (PIPE_BUF bytes) */
		n = read(fd1, puffer, PIPE_BUF);
		/* write to pipe */
		if ((write(fd[1], puffer, n)) != n) {
			perror("write()");
			exit(EXIT_FAILURE);
		}
		if ((waitpid(pid, NULL, 0)) < 0) {
			perror("waitpid()");
			exit(EXIT_FAILURE);
		}
	}
	else { /* child */
		close(fd[1]); /* close writing part of pipe */
		/* read from pipe */
		n = read(fd[0], puffer, PIPE_BUF);
		/* write to stdout */
		if ((write(STDOUT_FILENO, puffer, n)) != n) {
			perror("write()");
			exit(EXIT_FAILURE);
		}
	}
	
	exit(EXIT_SUCCESS);
}
