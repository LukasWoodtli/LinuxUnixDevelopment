#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

static void child_status(int status) {
	if (WIFEXITED(status)) {
		printf("Kind normal beendet mit Return-Wert %d\n",
		       WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status)) {
		printf("Kind mit Signal-Nr beendet %d\n",
		       WTERMSIG(status));
#ifdef WCOREDUMP
		printf("Core-Dump %s\n",
		       WCOREDUMP(status) ? "yes" : "no");
#endif /* WCOREDUMP */

	}
	else if (WIFSTOPPED(status)) {
		printf("Kind wurde angehalten mit Signal-Nr %d\n",
		       WSTOPSIG(status));
		/* in der Regel SIGSTOP aber kann auch ptrace(), debugger... sein */
	}
}


int main(void) {
	pid_t pid;
	int status;
	int end;

	switch(pid=fork()) {
		case -1:
			perror("Fehler bei fork()");
			return EXIT_FAILURE;
		case 0:
			printf("--- Im Kindprozess ---\n");
			printf("Wie den Prozess beenden?\n");
			printf("- 1 - WIFEXITED (normal)\n");
			printf("- 2 - WIFSIGNALED (abnormal)\n");
			printf("Auswahl: ");
			
			scanf("%d", &end);
			if (end == 1)
				exit(66);
			else if (end == 2)
				abort(); /* entspricht kill(0, SIGABRT) */
			else
				printf("Was? ->%d<- ???\n", end);
			break;
		
		default:
			if (wait(&status) != pid) {
				perror("wait()");
				return EXIT_FAILURE;
			}
			child_status(status);
	}
	return EXIT_SUCCESS;
}
