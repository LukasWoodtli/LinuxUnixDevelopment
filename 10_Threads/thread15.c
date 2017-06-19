#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


static void cancel_test(void* arg) {
	(void)arg;
	int oldstate;

	if ((pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,
					&oldstate)) !=0) {
		printf("Fehler bei pthread_setcancelstate()\n");
		exit(EXIT_FAILURE);
	}

	printf("Thread %ld im kritischen Abschnitt\n", (long)pthread_self());

	sleep(5);
	

	if ((pthread_setcancelstate(oldstate, NULL)) !=0) {
		printf("Fehler bei pthread_setcancelstate()\n");
		exit(EXIT_FAILURE);
	}

	printf("\t->Thread %ld nach kritischem Abschnitt\n",
		(long)pthread_self());

	pthread_exit((void*)0);
}


int main(void) {

	pthread_t t1;
	long abbruch;

	printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

	if (pthread_create(&t1, NULL, (void*)cancel_test, NULL) != 0) {
		printf("Fehler bei pthread_setcancelstate()\n");
		exit(EXIT_FAILURE);
	}

	pthread_cancel(t1);

	pthread_join(t1, (void*)&abbruch);

	if (abbruch == (long)PTHREAD_CANCELED) {
		printf("Fehler bei pthread_setcancelstate()\n");
		exit(EXIT_FAILURE);
	}

	
	printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());

	return EXIT_SUCCESS;
}

