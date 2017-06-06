#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#define _DARWIN_C_SOURCE

#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define SHMDATASIZE 1024
#define BUFFERSIZE (SHMDATASIZE - sizeof(int))

#define SN_EMPTY 0
#define SN_FULL 1


#ifdef __linux__
/* need to define union semun ourselves (X/OPEN) */
union semun {
	int val;               /* value for SETVAL                 */
	struct semid_ds *buf;  /* buffer for IPC_STAT, IPC_SET     */
	unsigned short *array; /* array for GETALL, SETALL         */
	struct seminfo *__buf; /* buffer for IPC_INFO (Linux only) */
};
#else
/* union semun defined in `sys/sem.h` */
#endif

static int safesemop(int semid, struct sembuf *sops, unsigned nsops);

static void locksem(int semid, int semnum) {
	struct sembuf sb;
	sb.sem_num = semnum;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	safesemop(semid, &sb, 1);
}

static void unlocksem(int semid, int semnum) {
	struct sembuf sb;
	sb.sem_num = semnum;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;
	safesemop(semid, &sb, 1);
}

#if 0 /* unused in this example */
static void waitzero(int semid, int semnum) {
	struct sembuf sb;
	sb.sem_num = semnum;
	sb.sem_op = 0;
	sb.sem_flg = 0;
	safesemop(semid, &sb, 1);
}
#endif


static int safesemop(int semid, struct sembuf* sops, unsigned nsops) {
	int retval;
	retval = semop(semid, sops, nsops);
	if (retval == -1) {
		printf("Fehler: Semaphor mit ID %d, (%d Operation)\n", semid, nsops);
	}

	return retval;
}
