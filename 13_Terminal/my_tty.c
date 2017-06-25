#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

static char termname[L_ctermid];

int main(void) {
    const char *ptr = ctermid(termname);
    if (ptr != NULL)
        printf("Kontrollterminal-Name = %s\n", ptr);
    else
        printf("Konnte Terminal nicht ermitteln\n");
    
    return EXIT_SUCCESS;
}
