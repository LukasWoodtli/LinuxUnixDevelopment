#include <curses.h>

int main (void) {
    int i, j;

    initscr();

    addch(ACS_ULCORNER);
    
    for (i = 0; i < 20; ++i)
        addch(ACS_HLINE);
    
    addch(ACS_URCORNER);
    addch('\n');
    
    for (i = 0; i < 10; ++i) {
        for (j = 0; j <= 21; ++j) {
            if (j == 0) {
                addch(ACS_VLINE);
            }
            else if (j == 21) {
                addch(ACS_VLINE);
                addch('\n');
            }
            else {
                addch(' ');
            }
        }
    }
    
    addch(ACS_LLCORNER);
    
    for (i = 0; i < 20; ++i)
        addch(ACS_HLINE);
    
    addch(ACS_LRCORNER);
    
    addch('\n');
    
    mvprintw(5, 7, "<TASTE>");
    getch();
    
    endwin();
    
    return 0;
}
