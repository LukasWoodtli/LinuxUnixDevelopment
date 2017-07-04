#include <curses.h>

int main (void) {
    int i;

    initscr();
    setscrreg(5, 10);

    for (i = 0; i < 30; ++i) {
        printw("%d: Ohne Funktion scrollok()\n", i);
        halfdelay(2);
        getch();
    }
    
    clear();
    refresh();
    
    scrollok(stdscr, TRUE);

    for (i = 0; i < 30; ++i) {
        printw("%d: Mit Funktion scrollok()\n", i);
        halfdelay(2);
        getch();
    }

    getch();
    endwin();
    
    return 0;
}
