#include <curses.h>

int main (void) {
    int i, x, y;

    initscr();
    setscrreg(0, 10);
    for (i = 0; i < 30; ++i) {
        getyx(stdscr, y,x);
        printw("%d. (%d/%d): Ohne Funktion scrollok()\n", i, y, x);
        halfdelay(2);
        getch();
    }
    
    clear();
    refresh();
    
    scrollok(stdscr, TRUE);

    for (i = 0; i < 30; ++i) {
        getyx(stdscr, y,x);
        printw("%d. (%d/%d): Mit Funktion scrollok()\n", i, y, x);
        halfdelay(2);
        getch();
    }

    getch();
    endwin();
    
    return 0;
}
