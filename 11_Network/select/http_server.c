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


static void SendHTTPHeader(int soc, int code, const char *phase,
                           int length, time_t *pftime) {
    char msg[255] = {0};
    struct tm *ptm, *pftm;
    time_t stime;
    
    sprintf(msg, "HTTP/1.1 %d %s\r\n", code, phase);
    SendBuf(soc, msg);
    time(&stime);
    ptm = gmtime(&stime);
    strftime(msg, 255, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", pftm);
    
    SendBuf(soc, msg);
    SendBuf(soc, "Server: http_server 0.1\r\n");
    
    if (pftime != NULL) {
        pftm = gmtime(pftime);
        strftime(msg, 255, "Last-Modified: %a, %d %b %Y %H:%M:%S %Z\r\n", pftm);
        SendBuf(soc, msg);
    }
    
    SendBuf(soc, "Accept-Ranges: none\r\n");
    sprintf(msg, "Content-Length: %d\r\n", length);
    SendBuf(soc, msg);
    SendBuf(soc, "Connection: Keep-Alive\r\n");
    SendBuf(soc, "Content-Type text/html\r\n");
    SendBuf(soc, "\r\n");
}


static int httpd(int soc) {
    int retval = 0;
    char buf[512] = {0};
    char method[255] = {0};
    char uri[512] = {0};
    char headline[1024] = {0};
    char request_header[BUFFER_SIZE] = {0};
    char *r = NULL;
    char hTitle[32] = {0};
    char hValue[128] = {0};
    FILE *fd;
    bool isFirst = true;
    
    fd = fdopen(soc, "r");
    printf("-----------Request-Header Client-------------\n");
    
    while (1) {
        r = fgets(buf, BUFFER_SIZE, fd);
        if (*r == '\r' && *(r + 1) == '\n') {
            break;
        }
        
        printf("ClientRequest: %s", r);
        
        if (isFirst) {
            strcpy(headline, r);
            GetToken(headline, 1, method, 255, 0);
            isFirst = false;
        }
        else {
            strcat(request_header, r);
            GetToken(r, 1, hTitle, 32, ':');
            if (strcmp(hTitle, "Connection") == 0) {
                GetToken(r, 2, hValue, 128, ' ');
                if (strcmp(hValue, "Keep-Alive") == 0) {
                    printf("Client benoetigt keep alive\r\n");
                    retval = 1;
                }
                else {
                    printf("Client benoetigt kein keep alive\r\n");
                    retval = -1;
                }
            }
        }
    }
    
    printf("Fertig mit dem Lesen des Request-Headers...\r\n");
    printf("-------------------------------------------\n");
    fflush(stdout);
    
    if ((strcmp(method, "GET") == 0) || (strcmp(method, "get") == 0)) {
        char dir[255] = {0};
        char path[512] = {0};
        FILE* file;
        struct stat fst;
        GetToken(headline, 2, uri, 1024, 0);
        GetToken(uri, 1, dir, 255, '/');
        printf("Folgende Datei wurde vom Client angefordert: %s\r\n", uri);
        sprintf(path, ".%s", uri);
        file = fopen(path, "r");
        
        if (file == NULL) {
            unsigned char error404[] =
                "<HTML><HEAD><TITLE>404 Not Found</TITLE>"
                "</HEAD><BODY><H1>Nicht gefunden</H1>"
                "Die angeforderte URL konnte nicht auf "
                "disem Server gefunden werden."
                "<P><HR><I>http_server0.1</I></BODY></HTML>\r\n\r\n";
            printf("404 Nacricht\r\n");
            SendHTTPHeader(soc, 404, "FILE NOT FOUND.",
                           strlen((const char*)error404) + 1, NULL);
            SendBlock(soc, error404, strlen((const char*)error404) + 1);
            fflush(stdout);
            SendBuf(soc, "\r\n\r\n");
            return -1;
        }
        else {
            unsigned char *blob = NULL;
            stat(path, &fst);
            printf("Datei existiert ...\r\n");
            fflush(stdout);
            
            SendHTTPHeader(soc, 200, "OK", fst.st_size, &(fst.st_mtime));
            blob = (unsigned char*)malloc(fst.st_size);
            fread(blob, fst.st_size, 1, file);
            SendBlock(soc, blob, fst.st_size);
            
            fclose(file);
            printf("Alle Daten gesendet ...\r\n");
            fflush(stdout);
            SendBuf(soc, "\r\n\r\n");
            free(blob);
        }
    }
    
    return retval;
}


int main(void) {
 
    return EXIT_SUCCESS;
}
