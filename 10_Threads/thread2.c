#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 3
#define BUF 255

struct data {
    int wert;
    char msg[BUF];
};

static void* mythread(void* arg) {
    struct data *f = (struct data *)arg;
    
    f->wert = 1 + (int)(10.0 * rand()/(RAND_MAX + 1.0));
    
    snprintf(f->msg, BUF, "Ich bin der Thread mit ID:%ld",
           (long)pthread_self());
    
    return arg;
}


int main(void) {
    pthread_t thread[MAX_THREADS];
    int i;
    
    static struct data* ret[MAX_THREADS];
    
    time_t t;
    srand((unsigned) time(&t));
    
    printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());
    
    for (i=0; i < MAX_THREADS; ++i) {
        ret[i] = (struct data*)malloc(sizeof(struct data));
        if (ret[i] == NULL) {
            printf("Konnte keinen Speicher reservieren ...!\n");
            exit(EXIT_FAILURE);
        }
    }

    for (i=0; i < MAX_THREADS; ++i) {
        if (pthread_create(&thread[i], NULL, (void*)&mythread, ret[i]) != 0) {
            fprintf(stderr, "Konnte Thread nicht erzeugen ...!\n");
            exit(EXIT_FAILURE);
        
        }
    }
    
    for (i=0; i< MAX_THREADS; ++i) {
        pthread_join(thread[i], (void*)(&ret[i]));
    }
    
    for (i=0; i< MAX_THREADS; ++i) {
        printf("Main-Thread: Daten empfangen:\n");
        printf("\t\twert = \"%d\"\n", ret[i]->wert);
        printf("\t\tmsg = \"%s\"\n", ret[i]->msg);
        
    }

    printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
    return EXIT_SUCCESS;
}

