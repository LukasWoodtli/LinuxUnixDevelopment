#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF 10

static char *alloc_memory(unsigned int mem) {
    return malloc(mem);
}

int main(void) {
    char* ptr = alloc_memory(BUF);
    char* buf;
    char array[BUF];
    unsigned int i;
    
    printf("Eingabe machen: ");
    fgets(ptr, BUF, stdin);
    printf("Die Eingabe lautet: %s", ptr);
    
    // buffer overflow
    strcat(ptr, " 123456789");
    printf("%s\n", ptr);
    
    // wrong free
    for (i = 0; i < BUF; ++i) {
        buf = alloc_memory(BUF * BUF);
        
        // free not allocated memory
        free(ptr);
    }
    
    return EXIT_SUCCESS;
}
