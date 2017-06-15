#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

pthread_t t1, t2, t3;
static int zufallszahl;


static void cancel_test1(void* arg) {
	(void)arg;
	
	/* pseudo sync */
	sleep(1);
	
	if (zufallszahl > 25) {
		pthread_cancel(t3);
		printf("\t->Thread %ld beendet thread %ld\n",
		       (long)pthread_self(), (long)t3);
	}
	
	printf("\t->Thread %ld zu Ende\n",
	       (long)pthread_self());
	pthread_exit((void*)0);
}

static void cancel_test2(void* arg) {
	(void)arg;
	
	/* pseudo sync */
	sleep(1);
	
	if (zufallszahl <= 25) {
		pthread_cancel(t2);
		printf("\t->Thread %ld beendet thread %ld\n",
		       (long)pthread_self(), (long)t2);
	}
	
	printf("\t->Thread %ld zu Ende\n",
	       (long)pthread_self());

	pthread_exit((void*)0);

}


static void zufall(void* arg) {
	(void)arg;
	srand(time(NULL));
	zufallszahl = rand() % 50;
	pthread_exit((void*)0);
}


int main(void) {

	printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

	//pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	pthread_create(&t1, NULL, (void*)zufall, NULL);
	pthread_create(&t2, NULL, (void*)cancel_test1, NULL);
	pthread_create(&t3, NULL, (void*)cancel_test2, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	
	printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
	return EXIT_SUCCESS;
}

