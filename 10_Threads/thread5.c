#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 2
#define BUF 255
#define COUNTER (10 * 1000 * 1000)

/* global var: Race Condition! */
static FILE *fz;

static void open_file(const char* file) {
    fz = fopen(file, "w+");
    if (fz == NULL) {
        printf("Konnte Datei %s nicht öffnen\n", file);
        exit(EXIT_FAILURE);
    }
}

static void thread_schreiben(void* name) {
    (void)name;
    char string[BUF];
    
    printf("Bitte Eingabe machen: ");
    fgets(string, BUF, stdin);
    fputs(string, fz);
    fflush(fz);
    for (int i=0; i<COUNTER; ++i);
    pthread_exit((void*)pthread_self());
}


static void thread_lesen(void* name) {
    (void)name;
    char string[BUF];
    
    rewind(fz);
    
    fgets(string, BUF, fz);
    
    printf("Ausgabe Thread %ld: ", (long)pthread_self());
    fputs(string, stdout);
    
    fflush(stdout);
    pthread_exit((void*)pthread_self());
}


int main(void) {
    int i;
    static int ret[MAX_THREADS];
    static pthread_t thread[MAX_THREADS];

    printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

    open_file("testfile");
    
    
    if (pthread_create(&thread[0], NULL, (void*)&thread_schreiben, NULL) != 0) {
        fprintf(stderr, "Konnte Thread nicht erzeugen ...!\n");
        exit(EXIT_FAILURE);
    }
    
    if (pthread_create(&thread[1], NULL, (void*)&thread_lesen, NULL) != 0) {
        fprintf(stderr, "Konnte Thread nicht erzeugen ...!\n");
        exit(EXIT_FAILURE);
    }
    
    
    
    
    for (i=0; i< MAX_THREADS; ++i) {
        pthread_join(thread[i], (void*)(&ret[i]));
    }
    
    for (i=0; i< MAX_THREADS; ++i) {
        printf("\t<-Thread %ld fertig\n", (long)thread[i]);
    }

    printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
    return EXIT_SUCCESS;
}

