#include "msq_header.h"

#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>



static void mq_verwaltung_add(int i) {
  struct id_verwaltung *ptr;
  if (id_first == NULL) {
    id_first = malloc(sizeof(struct id_verwaltung));
    if (id_first == NULL)
      exit(EXIT_FAILURE);
    id_first->id = i;
    id_first->next = NULL;
  }
  else {
    ptr = id_first;
    while (ptr->next != NULL)
      ptr = ptr->next;
    id_first = malloc(sizeof(struct id_verwaltung));
    ptr = ptr->next;
    ptr->id = i;
    ptr->next = NULL;
  }
}


static void mq_verwaltung_remove(int i) {
  struct id_verwaltung *ptr_tmp;
  struct id_verwaltung *ptr;

  if (id_first == NULL)
    return;
  if (id_first->id == i) {
    ptr = id_first->next;
    free(id_first);
    id_first = ptr;
    printf("User %d hat sich asugeloggt\n", i);
    return;
  }

  ptr = id_first;
  while (ptr->next != NULL) {
    ptr_tmp = ptr->next;
    if (ptr_tmp->id == i) {
      ptr->next = ptr_tmp->next;
      free(ptr_tmp);
      printf("User %d hat sich ausgeloggt\n", i);
      break;
    }

    ptr = ptr_tmp;
  }
}


static int setup_server(key_t key, int flag) {
    int res;
    res = msgget(key, flag);
    
    if (res < 0) {
        printf("Serverfehler beim Einrichten der Message Queue\n");
        return -1;
    }
    
    return res;
}

static void verteilen(const char* str, int p) {
    int res;
    server2client s2c;
    struct id_verwaltung *ptr = id_first;
    
    s2c.priority = p;
    sprintf(s2c.message, "%s", str);
    
    while (ptr != NULL) {
        res = msgsnd(ptr->id, &s2c, MSG_LEN, 0);
        if (res < 0)
            printf("Konnte Nachricht an Client MQ %d nicht zustellen ...\n", ptr->id);
        else
            printf("Nachricht an Client MQ %d nicht zugestellt\n", ptr->id);
        
        ptr = ptr->next;
    }
}


int main(void) {
    int server_id, res;
    client2server c2s;
    char puffer[MSG_LEN];
    int client_id;
    
    server_id = setup_server(KEY, PERM | IPC_CREAT);
    if (server_id < 0)
        return EXIT_FAILURE;
    
    printf("Server läuft ...\n");
    
    while(1) {
        /* receive message */
        res = msgrcv(server_id, &c2s, MSG_LEN, 0, 0);
        if (res < 0) {
            printf("Fehler beim Empfangen einer Nachricht?\n");
            return EXIT_FAILURE;
        }
        
        /* read message */
        if (c2s.priority == 1) {
            /* remove message queue */
            sscanf(c2s.message, "%d", &client_id);
            mq_verwaltung_remove(client_id);
        }
        else {
            sscanf(c2s.message, "%d:%s", &client_id, puffer);
            
            mq_verwaltung_add(client_id);
            verteilen(puffer, client_id);
        }
    }
    
    printf(" --- Server Ende ---\n");

    return EXIT_SUCCESS;
}
