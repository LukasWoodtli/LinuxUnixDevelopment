#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>
#include <unistd.h>

#define MAX 255

static int search = 0;
static regex_t regexpr;

static int my_select (const struct dirent *dp) {
    if (search == 0)
        return 1;
    if (regexec(&regexpr, dp->d_name, 0, NULL, 0) == 0)
        return 1;
    else
        return 0;
}

int main (int argc, char **argv) {
    int num_entries, i;
    struct dirent **namelist, **list;
    const char *ptr = NULL;
    char wd[MAX];
    
    if (argc < 2)
        ptr = ".";
    else
        ptr = argv[1];
    
    if (argc == 3) {
        if (regcomp(&regexpr, argv[2], REG_EXTENDED)) {
            printf("Problem beim Ausdruck %sn", argv[2]);
            return EXIT_FAILURE;
        }
        
        search = 1;
    }
    
    if ((num_entries = scandir(ptr, &namelist, my_select, 0)) < 0) {
        fprintf(stderr, "Unerwarteter Fehler\n");
        exit(EXIT_FAILURE);
    }
    
    chdir(ptr);
    
    getcwd(wd, MAX);
    
    printf("Anzahl der Einträge in %s: %d\n", wd, num_entries);
    
    if (num_entries) {
        printf("Die Einträge:\n");
        for (i = 0, list = namelist; i < num_entries; ++i, *list++) {
            printf(" %s\n", (*list)->d_name);
            free(*list);
        }
        free(namelist);
        printf("\n");
    }
    
    printf("\n");
    regfree(&regexpr);
    return EXIT_SUCCESS;
}
