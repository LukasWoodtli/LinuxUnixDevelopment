#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255

const char *filterwords[] = {
    "undo", "oder", "aber", "ein", NULL
};


const char token[] = " ";

static void output(char* puffer) {
    int i = 0, len;
    static int merker = 0;
    char *p, tmp[20];
    
    /* split words */
    p = strtok(puffer, token);
    
    while (p != NULL) {
        i = 0;
        merker = 0;

        while (filterwords[i] != NULL) {
            /* compare words with filterwords */
            if (strstr(p, filterwords[i]) != NULL) {
                len = strlen(filterwords[i]);
                
                memset(tmp, 'x', len);
                tmp[len] = 0;
                
                fputs(tmp, stdout);
                fputs(" ", stdout);
                merker = 1;
            }
            ++i;
        }
        
        if (merker == 0) {
            fputs(p, stdout);
            fputs(" ", stdout);
        }
        
        p = strtok(NULL, token);
    }
}

int main() {
    char pufferin[MAX];
    while (fgets(pufferin, MAX, stdin) != NULL)
        output(pufferin);
    
    return EXIT_SUCCESS;
}
