#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define MAX 255

int main(int argc, char ** argv) {
    int num_entries, i;
    struct dirent **namelist, **list;
    const char *ptr = NULL;
    char wd[MAX];
    
    if (argc < 2)
        ptr = ".";
    else
        ptr = argv[1];
    
    if ((num_entries = scandir(ptr, &namelist, 0, alphasort)) < 0) {
        fprintf(stderr, "Unerwarteter Fehler\n");
        exit (EXIT_FAILURE);
    }
    
    chdir(ptr);
    getcwd(wd, MAX);
    printf("Anzahl der Einträge in %s: %d\n", wd, num_entries);
    
    if (num_entries) {
        printf("Die Einträge:\n");
        for (i=0, list=namelist; i<num_entries; i++, *list++) {
            printf(" %s\n", (*list)->d_name);
            free(*list);
        }
        free(namelist);
        printf("\n");
    }

    printf("\n");
    
    return EXIT_SUCCESS;
}