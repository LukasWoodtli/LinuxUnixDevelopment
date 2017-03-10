#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255

#define DONT_OVERWRITE 0
#define OVERWRITE      1

int main(void) {
    char name[MAX], neu_wert[MAX];
    printf("Umgebungsvariable: ");
    scanf("%254s", name);
    printf("Wert        : %s\n", getenv(name));
    
    printf("Wert aendern/setzen: ");
    scanf("%254s", neu_wert);
    
    if  (setenv(name, neu_wert, OVERWRITE) != 0) {
        printf("Konnte neuen Wert nicht setzen\n");
    }
    
    printf("Neuer Wert (%s): %s\n", name, getenv(name));
    return EXIT_SUCCESS;
}
