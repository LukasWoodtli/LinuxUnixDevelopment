#include <curses.h>

int main(void) {
    int c;
    initscr();
    
    keypad(stdscr, TRUE);
    noecho();
    while ((c = getch()) != 'q') {
        switch (c) {
            case KEY_DOWN:
                printw("KEY_DOWN : %d\n", c);
                break;
            case KEY_UP:
                printw("KEY_UP : %d\n", c);
                break;
            case KEY_LEFT:
                printw("KEY_LEFT : %d\n", c);
                break;
            case KEY_RIGHT:
                printw("KEY_RIGHT : %d\n", c);
                break;
            default:
                printw("Tastaturcode : %d\n", c);
                break;
        }
    }
    
    endwin();
    return 0;
}
