#include <stdlib.h>
#include <stdio.h>

int main(void) {
    printf("Vor system()-Aufruf\n");
    system("find /usr/include -name 'std*'");
    printf("Nach system()-Aufruf\n");

    return EXIT_SUCCESS;
}
