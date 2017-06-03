#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 5
#define BUF 255

static pthread_t thread[MAX_THREADS];

static void aktion(void* name) {
    (void)name;

    while(1) {
        if (pthread_equal(pthread_self(), thread[0])) {
            printf("\t->(%ld): Aufgabe \"abc\" Ausfuehren \n",
                   (long)pthread_self());
            break;
        }
        else if (pthread_equal(pthread_self(), thread[1])) {
            printf("\t->(%ld): Aufgabe \"def\" Ausfuehren \n",
                   (long)pthread_self());
            break;
        }
        else if (pthread_equal(pthread_self(), thread[2])) {
            printf("\t->(%ld): Aufgabe \"ghi\" Ausfuehren \n",
                   (long)pthread_self());
            break;
        }
        else {
            printf("\t->(%ld): Aufgabe \"xyz\" Ausfuehren \n",
                   (long)pthread_self());
            break;
        }
    }
    
    pthread_exit((void*)pthread_self());

}


int main(void) {
    
    int i;
    
    static int ret[MAX_THREADS];

    printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

    for (i=0; i < MAX_THREADS; ++i) {
        if (pthread_create(&thread[i], NULL, (void*)&aktion, NULL) != 0) {
            fprintf(stderr, "Konnte Thread nicht erzeugen ...!\n");
            exit(EXIT_FAILURE);
        
        }
    }
    
    for (i=0; i< MAX_THREADS; ++i) {
        pthread_join(thread[i], (void*)(&ret[i]));
    }
    
    for (i=0; i< MAX_THREADS; ++i) {
        printf("\t<-Thread %ld mit Arbeit fertig\n", (long)ret[i]);
    }

    printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
    return EXIT_SUCCESS;
}

