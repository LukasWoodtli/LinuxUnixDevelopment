#include <curses.h>

#define MAX 100

int main (void) {
    char string[MAX];
    char string2[MAX];
    
    initscr();
    printw("Bitte Eingabe machen : ");
    getnstr(string, MAX);
    mvprintw(5, 10, "Eingabe machen : ");
    mvgetnstr(5, 30, string2, MAX);
    endwin();

    printf("Die 1. Eingabe lautet: %s\n", string);
    printf("Die 2. Eingabe lautet: %s\n", string2);
    
    return 0;
}
