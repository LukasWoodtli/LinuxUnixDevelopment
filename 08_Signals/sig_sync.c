#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define MAX 5

typedef void (*sighandler_t)(int);

static volatile sig_atomic_t flag = 0;

static sigset_t mask_neu, mask_alt, mask_null;

static void sig_func(int);

static sighandler_t my_signal(int sig_nr, sighandler_t sighandler) {
    struct sigaction neu_sig, alt_sig;
    neu_sig.sa_handler = sighandler;
    sigemptyset(&neu_sig.sa_mask);
    neu_sig.sa_flags = SA_RESTART;
    
    if (sigaction(sig_nr, &neu_sig, &alt_sig) < 0)
        return SIG_ERR;
    
    return alt_sig.sa_handler;
}


static void start_sync(void) {
    my_signal(SIGUSR1, sig_func);
    my_signal(SIGUSR2, sig_func);
    
    sigemptyset(&mask_null);
    sigemptyset(&mask_neu);

    sigaddset(&mask_neu, SIGUSR1);
    sigaddset(&mask_neu, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &mask_neu, &mask_alt) < 0) {
        printf("Fehler bei sigprocmask() ...\n");
        exit(EXIT_FAILURE);
    }
}

static void sig_func(int sig_nr) {
    (void)sig_nr;
    start_sync();
    flag = 1;
}

static void kind2eltern(pid_t pid) {
    kill(pid, SIGUSR1);
}

static void eltern2kind(pid_t pid) {
    kill(pid, SIGUSR2);
}


static void kind_wartet(void) {
    while (!flag) {
        sigsuspend(&mask_null);
    }
    flag = 0;
    
    if (sigprocmask(SIG_BLOCK, &mask_alt, NULL) < 0) {
        printf("Fehler bei sigprocmask() ...\n");
        exit(EXIT_FAILURE);
    }
}

static void eltern_warten(void) {
    while (!flag) {
        sigsuspend(&mask_null);
    }
    flag = 0;
    
    if (sigprocmask(SIG_BLOCK, &mask_alt, NULL) < 0) {
        printf("Fehler bei sigprocmask() ...\n");
        exit(EXIT_FAILURE);
    }
}


int main(void) {
    pid_t pid;
    int i, j;
    
    start_sync();
    
    if ((pid=fork()) < 0) {
        perror("fork()");
        return EXIT_FAILURE;
    }
    else if (pid == 0) {
        for (i = 0; i < MAX; ++i) {
            kind_wartet();
            printf("Kind \n");
            kind2eltern(getppid());
        }
        
        printf("--- Kind Ende ---\n");
        exit(EXIT_SUCCESS);
    }
    
    for (j = 0; j < MAX; ++j) {
        printf("Eltern \n");
        eltern2kind(pid);
        eltern_warten();
    }
    
    printf("-- Eltern Ende ---\n");
    return EXIT_SUCCESS;
}
