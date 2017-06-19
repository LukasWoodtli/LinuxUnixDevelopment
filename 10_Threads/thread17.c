#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_THREADS 3 


static pthread_key_t tsd_key;


void thread_write(const char* text) {
	FILE* th_fp = (FILE*)pthread_getspecific(tsd_key);
	fprintf(th_fp, "%s\n", text);
}

void thread_close(void* th_fp) {
	fclose((FILE*)th_fp);
}

void* thread_tsd(void* args) {
	(void)args;
	char th_fpname[20];
	FILE* th_fp;
	
	sprintf(th_fpname, "thread%ld.thread", (long)pthread_self());
	
	th_fp = fopen(th_fpname, "w");
	if (th_fp == NULL)
		pthread_exit(NULL);
	
	pthread_setspecific(tsd_key, th_fp);

	thread_write("Thread wurde gestartet ...\n");
	
	// the thread would do here its work
	
	thread_write("Thread is fertig");
	pthread_exit(NULL);
}


int main(void) {
	int i;
	pthread_t threads[MAX_THREADS];
	
	printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

	pthread_key_create(&tsd_key, thread_close);
	
	
	for (i = 0; i < MAX_THREADS; ++i) 
		pthread_create(&threads[i], NULL, (void*)thread_tsd, NULL);

	for (i = 0; i < MAX_THREADS; ++i) 
		pthread_join(threads[i], NULL);
	
	printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());

	return EXIT_SUCCESS;
}

