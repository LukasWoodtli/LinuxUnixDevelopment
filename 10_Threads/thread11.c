#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


struct data {
    int werte[10];
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};


static void *thread1(void* arg) {
	struct data *d = (struct data*)arg;
	int ret, i;

	printf("\t->Thread %ld gestartet ..\n", (long)pthread_self());
	sleep(1);

	ret = pthread_mutex_lock(&d->mutex);
	if (ret != 0) {
		printf("Fehler bei lock in Thread:%ld\n", (long)pthread_self());
		exit(EXIT_FAILURE);
	}

	/* critical section */
	for (i = 0; i < 10; ++i)
		d->werte[i] = i;
	/* critical section end */

	printf("->Thread %ld Sende Bedingugsvariable\n", (long)pthread_self());

	pthread_cond_signal(&d->cond);
	ret = pthread_mutex_unlock(&d->mutex);
	if (ret != 0) {
		printf("Fehler bei unlock in Thread:%ld\n", (long)pthread_self());
		exit(EXIT_FAILURE);
	}

	printf("\t->Thread %ld ende ..\n", (long)pthread_self());

	pthread_exit((void*)0);
}

static void *thread2(void* arg) {
	struct data *d = (struct data*)arg;
    int i;
	int summe = 0;
	
	printf("->Thread %ld wartet auf Bedingugsvariable\n", (long)pthread_self());
	pthread_cond_wait(&d->cond, &d->mutex);
	
	printf("\t->Thread %ld gestartet ..\n", (long)pthread_self());
	
	for (i=0; i < 10; ++i) {
		summe += d->werte[i];
	}
	
	printf("\t->Thread %ld ende ..\n", (long)pthread_self());

	printf("\t->Summe %d\n", summe);

	pthread_exit((void*)0);	
}

int main(void) {
	static pthread_t th[2];
    struct data *d;
    
    d = (struct data*)malloc(sizeof(struct data));
    if (d == NULL) {
        printf("Konnte keinen Speicher allozieren...!\n");
        exit(EXIT_FAILURE);
    }
    
    pthread_mutex_init(&d->mutex, NULL);
    pthread_cond_init(&d->cond, NULL);
    
    printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

	pthread_create(&th[0], NULL, (void*)&thread1, d);
	pthread_create(&th[1], NULL, (void*)&thread2, d);

	pthread_join(th[0], NULL);
	pthread_join(th[1], NULL);
	
    /* delete cond var */
    pthread_cond_destroy(&d->cond);
    pthread_mutex_destroy(&d->mutex);
    
	printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
	return EXIT_SUCCESS;
}

