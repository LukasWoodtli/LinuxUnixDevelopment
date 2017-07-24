#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#define bool int
#define true 1
#define false 0

#define SERVER_PORT 2001
#define BUFFER_SIZE 2048

typedef void (*signalhandler_t)(int);

static signalhandler_t my_signal(int sig_nr, signalhandler_t signalhandler) {
    struct sigaction neu_sig, alt_sig;
    neu_sig.sa_handler = signalhandler;
    sigemptyset(&neu_sig.sa_mask);
    neu_sig.sa_flags = SA_RESTART;
    
    if (sigaction(sig_nr, &neu_sig, &alt_sig) < 0)
        return SIG_ERR;
    
    return alt_sig.sa_handler;
}


static int GetToken(char* buf, int n, char *token, int toklen, char delim) {
    int len;
    int i = 0;
    int k;
    
    for (k = 1; k <= n; ++k) {
        token[0] = 0;
        len = 0;
        
        while (buf[i] != delim && buf[i] != ' '  && buf[i] != '\0'  &&
               buf[i] != '\t'  && buf[i] != 10  && buf[i] != 13) {
            token[len++] = buf[i++];
            
            if (len > toklen - 1) {
                return -1;
            }
        }
        
        token[len] = 0;
    }
    return len;
}


static int SendBlock(int soc, unsigned char *buf, int size) {
    unsigned char *r;
    int len = 0;
    r = buf;
    
    while (size > 0) {
	    if ((len = send(soc, r, size, 0)) == -1) {
		    return -1;
	    }
	    
	    size -= len;
	    r += len;
    }
    return 0;
}


static int SendBuf(int soc, char *buf) {
	char *r = NULL;
	int len = 0;
	int rlen = strlen(buf);
	r = buf;
	
	while (rlen > 0) {
		if ((len = send(soc, r, strlen(r), 0)) == -1) {
			return -1;
		}
		
		rlen -= len;
		r += len;
	}
	
	return 0;
}


static void SendHTTPHeader(int soc, int code, const char *phase, int length, time_t *pftime) {
	// ...
}





int main(void) {
 
    return EXIT_SUCCESS;
}
