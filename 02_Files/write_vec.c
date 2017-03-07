#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 3 // num elements

int main(void) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    const char *str[] = {"Ein string\n", "Noch einer\n", "Der letzte\n"};
    struct iovec vec[MAX];
    ssize_t length = 0;
    int fd, i;
    
    // use if file exists
    //unlink("test");
    umask(0);
    
    fd = open("test", O_WRONLY | O_CREAT | O_EXCL, mode);
    
    if (fd == -1) {
        perror("open()");
        return EXIT_FAILURE;
    }
    
    // einzelne Daten in die Struktur legen
    for (i = 0; i < MAX; ++i) {
        vec[i].iov_base = (void*)str[i];
        vec[i].iov_len = strlen(str[i]);
        length += strlen(str[i]);
    }
    
    // jetzt in die Datei schreiben
    if (writev(fd, vec, MAX) != length) {
        perror("writev()");
        return EXIT_FAILURE;
    }
    
    close(fd);
    
    return EXIT_SUCCESS;
}