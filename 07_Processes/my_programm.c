#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int i;
    printf("Programmname : %s\n", argv[0]);
    for (i=1; i<argc; ++i) {
        printf("Argumente %d : %s\n", i, argv[i]);
    }
    return EXIT_SUCCESS;
}
