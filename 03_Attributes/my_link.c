#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define HARD_LINK "hard_link"
#define SYM_LINK "sym_link"

#define MAX 255

static int attribute(const char *name) {
    struct stat st;
    
    if (stat(name, &st) < 0) {
        fprintf(stderr, "Fehler bei stat: %s\n",
                strerror(errno));
        return -1;
    }
    
    printf("Anzahl Links  : %d\n", st.st_nlink);
    return 1;
}

int main(int argc, char** argv) {
    const char *ptr;
    char buf[MAX];
    int n;
    
    if (argc < 2) {
        printf("usage: %s datei\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    ptr = argv[1];
    
    // num links
    if (attribute(ptr) < 0) {
        return EXIT_FAILURE;
    }
    
    // create hard link
    if (link(ptr, HARD_LINK) == -1) {
        printf("Konnte keinen Hardlink erzeugen\n");
        return EXIT_FAILURE;
    }
    
    printf("Nach dem Aufruf fon link()\n");
    
    // wieder Anzahl links erfragen
    if (attribute(ptr) < 0) {
        return EXIT_FAILURE;
    }
    
    // symlink erstellen
    if (symlink(ptr, SYM_LINK) == -1) {
        printf("Konnte keinen symlink erstellen\n");
        return EXIT_FAILURE;
    }
    
    printf("Nach dem Funktionsaufruf von symlink()\n");
    
    // wieder Anzahl Links erfragen
    if (attribute(ptr) < 0) {
        return EXIT_FAILURE;
    }
    
    // namen anzeigen (symlink)
    n = readlink(SYM_LINK, buf, MAX);
    printf("Der symbolische Link \"%s\" zeigt auf \"%.*s\"\n", SYM_LINK, n, buf);
    
    // links entfernen
    unlink(HARD_LINK);
    unlink(SYM_LINK);
    return EXIT_SUCCESS;
}

