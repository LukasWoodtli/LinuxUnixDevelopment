#include <sys/types.h>
#include <sys/stat.h>
#include <ftw.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>




static int dir_deep(const char* pfad) {
    int deep = 0;
    char *ptr = (char *)pfad;
    while ((ptr = strchr(ptr, '/'))) {
        ptr++;
        deep++;
    }
    
    return deep;
}

static int fn(const char *pfadname, const struct stat *attribut, int flag) {
    static int first = 1;
    static int deep;
    int i;
    
    if (!first) {
        for (i = 1; i <= dir_deep(pfadname) - deep; ++i)
            printf("%2c|", pfadname);
        printf("---[%s", strchr(pfadname, '/') + 1);
    }
    else {
        deep = dir_deep(pfadname);
        printf("[%s", pfadname);
        first = 0;
    }
    
    switch (flag) {
        case FTW_F:   printf("]\n");    break;
        case FTW_D:   printf("/]\n");   break;
        case FTW_DNR: printf("/-]\n");  break;
        default:      printf("???]\n"); break;
    }
    return 0;
}


int main(int argc, char **argv) {
    const char *ptr = NULL;
    
    if (argc < 2)
        ptr = ".";
    else
        ptr = argv[1];
    
    if (ftw(ptr, &fn, 16) == 0)
        return EXIT_SUCCESS;
    else
        printf("Fehler bei ftw() ...!\n");

    return EXIT_SUCCESS;
}