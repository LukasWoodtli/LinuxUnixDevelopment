#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

/* magic number */
#define KEY 1234L

#define MSG_LEN 1024

#define PERM 0666 // rights

#undef signal

typedef struct {
  long priority;
  char message[MSG_LEN];
} client2server;

typedef struct {
  long priority;
  char message[MSG_LEN];
} server2client;


struct id_verwaltung {
  int id;
  struct id_verwaltung *next;
};

struct id_verwaltung *id_first = NULL;



typedef void (sigfunk) (int);
sigfunk * signal (int sig_nr, sigfunk signalhandler) {
  struct sigaction neu_sig, alt_sig;
  neu_sig.sa_handler = signalhandler;
  sigemptyset(&neu_sig.sa_mask);
  neu_sig.sa_flags = SA_RESTART;
  if (sigaction(sig_nr, &neu_sig, &alt_sig) < 0)
    return SIG_ERR;

  return alt_sig.sa_handler;
}
