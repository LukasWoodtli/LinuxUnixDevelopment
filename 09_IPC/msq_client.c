#include "msq_header.h"

#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>

static int ende = 1;

static int setup_client(key_t key, int flag) {
    int res = msgget(key, flag);
    if (res == -1) {
        printf("Client-Fehler beim Einrichten der Message queue!\n");
        return -1;
    }
    
    return res;
}


/* client wants to exit */
static void shutdown_msq(int signr) {
    (void)signr;
    ende = 0;
}


int main(void) {
    int server_id, client_id;
    int res;
    
    client2server c2s;
    server2client s2c;
    
    /* setup signal handler */
    signal(SIGINT, shutdown_msq);
    
    /* MQ for server */
    server_id = setup_client(KEY, 0);
    if (server_id < 0)
        return EXIT_FAILURE;
    
    /* MQ for client */
    client_id = setup_client(IPC_PRIVATE, PERM | IPC_CREAT);
    if (server_id < 0)
        return EXIT_FAILURE;


    c2s.priority = 2;
    sprintf(c2s.message, "%d:Login", client_id);
    res = msgsnd(server_id, &c2s, MSG_LEN, 0);
    if (res == -1) {
        printf("Konnte keine Nachricht versenden ...\n");
        return EXIT_FAILURE;
    }
    
    
    res = msgrcv(client_id, &s2c, MSG_LEN, 0, 0);
    if (res == -1) {
        printf("Fehler beim Erhalt der Nachricht ...\n");
        return EXIT_FAILURE;
    }
    
    printf("%ld: %s\n", s2c.priority, s2c.message);
    
    while (ende) {
        res = msgrcv(client_id, &s2c, MSG_LEN, 0, IPC_NOWAIT);
        if (res != -1) {
            printf("(%s) von User mit der Message Queue ID: %ld\n",
                   s2c.message, s2c.priority);
        }
        
        usleep(1000);
    }

    /* CTRL+C (SIGINT) */
    c2s.priority = 1;
    sprintf(c2s.message, "%d", client_id);
    res = msgsnd(server_id, &c2s, MSG_LEN, 0);
    if (res == -1) {
        printf("Konnte keine Nachricht versenden ...\n");
        return EXIT_FAILURE;
    }
    
    /* remove message queue */
    msgctl(client_id, IPC_RMID, NULL);
    
    return EXIT_SUCCESS;
}
