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

static int DeleteSemid = 0;
static int DeleteShmid = 0;

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

static int safesemget(key_t key, int nsems, int semflg) {
	int retval;
	retval = semget(key, nsems, semflg);
	if (retval == -1) {
		printf("Semaphor-Schluessel %d, nsems %d konnte nicht erstellt werden\n",
			key, nsems);
	}

	return retval;
}

static int safesemctl(int semid, int semnum, int cmd, union semun arg) {
	int retval;
	retval = semctl(semid, semnum, cmd, arg);
	if (retval == -1) {
		printf("Fehler: Semaphor mit ID %d, semnum %d, Kommando %d\n",
			semid, semnum, cmd);
	}

	return retval;
}

static int safesemop(int semid, struct sembuf* sops, unsigned nsops) {
	int retval;
	retval = semop(semid, sops, nsops);
	if (retval == -1) {
		printf("Fehler: Semaphor mit ID %d, (%d Operation)\n", semid, nsops);
	}
	
	return retval;
}


typedef void (*signalhandler_t)(int);

static signalhandler_t my_signal(int sig_nr, signalhandler_t signalhandler) {
	struct sigaction neu_sig, alt_sig;
	neu_sig.sa_handler = signalhandler;
	sigemptyset(&neu_sig.sa_mask);
	neu_sig.sa_flags = SA_RESTART;
	if (sigaction(sig_nr, &neu_sig, &alt_sig) < 0)
		return SIG_ERR;
	return alt_sig.sa_handler;
}
