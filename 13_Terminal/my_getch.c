#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

static struct termios new_io;
static struct termios old_io;

// set terminal to raw-mode
// this function implements the same functionality
// as `int cfmakeraw(struct termios *tty_zeiger)` in `#include <termios.h>`

static int raw(int fd) {
    if ((tcgetattr(fd, &old_io)) == -1)
        return -1;
    
    new_io = old_io;
    
    new_io.c_iflag =
        new_io.c_iflag & ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    new_io.c_oflag = new_io.c_oflag & ~OPOST;
    new_io.c_cflag = new_io.c_cflag & ~(CSIZE | PARENB);
    new_io.c_lflag = new_io.c_lflag & ~(ECHO | ICANON | IEXTEN | ISIG);
    new_io.c_cflag = new_io.c_cflag | CS8;
    new_io.c_cc[VMIN] = 1;
    new_io.c_cc[VTIME] = 0;
    
    if ((tcsetattr(fd, TCSAFLUSH, &new_io)) == -1)
        return -1;
    
    return 0;
}


static int getch() {
    int c;
    if (raw(STDIN_FILENO) == -1) {
        printf("Fehler bei raw()\n");
        exit(EXIT_FAILURE);
    }
    
    c = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
    return c;
}

int main(void) {
    int zeichen;
    printf("Warte auf 'q' um das Programm zu beenden\n");

    while ((zeichen = getch()) != 'q')
        printf("'%c' != 'q'\n", zeichen);
    printf("'%c' == 'q'\n", zeichen);
    printf("ENDE\n");
    
    return EXIT_SUCCESS;
}
