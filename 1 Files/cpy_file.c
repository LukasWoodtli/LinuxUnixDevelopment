
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#include <fcntl.h>

#include <stdlib.h>

#define BLOCK_SIZE 64 // bytes


ssize_t write2(int fd, const void *buffer, size_t count);

int main (int argc, char ** argv) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    char buffer[BLOCK_SIZE];
    
    int fd_r, fd_w;
    int n;
    
    umask(0);
    
    if (argc < 3) {
        fprintf(stderr, "> %s quelldatei zieldatei\n", *argv);
        return EXIT_FAILURE;
    }
    
    fd_r = open(argv[1], O_RDONLY);
    fd_w = open(argv[2], O_WRONLY | O_EXCL | O_CREAT, mode);
    
    if (fd_r == -1 || fd_w == -1) {
        perror("Fehler bei open()");
        return EXIT_FAILURE;
    }
    
    while ((n = read(fd_r, buffer, BLOCK_SIZE)) > 0) {
        if (write2(fd_w, buffer, n) > 0)
            perror("Fehler bei write()");
    }
    return EXIT_SUCCESS;
}


ssize_t write2(int fd, const void *buffer, size_t count) {
    while (count > 0) {
        ssize_t written = write(fd, buffer, count);
        if (written == -1)
            return -1;
        count -= written;
        buffer += written;
    }
    return 0;
}