#include "shm_header.h"

static void clientwrite(int semid, char* buffer) {
    printf("Warte ...");
    fflush(stdout);
    
    /* lock: client wants to write */
    locksem(semid, SN_EMPTY);
    printf("\n... fertig\n");
    printf("Eingabe machen: ");
    fgets(buffer, BUFFERSIZE, stdin);
    unlocksem(semid, SN_FULL);
}

static void client(int shmid) {
    int semid;
    void *shmdata;
    char *buffer;
    
    /* shared-memory connection to server */
    shmdata = shmat(shmid, NULL, 0);
    if (shmdata == (void*) -1) {
        printf("Fehler bei shmat(): shmid %d\n", shmid);
    }
    
    semid = *(int*)shmdata;
    buffer = shmdata + sizeof(int);
    printf("Client: Shared-Memory-ID: %d, Semaphor-ID: %d\n",
           shmid, semid);
    
    while (1) {
        char input[3];
        
        printf("\n\nMenu\n1. Eine Nachricht verschicken\n");
        printf("2. Client-Ende\n");
        fgets(input, sizeof(input), stdin);
        
        switch (input[0]) {
            case '1':
                clientwrite(semid, buffer);
                break;
            case '2':
                exit(EXIT_FAILURE);
                break;
        }
    }
}

int main(int argc, char **argv) {
    
    if (argc < 2) {
        printf("Aufruf: %s Shared-Memory-ID\n", *argv);
    }
    else {
        client(atoi(argv[1]));
    }

	return EXIT_SUCCESS;
}
