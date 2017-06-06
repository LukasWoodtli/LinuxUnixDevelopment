#include "shm_header.h"

static void delete(void) {
	int res;
	printf("\nServer wird beendet - Loesche Semaphor %d.\n", DeleteSemid);
	if (semctl(DeleteSemid, 0, IPC_RMID, 0) == -1) {
		printf("Fehler beim loeschen des Semaphors.\n");
	}
	
	/* delete segment automatically when server exits */
	res = shmctl(DeleteSemid, IPC_RMID, NULL);
	if (res == -1) {
		printf("Fehler bei shmctl() shmid %d, Kommando %d\n", DeleteSemid, IPC_RMID);
	}
}

static void sigdelete(int signum) {
	(void)signum;

	exit(EXIT_FAILURE);
}


static void server(void) {
	union semun sunion;
	int semid, shmid;
	void *shmdata;
	char* buffer;

	semid = safesemget(IPC_PRIVATE, 2, SHM_R | SHM_W);
	DeleteSemid = semid;
	atexit(&delete); /* delete semaphore at exit */

	my_signal(SIGINT, &sigdelete);

	/* init semaphore */
	sunion.val = 1;
	safesemctl(semid, SN_EMPTY, SETVAL, sunion);

	sunion.val = 0;
	safesemctl(semid, SN_FULL, SETVAL, sunion);

	/* setup shared-memory segment */
	shmid = shmget(IPC_PRIVATE, SHMDATASIZE, IPC_CREAT | SHM_R | SHM_W);
	if (shmid == -1)
		printf("Fehler bei key %d, mit Groesse %d\n", IPC_PRIVATE, SHMDATASIZE);

	DeleteShmid = shmid;

	shmdata = shmat(shmid, NULL, 0);
	if (shmdata == (void*) -1)
		printf("Fehler bei shmat(): shmid %d\n", shmid);
	
	/* write indicating key to beginning */
	*(int*)shmdata = semid;
	buffer = shmdata + sizeof(int);
	
	printf("Server laeuft mit Shared-Memory-ID %d\n", shmid);
	
	while (1) {
		printf("Warte ...");
		fflush(stdout);

		/* lock and wait */
		locksem(semid, SN_FULL);
		printf("\n... fertig.\n");
		
		/* client wrote something */
		printf("Nachricht erhalten: %s\n", buffer);
		
		unlocksem(semid, SN_EMPTY);
	}
}


int main(void) {
	server();
	printf("--- Server Ende ---\n");
	return EXIT_SUCCESS;
}
