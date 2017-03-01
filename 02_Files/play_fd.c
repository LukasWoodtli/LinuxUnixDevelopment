
#include <sys/types.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main(int argc, char** argv) {
    
    int fd, fdup;
    int flags, modus;
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s DATEI\n", *argv);
        exit(EXIT_FAILURE);
    }
    
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "Konnte %s nicht öffnen!\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if ((fdup = fcntl(fd, F_DUPFD, 0)) == -1) {
        printf("Filedescriptor erfolgreich dupliziert\n");
    }
    
    
    if (fcntl(fd, F_GETFD, FD_CLOEXEC) == 1) {
        printf(("Close-on-exec-Bit ist schon gesetzt\n"));
    }
    else if (fcntl(fd, F_SETFD, FD_CLOEXEC) < 0) {
        printf("Konnte close-on-exec-Bit nicht setzen\n");
    }
    else {
        printf("Close-on-exec Bit wurde gesetzt\n");
    }
    
    flags = fcntl(fd, F_GETFL, 0);
    modus = flags & O_ACCMODE;

    if (modus == O_RDWR) {
        printf("Datei nur zum lesen und schreiben geöffnet\n");
    }
    else if (modus == O_RDONLY) {
        printf("Datei nur zum lesen geöffnet\n");
    }
    else if (modus == O_WRONLY) {
        printf("Datei nur zum schreiben geöffnet\n");
    }
    
    if (modus == O_APPEND) {
        printf("Wir können am Dateiende schreiben\n");
    }

    modus |= O_APPEND;
    if (fcntl(fd, F_SETFL, modus) < 0) {
        printf("Konnte Attribut O_APPEND nicht setzen\n");
    }
    
    if (modus == O_APPEND) {
        printf("O_APPEND gesetzt\n");
    }
    
    flags = fcntl(fd, F_GETFD, 0);
    modus = flags & O_ACCMODE;
    if (modus == O_NONBLOCK) {
        printf("O_NONBLOCK schon gesetzt\n");
    }
    else {
        modus |= O_NONBLOCK;
        fcntl(fd, F_SETFL, modus);
        if (modus == O_NONBLOCK) {
            printf("O_NONBLOCK erfolgreich gesetzt\n");
        }
        else {
            printf("Konnte O_NONBLOCK nicht setzen\n");
        }
    }
    
    return EXIT_SUCCESS;

}
