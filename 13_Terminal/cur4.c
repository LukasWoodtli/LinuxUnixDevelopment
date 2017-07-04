#include <curses.h>

#define MAX 100

int main (void) {
    char string[MAX];
    char string2[MAX];
    const char string3[] = "Dies ist der neue String3\n";
    int c;

    initscr();
    printw("Bitte Eingabe machen : ");
    getnstr(string, MAX);

    mvprintw(3, 0, "Eingabe für 2. String machen : ");
    getnstr(string2, MAX);

    mvprintw(7, 20, "String ist gleich: %s\n", string);
    mvprintw(8, 20, "Diese Zeile wird mit deleteln() gelöscht\n");
    mvprintw(7, 20, "String 2 ist gleich: %s\n", string2);
    mvprintw(14, 20, "Weiter mit TASTE");

    noecho();
    
    c = getch();
    move(8, 20);
    
    deleteln();
    noecho();
    
    c = getch();
    move(8, 20);
    
    insertln();
    mvprintw(8, 20, string3);
    
    c = getch();
    
    clear();
    refresh();
    
    mvprintw(12, 38, "ENDE");
    
    // wait 20 * 1/10-sec
    halfdelay(20);
    
    c = getch();
    
    (void)c;

    endwin();
    
    return 0;
}
