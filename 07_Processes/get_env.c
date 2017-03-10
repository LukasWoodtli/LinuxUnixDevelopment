#include <stdio.h>
#include <stdlib.h>

#define MAX 255

int main(void) {
    char name[MAX];
    printf("Umgebungsvariable: ");
    scanf("%254s", name);
    printf("Wert        : %s\n", getenv(name));
    
    return EXIT_SUCCESS;
}
