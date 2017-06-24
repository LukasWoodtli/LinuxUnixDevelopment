#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

#define PASS 8

static char passwort[PASS];

static char *my_getpass(void) {
    struct termios origstate, changedstate, teststate;
    char *p = passwort;
    
    if (tcgetattr(0, &origstate) != 0)
        printf("tcgetattr() failed\n");
    
    changedstate = origstate;
    
    // no output
    changedstate.c_lflag &= ~ECHO;
    // activate new-line
    changedstate.c_lflag |= ECHONL;
    if (tcsetattr(0, TCSANOW, &changedstate) != 0)
        printf("tcsetattr() failed\n");
    
    // check if terminal state ok
    tcgetattr(0, &teststate);
    if (teststate.c_lflag & ECHO)
        printf("ECHO-Flag konnte nicht abgeschaltet werden\n");
    if (!(teststate.c_lflag & ECHONL))
        printf("ECHONL-Flag konnte nicht eingeschaltet werden\n");

    fprintf(stdout, "Passwort eingeben: ");
    fgets(passwort, PASS, stdin);
    
    // restore old terminal state
    if (tcsetattr(0, TCSANOW, &origstate) != 0)
        printf("tcsetattr() failed");
    
    return p;
}

int main(void) {
    char* ptr = my_getpass();
    printf("Ihre Eingabe war: %s\n", ptr);

    return EXIT_SUCCESS;
}
