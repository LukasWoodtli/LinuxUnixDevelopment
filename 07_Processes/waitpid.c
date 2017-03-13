#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(void) {
	pid_t pid;

	switch(pid=fork()) {
		case -1:
			perror("Fehler bei fork()");
			return EXIT_FAILURE;
		case 0:
			printf("--- Im Kindprozess ---\n");
			sleep(5);
			printf("--- Kindprozess ist fertig ---\n");
			break;
		
		default:
			if (waitpid(pid, NULL, WNOHANG) != 0) {
				perror("waitpid()");
				return EXIT_FAILURE;
			}
			printf("--- Elternprozess blockiert nicht ---\n");
	}
	return EXIT_SUCCESS;
}
