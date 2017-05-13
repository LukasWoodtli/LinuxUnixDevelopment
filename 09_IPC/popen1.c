#define _GNU_SOURCE
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>

#define EXIT(s) { fprintf(stderr, "%s", s); exit(EXIT_FAILURE); }

#define MAX PIPE_BUF * 8

enum { ERROR = -1, SUCCESS = 0 };


int main(int argc, char **argv) {
    FILE *pipe_writer, *file;
    char puffer[MAX];
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s DateiZumLesen\n", *argv);
        exit(EXIT_FAILURE);
    }
    
    if ((file = fopen(argv[1], "r")) == NULL)
        EXIT("Fehler bei fopen() ...\n");
    if ((pipe_writer = popen("./filter", "w")) == NULL)
        EXIT("Fehler bei popen ...\n");
    while (fgets(puffer, MAX, file) != NULL) {
        if (fputs(puffer, pipe_writer) == EOF) {
            EXIT("Fehler bie fputs() ...\n")
        }
    }

    pclose(pipe_writer);
    return EXIT_SUCCESS;
}

