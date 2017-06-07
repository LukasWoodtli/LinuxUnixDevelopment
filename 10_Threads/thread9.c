#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define THREAD_MAX 3


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void *threads(void* arg) {
	(void)arg;

	printf("\t->Thread %ld wartet auf Bedingung\n", (long)pthread_self());

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&cond, &mutex);

	printf("->Thread %ld hat Bedingung erhalten\n", (long)pthread_self());

	printf("->Thread %ld Sende Bedingugsvariable\n", (long)pthread_self());

	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	return NULL;
}


int main(void) {
	int i;
	static pthread_t th[THREAD_MAX];

	printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());


	for (i=0; i<THREAD_MAX; ++i) {
		if (pthread_create(&th[i], NULL, (void*)&threads, NULL) != 0) {
			fprintf(stderr, "Konnte Thread nicht erzeugen ...!\n");
			exit(EXIT_FAILURE);
		}
	}    

	printf("\n-> Main-Thread hat %d Threads erzeugt\n", THREAD_MAX);

    
	sleep(1);
	printf("->Main-Thread: Sende Bedingugsvariable\n");
	pthread_cond_signal(&cond);
	
	for (i=0; i<THREAD_MAX; ++i) {
		pthread_join(th[i], NULL);
	}
    
    printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
    return EXIT_SUCCESS;
}

