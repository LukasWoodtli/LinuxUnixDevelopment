#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include <unistd.h>

int main(void) {
    int fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    pid_t pid;
    
    int x, y;
    
    if (fd == -1) {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    
    if ((pid = fork()) == -1) {
        perror("fork()");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // elternprozess
        srand(time(NULL) ^ getpid());
        for (x = 0; x < 10; ++x) {
            usleep(rand() % 10 * 10000);
            write(fd, "x", 1);
            write(STDOUT_FILENO, "x", 1);
        }
        
        wait(NULL);
        printf("\n");
    } else {
        // Kindprozess
        srand(time(NULL) ^ getpid());
        for (y = 0; y < 10; ++y) {
            usleep(rand() % 10 * 10000);
            write(fd, "X", 1);
            write(STDOUT_FILENO, "X", 1);
        }
    }
    
    return EXIT_SUCCESS;
}


