#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(void) {

    mode_t mode = S_ISUID|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH;

    const char * const new_file = "test_file.txt";
    int fd; // file descriptor

    umask(0);
    fd = open(new_file, O_WRONLY | O_EXCL | O_CREAT, mode);
    if(-1 == fd) {
        printf("%s",strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
