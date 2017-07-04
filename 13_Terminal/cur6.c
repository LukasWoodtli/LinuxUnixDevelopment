#include <curses.h>

int main (void) {
    initscr();

    mvprintw(5, 5, "Diese Zeile enthält einen Fehhler");
    mvprintw(6, 5, "Taste drücken für Korrektur");
    getch();
    
    mvdelch(5, 33);
    mvprintw(7, 5, "Fehler wurde korrigiert! Bitte Taste drücken!");
    getch();
    
    endwin();
    
    return 0;
}
