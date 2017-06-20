#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int main(void) {
    struct termios term;
    int c;
    
    if ((tcgetattr(STDIN_FILENO, &term)) < 0) {
        printf("Fehler bei tcgetattr(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    // ASCII-Code 27 = ESC == EOF
    term.c_cc[VEOF] = 27;
    if ((tcsetattr(STDIN_FILENO,TCSAFLUSH, &term)) < 0) {
        printf("Fehler bei tcsetattr(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    printf("Eingabe machen (mit ESC abbrechen)\n");
    while ((c = getchar()) != EOF)
        putchar(c);
    return EXIT_SUCCESS;
}
