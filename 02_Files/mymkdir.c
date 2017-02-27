#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char ** argv) {
    mode_t mode = S_IRWXU;
    
    if (argc < 2) {
        printf("USAGE: %s neues_verzeichnis\n", *argv);
        return EXIT_FAILURE;
    }
    
    umask(0);
    
    if ((mkdir(argv[1], mode)) != -1) {
        printf("Verzeichnis \"%s\" erfolgreich erstellt\n", argv[1]);
    }
    else {
        printf("Konnte Verzeichnis nicht erstellen (%s)\n", strerror(errno));
    }
    
    return EXIT_SUCCESS;
}