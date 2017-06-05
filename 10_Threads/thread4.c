#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 5
#define BUF 255

static void getprio(pthread_t id) {
    int policy;
    struct sched_param param;
    
    printf("\t->Thread %ld:\n", (long)id);
    if ((pthread_getschedparam(id, &policy, &param)) == 0) {
        printf("Zuteilung:\n");
        switch (policy) {
            case SCHED_OTHER:
                printf("SCHED_OTHER;\n");
                break;
            case SCHED_FIFO:
                printf("SCHED_FIFO;\n");
                break;
            case SCHED_RR:
                printf("SCHED_RR;\n");
                break;
            default:
                printf("Unbekannt;\n");
                break;
        }
        
        printf("Prioritaet: %d\n", param.sched_priority);
    }
}

static void setprio(pthread_t thread, int policy, int prio) {
    struct sched_param param;
    
    param.sched_priority = prio;
    if ((pthread_setschedparam(thread,
                                policy, &param)) != 0) {
        printf("Konnte Zuteilungsstrategie nicht aendern\n");
        pthread_exit((void*)thread);
    }
}

static void thread_prio_demo(void *name) {
    (void)name;

    getprio(pthread_self());
    
    /* only root can change prio */
    if (getuid() != 0) {
        printf("Veraendern kann nur Super-User\n");
        pthread_exit((void*)pthread_self());
    }

    setprio(pthread_self(), SCHED_RR, 2);
    getprio(pthread_self());
    pthread_exit((void*)pthread_self());
}




int main(void) {
    
    int i;
    static int ret[MAX_THREADS];
    static pthread_t thread[MAX_THREADS];

    printf("\n-> Main-Thread gestartet (ID:%ld)\n", (long)pthread_self());

    for (i=0; i < MAX_THREADS; ++i) {
        if (pthread_create(&thread[i], NULL, (void*)&thread_prio_demo, NULL) != 0) {
            fprintf(stderr, "Konnte Thread nicht erzeugen ...!\n");
            exit(EXIT_FAILURE);
        
        }
    }
    
    for (i=0; i< MAX_THREADS; ++i) {
        pthread_join(thread[i], (void*)(&ret[i]));
    }
    
    for (i=0; i< MAX_THREADS; ++i) {
        printf("\t<-Thread %ld mit Arbeit fertig\n", (long)ret[i]);
    }

    printf("<- Main-Thread beendet (ID:%ld)\n", (long)pthread_self());
    
    return EXIT_SUCCESS;
}

