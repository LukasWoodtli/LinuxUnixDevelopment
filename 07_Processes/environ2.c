#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv, char **envp) {
    (void)argc;
    (void)argv;
    while(envp != NULL && *envp != NULL) {
        printf("%s\n", *envp);
        envp++;
    }

    return EXIT_SUCCESS;
}
