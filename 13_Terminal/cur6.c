#include <curses.h>

int main (void) {
    initscr();

    mvprintw(5, 5, "Diese Zeile enthaelt einen Fehhler");
    mvprintw(6, 5, "Taste druecken fuer Korrektur");
    getch();
    
    mvdelch(5, 34);
    mvprintw(7, 5, "Fehler wurde korrigiert! Bitte Taste druecken!");
    getch();
    
    endwin();
    
    return 0;
}
