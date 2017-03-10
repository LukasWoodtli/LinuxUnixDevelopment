#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255

int main(void) {
    char name[MAX];
    printf("Umgebungsvariable: ");
    scanf("%254s", name);
    printf("Wert        : %s\n", getenv(name));
    
    unsetenv(name);
    printf("Neuer Wert (%s): %s\n", name, getenv(name));
    return EXIT_SUCCESS;
}
