#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 3
#define BUF 255

struct data {
    int wert;
    char msg[BUF];
};

static void exit_handler_mem(void * arg) {
    printf("\tExit-Handler aufgerufen ...\n");
    struct data *mem = (struct data*)arg;
    
    free(mem);
    printf("\tSpeicher freigegeben\n");
}

static void mythread(void* arg) {
    struct data *f = (struct data *)arg;
    
    pthread_cleanup_push(exit_handler_mem, (void*)f);
    
    printf("\t-> Thread mit ID:%ld gestartet\n",
           (long)pthread_self());
    printf("\tDaten empfangen: \n");
    printf("\t\twert = \"%d\"\n", f->wert);
    printf("\t\tmsg = \"%s\"\n", f->msg);
    
    pthread_cleanup_pop(1);
    
    pthread_exit((void*)pthread_self());
}


int main(void) {
    pthread_t thread[MAX_THREADS];
    struct data *f;
    int i;
    
    static int ret[MAX_THREADS];
    
    time_t t;
    
    srand((unsigned) time(&t));
    
    printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());
    
    for (i=0; i < MAX_THREADS; ++i) {
        f = (struct data*)malloc(sizeof(struct data));
        if (f == NULL) {
            printf("Konnte keinen Speicher reservieren ...!\n");
            exit(EXIT_FAILURE);
        }
        
        f->wert = 1 + (int)(10.0 * rand()/(RAND_MAX + 1.0));
        snprintf(f->msg, BUF, "Ich bin Thread Nr. %d", i + 1);
        
        if (pthread_create(&thread[i], NULL, (void*)&mythread, f) != 0) {
            fprintf(stderr, "Konnte Thread nicht erzeugen ...!\n");
            exit(EXIT_FAILURE);
        
        }
    }
    
    for (i=0; i< MAX_THREADS; ++i)
        pthread_join(thread[i], (void*)&ret[i]);
    
    for (i=0; i < MAX_THREADS; ++i)
        printf("<-Thread %ld ist fertig\n", (long)ret[i]);
    
    printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
    return EXIT_SUCCESS;
}

