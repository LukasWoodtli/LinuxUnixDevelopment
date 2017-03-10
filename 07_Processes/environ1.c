#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main(void) {
    int i = 0;
    while(environ[i] != NULL) {
        printf("%s\n", environ[i++]);
    }
    return EXIT_SUCCESS;
}
