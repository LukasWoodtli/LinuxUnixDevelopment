#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define THREAD_MAX 3

static int werte[10];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void *thread1(void* arg) {
	(void)arg;
	int ret, i;

	printf("\t->Thread %ld gestartet ..\n", (long)pthread_self());
	sleep(1);

	ret = pthread_mutex_lock(&mutex);
	if (ret != 0) {
		printf("Fehler bei lock in Thread:%ld\n", (long)pthread_self());
		exit(EXIT_FAILURE);
	}

	/* critical section */
	for (i = 0; i < 10; ++i)
		werte[i] = i;
	/* critical section end */

	printf("->Thread %ld Sende Bedingugsvariable\n", (long)pthread_self());

	pthread_cond_signal(&cond);
	ret = pthread_mutex_unlock(&mutex);
	if (ret != 0) {
		printf("Fehler bei unlock in Thread:%ld\n", (long)pthread_self());
		exit(EXIT_FAILURE);
	}

	printf("\t->Thread %ld ende ..\n", (long)pthread_self());

	pthread_exit((void*)0);
}

static void *thread2(void* arg) {
	(void)arg;
	int i;
	int summe = 0;
	
	printf("->Thread %ld wartet auf Bedingugsvariable\n", (long)pthread_self());
	pthread_cond_wait(&cond, &mutex);
	
	printf("\t->Thread %ld gestartet ..\n", (long)pthread_self());
	
	for (i=0; i < 10; ++i) {
		summe += werte[i];
	}
	
	printf("\t->Thread %ld ende ..\n", (long)pthread_self());

	printf("\t->Summe %d\n", summe);

	pthread_exit((void*)0);	
}

int main(void) {
	static pthread_t th[2];

	printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());


	
	pthread_create(&th[0], NULL, (void*)&thread1, NULL);
	pthread_create(&th[1], NULL, (void*)&thread2, NULL);

	
	pthread_join(th[0], NULL);
	pthread_join(th[1], NULL);
	
	printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
	return EXIT_SUCCESS;
}

