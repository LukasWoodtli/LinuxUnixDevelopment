#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef void (*signalhandler_t)(int);

/* Kontrolliert Beendigung der Hauptschleife */
static volatile sig_atomic_t sigint = 1;

static signalhandler_t my_signal(int sig_nr, signalhandler_t signalhandler) {
    struct sigaction neu_sig, alt_sig;
    
    neu_sig.sa_handler = signalhandler;
    sigemptyset(&neu_sig.sa_mask);
    neu_sig.sa_flags = SA_RESTART;
    
    if (sigaction(sig_nr, &neu_sig, &alt_sig) < 0)
        return SIG_ERR;
    return alt_sig.sa_handler;
}

/* signal handler */
static void catch_sigint(int sig_nr) {
    --sigint;
    /* nicht umbeding nötig, handler bleibt eingerichtet */
    my_signal(sig_nr, catch_sigint);
}

int main(void){
    int i = 90000000, j, k;
    my_signal(SIGINT, catch_sigint);
    
    /* kein ping ohne pong ausgeben (auch bei SIGINT) */
    
    while (sigint) {
        j = i;
        k = i;
        while(--j);
        while(--k);
        printf("Ping-");
        
        j = i;
        k = i;
        while(--j);
        while(--k);
        printf("Pong\n");
    }

    return EXIT_SUCCESS;
}

