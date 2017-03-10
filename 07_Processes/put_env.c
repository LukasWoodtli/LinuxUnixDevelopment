#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255

static int put_env(const char *name, const char *wert) {
    static char buffer[MAX+MAX];
    strcpy(buffer, name);
    strcat(buffer, "=");
    strcat(buffer, wert);
    return putenv(strdup(buffer));
}

int main(void) {
    char name[MAX], neu_wert[MAX];
    printf("Umgebungsvariable: ");
    scanf("%254s", name);
    printf("Wert        : %s\n", getenv(name));
    
    printf("Wert aendern/setzen: ");
    scanf("%254s", neu_wert);
    
    if  (put_env(name, neu_wert) != 0) {
        printf("Konnte neuen Wert nicht setzen\n");
    }
    
    printf("Neuer Wert (%s): %s\n", name, getenv(name));
    return EXIT_SUCCESS;
}
