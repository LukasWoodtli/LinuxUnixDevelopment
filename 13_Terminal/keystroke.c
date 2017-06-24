#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>


static struct termios BACKUP_TTY;

static int new_tty(int fd) {
    struct termios buffer;
    
    if ((tcgetattr(fd, &BACKUP_TTY)) == -1)
        return -1;
        
    buffer = BACKUP_TTY;
    // ECHO: output on screen
    // ICANON: line oriented input
    // ISIG: no CTRL-C
    buffer.c_lflag = buffer.c_lflag & ~(ECHO | ICANON | ISIG);
    buffer.c_cc[VMIN] = 1;
    
    if ((tcsetattr(fd, TCSAFLUSH, &buffer)) == -1)
        return -1;
    
    return 0;
}


static int restore_tty(int fd) {
    if ((tcsetattr(fd, TCSAFLUSH, &BACKUP_TTY)) == -1)
        return -1;

    return 0;
}


int main(void) {
    int rd;
    char c, buffer[10];
    
    if (new_tty(STDIN_FILENO) == -1) {
        printf("Fehler bei new_tty()\n");
        exit(EXIT_FAILURE);
    }
    
    do {
        if (read(STDIN_FILENO, &c, 1) < 1) {
            printf("Fehler bei read()\n");
            restore_tty(STDIN_FILENO);
            exit(EXIT_FAILURE);
        }
        
        // check for ESC ('\E')
        if (c == 27) {
            rd = read(STDIN_FILENO, buffer, 4);
            buffer[rd] = '\0';

            if (strcmp(buffer, "[[A") == 0)
                printf("F1\n");
            if (strcmp(buffer, "[[B") == 0)
                printf("F2\n");
            if (strcmp(buffer, "[C") == 0)
                printf("->\n");
            if (strcmp(buffer, "[D") == 0)
                printf("<-\n");
            if (strcmp(buffer, "[B") == 0)
                printf("V\n");
            if (strcmp(buffer, "[A") == 0)
                printf("^\n");
            if (strcmp(buffer, "[1~") == 0)
                printf("Pos 1\n");
            if (strcmp(buffer, "[6~") == 0)
                printf("Pg Dwn\n");
            if (strcmp(buffer, "[5~") == 0)
                printf("Pg Up\n");
        }
        else {
            if ((c < 32) || (c == 127))
                printf("%d\n", c);
            else
                printf("%c\n", c);
        }
        
    } while (c != 'q');
    
    restore_tty(STDIN_FILENO);

    return EXIT_SUCCESS;
}
