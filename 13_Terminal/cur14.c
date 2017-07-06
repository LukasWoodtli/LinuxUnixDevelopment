#include <curses.h>
#include <stdlib.h>

int main (void) {
    MEVENT pos;
    int l_mouse;
    chtype button;

    initscr();

    noecho();
    keypad(stdscr, TRUE);
    l_mouse = mousemask(BUTTON1_RELEASED | BUTTON1_PRESSED | BUTTON1_CLICKED, NULL);
    (void)l_mouse;
    
    while (1) {
	button = wgetch(stdscr);
	clear();

	if (button == KEY_MOUSE) {
		if (getmouse(&pos) == OK) {
			if (pos.bstate == BUTTON1_PRESSED) {
				wenclose(stdscr, pos.y, pos.x);
				mvwprintw(stdscr, 1, 0, "Linke Maustaste gedrueckt: y = %2d x = %2d",
					pos.y, pos.x);
			}
			if (pos.bstate == BUTTON1_RELEASED) {
				mouseinterval(1);
				wenclose(stdscr, pos.y, pos.x);
				mvwprintw(stdscr, 2, 0, "Linke Maustaste losgelassen: y = %2d x = %2d",
					pos.y, pos.x);
			}
		}
		
	}
	wrefresh(stdscr);
    }

    endwin();
    
    return 0;
}
