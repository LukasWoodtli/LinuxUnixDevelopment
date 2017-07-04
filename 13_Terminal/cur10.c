#include <curses.h>
#include <stdlib.h>

#define QUIT 113 /* 'q' */
#define LEFT 260 /* '<-' */
#define RIGHT 261 /* '->' */

static void print_raumschiff(int x) {
    mvdelch(9, x + 2);
    mvdelch(9, x + 1);
    mvdelch(9, x);
    mvdelch(9, x - 1);
    mvdelch(9, x - 2);
    
    mvaddch(10, x - 1, ACS_LLCORNER);
    mvaddch(10, x + 1, ACS_LRCORNER);
    mvaddch(10, x, ACS_TTEE);
}

int main (void) {
    int x = 40, zufall, c = 0 , i;
    srand(79);

    initscr();

    keypad(stdscr, TRUE);
    noecho();
    scrollok(stdscr, TRUE);
    scroll(stdscr);
    
    while (c != QUIT) {
        scrl(1);
        
        for (i = 0; i < 5; ++i) {
            zufall = rand() % 79;
            mvaddch(20, zufall, '*');
            mvprintw(0, 0, "'q' druecken fuer Quit | "
                     "Taste fuer Start | "
                     "<- nach links -> nach rechts");
            mvprintw(1,0, "Position Raumschiff %d", x);
        }
        
        c = getch();
        halfdelay(3);
        
        switch (c) {
            case LEFT:
                if (x < 1)
                    x = 79;
                else
                    --x;
                break;
            case RIGHT:
                if (x > 79)
                    x = 1;
                else
                    ++x;
                break;
            default:
                break;
        }
        print_raumschiff(x);
    }

    endwin();
    
    return 0;
}
