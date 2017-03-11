#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

static void print_lim(rlim_t const * const lim) {
    if (*lim == RLIM_INFINITY) {
        printf("kein Limit\n");
    } else {
#ifdef __APPLE__    
   	printf("%lld \n", *lim);
#else /* linux */
        printf("%ld \n", *lim);
#endif
    }
}

static void limits(int resource) {
    struct rlimit l;
    
    if (getrlimit(resource, &l) < 0) {
        printf("Keine Angaben\n");
        return;
    }
    
    printf("Soft-Limit: ");
    print_lim(&l.rlim_cur);
    
    printf("Hard-Limit: ");
    print_lim(&l.rlim_max);
    
    
}


struct limit_and_name_t { int lim; const char * name;};
#define LIM(lim) {lim, #lim}

static const struct limit_and_name_t all_limits[] = {
    LIM(RLIMIT_CORE),
    LIM(RLIMIT_CPU),
    LIM(RLIMIT_DATA),
    LIM(RLIMIT_FSIZE),
    LIM(RLIMIT_MEMLOCK),
    LIM(RLIMIT_NOFILE),
    LIM(RLIMIT_NPROC),
    LIM(RLIMIT_RSS),
    LIM(RLIMIT_STACK),
#ifdef __linux__
    LIM(RLIMIT_AS),
    LIM(RLIMIT_MSGQUEUE),
    LIM(RLIMIT_NICE),
    LIM(RLIMIT_RTPRIO),
    LIM(RLIMIT_RTTIME),
    LIM(RLIMIT_SIGPENDING)
#endif /* __linux__ */
};

int main(void) {
    unsigned int i;
    for (i = 0; i < sizeof(all_limits)/sizeof(*all_limits); ++i) {
        printf("\n%s :\n", all_limits[i].name);
        limits(all_limits[i].lim);
    }
    
    return EXIT_SUCCESS;
}
