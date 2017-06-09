#define _MULTI_THREADED
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUMTHREADS 2


static void checkResult(const char *string, int val) {
    if (val) {
        printf("Fehler mit %d bei %s", val, string);
        exit(EXIT_FAILURE);
    }
}


static pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t datenVorhandenCondition = PTHREAD_COND_INITIALIZER;
static int datenVorhanden = 0;
static int geteilteDaten = 0;


static void *theThread(void* param) {
    (void)param;
    int rc;
    int retries = 2;

	printf("\t->Client %ld: gestartet\n", (long)pthread_self());
    rc = pthread_mutex_lock(&dataMutex);
    checkResult("pthread_mutex_lock()\n", rc);
    
    
    while(--retries) {
        while (!datenVorhanden) {
            printf("\t->Client %ld: Warte auf Daten ...\n", (long)pthread_self());
            rc = pthread_cond_wait(&datenVorhandenCondition, &dataMutex);
            checkResult("pthread_cond_wait()\n", rc);
        }
    
        printf("\t->Client %ld: Daten wurden gemeldet --->\n"
               "\t----> Bearbeite Daten solange sie geschützt sind (lock)\n",
               (long)pthread_self());
    
        if (geteilteDaten == 0) {
            datenVorhanden = 0;
        }
    }
    
    printf("\t->Client %ld: Alles erledigt\n", (long)pthread_self());
    rc = pthread_mutex_unlock(&dataMutex);
    checkResult("pthread_mutex_unlock()\n", rc);

    return NULL;
}

int main(void) {
	static pthread_t thread[NUMTHREADS];
    int rc = 0;
    
    int anzahlDaten = 4;
    int i;
    
    printf("->Main Thread %ld gestartet ...\n", (long)pthread_self());
    
    for (i = 0; i < NUMTHREADS; ++i) {
        rc = pthread_create(&thread[i], NULL, theThread, NULL);
        checkResult("pthread_create()\n", rc);
    }
    
    
    
    while (anzahlDaten--) {
        sleep(3);
        printf("->Server: Daten gefunden\n");
        
        rc = pthread_mutex_lock(&dataMutex);
        checkResult("pthread_mutex_lock()\n", rc);
        printf("->Server: Sperre Daten und gib Meldung an Consumer\n");
        ++geteilteDaten;
        datenVorhanden = 1;
        
        rc = pthread_cond_signal(&datenVorhandenCondition);
        checkResult("pthread_cond_signal()\n", rc);
        
        printf("->Server: Gib gesperrte Daten wieder frei\n");
        rc = pthread_mutex_unlock(&dataMutex);
        checkResult("pthread_mutex_unlock()\n", rc);
    }
    
    for (i = 0; i < NUMTHREADS; ++i) {
        rc = pthread_join(thread[i], NULL);
        checkResult("pthread_join()\n", rc);
    }
	
    printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
	return EXIT_SUCCESS;
}
