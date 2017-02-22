
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>

#include <string.h>

#include <fcntl.h>

int main(int argc, char **argv) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    ssize_t size;
    int fd; // file descriptor;
    char * str;
    
    umask(0);
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s datei_zum_oeffnen\n", *argv);
        return EXIT_FAILURE;
    }
    if (argc < 3) {
        str = "Keine Angaben gemacht\n";
    }
    else {
        str = argv[2];
    }
    size = strlen(str);

    fd = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, mode);
    
    if (fd == -1) {
        perror("Fehler bei open()");
        return EXIT_FAILURE;
    }
    
    if (write(fd, str, size) != size)
        perror("Fehler bei write()");
    return EXIT_SUCCESS;
}