#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_t tid2;


static void int_handler(int dummy) {
    (void)dummy;
    printf("SIGINT erhalten von thread: %ld\n", (long)pthread_self());
}

static void usr1_handler(int dummy) {
    (void)dummy;
    printf("SIGUSR1 erhalten von thread: %ld\n", (long)pthread_self());
}


void *thrad_1(void *dummy) {
    (void)dummy;
    int sig, status, *status_ptr = &status;
    sigset_t sigmask;

    // blocking no signal
    sigfillset(&sigmask);
    pthread_sigmask(SIG_UNBLOCK, &sigmask, (sigset_t*)0);
    sigwait(&sigmask, &sig);
    
    switch (sig) {
        case SIGINT: int_handler(sig); break;
        default: break;
    }
    
    // blocked by tid2
    printf("Thread %ld: sende SIGINT and %ld\n", (long)pthread_self(), (long)tid2);
    pthread_kill(tid2, SIGINT);
    
    // not blocked by tid2
    printf("Thread %ld: sende SIGUSR1 and %ld\n", (long)pthread_self(), (long)tid2);
    pthread_kill(tid2, SIGUSR1);

    pthread_join(tid2, (void**)status_ptr);
    printf("Thread %ld: Exit-Status = %d\n",(long)tid2, status);
    
    printf("Thread %ld: wird beendet\n", (long)pthread_self());
    pthread_exit(NULL);
    
}


void *thread_2(void* dummy) {
    (void)dummy;
    int sig;
    sigset_t sigmask;
    
    sigemptyset(&sigmask);
    // don't block SIGUSR1
    sigaddset(&sigmask, SIGUSR1);
    pthread_sigmask(SIG_UNBLOCK, &sigmask, (sigset_t*)0);
    sigwait(&sigmask, &sig);
    
    switch (sig) {
        case SIGUSR1: usr1_handler(sig); break;
        default: break;
    }
    
    printf("Thread %ld: wird beendet\n", (long)pthread_self());
    pthread_exit((void*)99);
}







int main(void) {
    pthread_t tid1;
    pthread_attr_t attr_obj;
    sigset_t sigmask;
    struct sigaction action;
    
	printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

    // block all signals in main thread
    sigfillset(&sigmask);
    pthread_sigmask(SIG_BLOCK, &sigmask, (sigset_t*)0);
    
    action.sa_flags = 0;
    action.sa_handler = int_handler;
    sigaction(SIGINT, &action, (struct sigaction*)0);
    action.sa_handler = usr1_handler;
    sigaction(SIGUSR1, &action, (struct sigaction*)0);
    
    pthread_attr_init(&attr_obj);
    pthread_attr_setdetachstate(&attr_obj, PTHREAD_CREATE_DETACHED);
    
    
	pthread_create(&tid1, &attr_obj, (void*)thrad_1, NULL);
    printf("Thread %ld erzeugt\n",(long)tid1);
    
    pthread_attr_setdetachstate(&attr_obj, PTHREAD_CREATE_JOINABLE);
    pthread_create(&tid2, &attr_obj, (void*)thread_2, NULL);
    printf("Thread %ld erzeugt\n",(long)tid2);
    
    sleep(1);
    
    printf("Haupt-Thread %ld sendet SIGINT and %ld\n", (long)pthread_self(),(long)tid2);
    pthread_kill(tid1, SIGINT);
    
    printf("Haupt-Thread %ld sendet SIGUSR and %ld\n", (long)pthread_self(),(long)tid2);
    pthread_kill(tid1, SIGUSR1);
    
	
	printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
    // doesn't end process
	pthread_exit(NULL);
}

