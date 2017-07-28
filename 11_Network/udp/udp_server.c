#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define LOCAL_SERVER_PORT 1234

#define BUF 255

int main(int argc, char **argv) {
    (void)argc;
    int s, rc, n;
    socklen_t len;
    struct sockaddr_in cliAddr, servAddr;
    char puffer[BUF + 1];
    time_t time1;
    char loctime[BUF + 1];
    char *ptr;
    const int y = 1;
    
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        printf ("%s: Kann Socket nicht öffnen ...(%s)\n",
                argv[0], strerror(errno));
        exit (EXIT_FAILURE);
    }
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htons(INADDR_ANY);
    servAddr.sin_port = htons(LOCAL_SERVER_PORT);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
    
    
    
    rc = bind(s, (struct sockaddr*)&servAddr, sizeof(servAddr));
    
    if (rc < 0) {
        printf("%s: Kann Portnummer %d nicht binden (%s)\n",
               argv[0], LOCAL_SERVER_PORT, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    printf("%s: Wartet auf Daten am Port (UDP), %u\n",
           argv[0], LOCAL_SERVER_PORT);

    while (1) {
        
        memset(puffer, 0, BUF);
        
        len = sizeof(cliAddr);
        
        n = recvfrom(s, puffer, BUF, 0,
                     (struct sockaddr*)&cliAddr, &len);
        if (n < 0) {
            printf("%s: Kann keine Daten empfangen ...\n",
                   argv[0]);
            continue;
        }

        time(&time1);
        strncpy(loctime, ctime(&time1), BUF);
        ptr = strchr(loctime, '\n');
        *ptr = '\0';
        
        printf("%s: Daten erhalten von %s:UDP%u : %s \n",
               loctime, inet_ntoa(cliAddr.sin_addr),
               htons(cliAddr.sin_port), puffer);
    }

    return EXIT_SUCCESS;
}
