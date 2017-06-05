#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define BUF 255


struct data {
    FILE *fz;
    char filename[BUF];
    pthread_mutex_t mutex;
};


static void thread_schreiben(void* arg) {
    char string[BUF];
    struct data *d=(struct data*)arg;
    
    printf("Bitte Eingabe machen: ");
    fgets(string, BUF, stdin);
    fputs(string, d->fz);
    fflush(d->fz);
    
    pthread_mutex_unlock(&d->mutex);
    
    pthread_exit((void*)pthread_self());
}


static void thread_lesen(void* arg) {
    char string[BUF];
    struct data *d=(struct data*)arg;
    
    while ((pthread_mutex_trylock(&d->mutex)) == EBUSY) {
        sleep(3);
        printf("Lese Thread wartet auf Arbeit ...\n");
        fflush(stdout);
    }
    
    rewind(d->fz);
    fgets(string, BUF, d->fz);
    
    printf("Ausgabe Thread %ld: ", (long)pthread_self());
    fputs(string, stdout);
    
    fflush(stdout);
    
    pthread_mutex_unlock(&d->mutex);
    
    pthread_exit((void*)pthread_self());
}


int main(void) {
    static int ret1, ret2;
    static pthread_t thread1, thread2;
    struct data *d;
    
    d = malloc(sizeof(struct data));
    if (d == NULL) {
        printf("Konnte keinen Speicher reservieren ...!\n");
        exit(EXIT_FAILURE);
    }
    
    printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

    strncpy(d->filename, "filename", BUF);
    d->fz = fopen(d->filename, "w+");
    if (d->fz == NULL) {
        printf("Konnte Datei %s nicht oeffnen\n", d->filename);
        exit(EXIT_FAILURE);
    }
    
    pthread_mutex_init(&d->mutex, NULL);
    
    pthread_mutex_lock(&d->mutex);
    
    
    if (pthread_create(&thread1, NULL, (void*)&thread_schreiben, d) != 0) {
        fprintf(stderr, "Konnte Thread nicht erzeugen ...!\n");
        exit(EXIT_FAILURE);
    }
    
    if (pthread_create(&thread2, NULL, (void*)&thread_lesen, d) != 0) {
        fprintf(stderr, "Konnte Thread nicht erzeugen ...!\n");
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1, (void*)(&ret1));
    pthread_join(thread2, (void*)(&ret2));

    
    pthread_mutex_destroy(&d->mutex);

    printf("\t<-Thread %ld fertig\n", (long)thread1);
    printf("\t<-Thread %ld fertig\n", (long)thread2);

    
    printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
    return EXIT_SUCCESS;
}

