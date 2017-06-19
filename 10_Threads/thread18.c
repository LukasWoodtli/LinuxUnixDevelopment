#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_THREADS 3 

void thread_once(void) {
	printf("Funktion thread_once aufgerufen\n");
}


void *thread_func(void* args) {
	(void)args;
	printf("Thread gestartet (ID:%ld)\n", (long)pthread_self());
	
	thread_once();
	
	printf("Thread fertig gestartet (ID:%ld)\n\n", (long)pthread_self());

	pthread_exit(NULL);
}


int main(void) {
	int i;


	pthread_t threads[MAX_THREADS];
	
	printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());
	
	for (i = 0; i < MAX_THREADS; ++i) 
		pthread_create(&threads[i], NULL, (void*)thread_func, NULL);

	for (i = 0; i < MAX_THREADS; ++i) 
		pthread_join(threads[i], NULL);
	
	printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());

	return EXIT_SUCCESS;
}

