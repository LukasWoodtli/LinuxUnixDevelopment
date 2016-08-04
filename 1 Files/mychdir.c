#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX 256

int main(int argc, char **argv) {
    mode_t mode = S_IRWXU;
    char wd[MAX];
    
    if (argc < 2) {
        printf("USAGE: %s neues_verzeichnis\n", *argv);
        return EXIT_FAILURE;
    }
    
    umask(0);
    
    if (mkdir(argv[1], mode) != -1)
        printf("Verzeichnis \"%s\" erfolgreich erstellt\n", argv[1]);
    else
        printf("Konnte das Verzeichnis nicht erstellen (%s)\n", strerror(errno));
    
    if (chdir(argv[1]) == -1)
        printf("Konnte das Verzeichnis nicht wechseln\n");
    if (NULL != getcwd(wd, MAX))
        printf("Neues Arbeitsverzeichnis: %s\n", wd);
    return EXIT_SUCCESS;
}
