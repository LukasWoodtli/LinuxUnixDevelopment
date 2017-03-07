
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>


static void bwread(int fd) {
    int rv, nl = 0;
    char ch;
    lseek(fd, -1, SEEK_END);
    while((rv = read(fd, &ch, 1)) == 1) {
        if (ch == '\n' && !nl) {
            ++nl;
        }
        else {
            putc(ch, stdout);
        }
        
        if (lseek(fd, -2, SEEK_CUR) < 0) {
            break;
        }
    }
    
    if (nl) {
        putc('\n', stdout);
    }
    
    if (rv == 0) {
        fprintf(stderr, "EOF unerwartet ...\n");
    }
    else if (rv == -1) {
        perror("Fehler bei read()");
    }
}


int main(int argc, char** argv) {
    int fd;
    if (argc < 2) {
        fprintf(stderr, "Syntax: %s datei\n", *argv);
        return EXIT_FAILURE;
    }
    
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("open");
        return EXIT_FAILURE;
    }
    
    bwread(fd);
    
    return EXIT_SUCCESS;
}
