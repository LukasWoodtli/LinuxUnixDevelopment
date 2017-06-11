#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>


static int werte[10];

sem_t sem;

static void *thread1(void* arg) {
	(void)arg;
	int i, val;

	printf("\t->Thread %ld gestartet ..\n", (long)pthread_self());

	/* critical section */
	for (i = 0; i < 10; ++i)
		werte[i] = i;
	/* critical section end */

	printf("->Thread %ld Inkrementiere Semaphor\n", (long)pthread_self());

	sem_post(&sem);
    sem_getvalue(&sem, &val);
    printf("->Semaphor inkrementiert (Wert: %d)\n", val);


	printf("\t->Thread %ld ende ..\n", (long)pthread_self());

	pthread_exit((void*)0);
}

static void *thread2(void* arg) {
	(void)arg;
	int i;
	int summe = 0;

    sem_wait(&sem);

	printf("\t->Thread %ld gestartet ..\n", (long)pthread_self());

	for (i=0; i < 10; ++i) {
		summe += werte[i];
	}

	printf("\t->Summe %d\n", summe);

    printf("\t->Thread %ld ende ..\n", (long)pthread_self());
	pthread_exit((void*)0);
}

int main(void) {
	static pthread_t th[2];
    int val;

	printf("-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

    sem_init(&sem, 0, 0);
    
    sem_getvalue(&sem, &val);
    printf("-> Semaphor initialisiert (Wert: %d)\n", val);
	
	pthread_create(&th[0], NULL, (void*)&thread1, NULL);
	pthread_create(&th[1], NULL, (void*)&thread2, NULL);

	
	pthread_join(th[0], NULL);
	pthread_join(th[1], NULL);
	
    sem_getvalue(&sem, &val);
    printf("-> Semaphor (Wert: %d)\n", val);
    
    sem_destroy(&sem);
    printf("-> Semaphor geloescht\n");
    
    
	printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
	return EXIT_SUCCESS;
}

