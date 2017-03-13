#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	pid_t pid;
	
	switch(pid=fork()) {
		case -1:
			printf("Fehler bei fork()");
			break;
		case 0:
			printf("--- Im Kindprozess (%d) ---\n", getpid());
			printf("Elternprozess: %d\n", getppid());
			
			sleep(2);
			
			printf("--- Im Kindprozess (%d) ---\n", getpid());
			printf("Elternprozess: %d\n", getppid());
			
			break;
		default:
			printf("--- Im Elternprozess (%d) ---\n", getpid());
			sleep(1);
			printf("--- Elternprozess endet vorzeitig ---\n");
			exit(0);
	}
	return EXIT_SUCCESS;
}
