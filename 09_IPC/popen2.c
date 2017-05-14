#define _GNU_SOURCE
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define BUF1 255
#define BUF2 8192

static void sendmail(const char *to, const char *from,
                     const char *subject, const char *inhalt) {
    FILE *pipe, *pfad;
    char pfad_sendmail[BUF1];

    /* find path of sendmail */
    pfad = popen("which sendmail", "r");
    if (pfad == NULL) {
        perror("popen()");
        exit(EXIT_FAILURE);
    }
    
    fscanf(pfad, "%s", pfad_sendmail);
    pclose(pfad);
    strcat(pfad_sendmail, " -oi -t");
    
    /* path is usually `/usr/sbin/sendmail` */
    pipe = popen(pfad_sendmail, "w");
    if (pipe == NULL) {
        fprintf(stderr, "Konnte keine Verbindung zu sendmail aufbauen!\n");
        exit(EXIT_FAILURE);
    }
    
    /* write to sendmail */
    fprintf(pipe, "From:%s To:%s", from, to);
    if (subject != NULL) {
        fprintf(pipe, "Subject:%s", subject);
    }

    /* new-line is important `-v` */
    if (inhalt != NULL) { fprintf(pipe, "\n%s\n",  inhalt); }
    pclose(pipe);
    printf("Mail wurde versendet an: %s\n", to);
}

int main(void) {
    char to[BUF1], from[BUF1], sub[BUF1], inhalt[BUF2], puffer[BUF1];
    
    printf("Mail to: ");
    fgets(to, BUF1, stdin);
    
    printf("Mail from: ");
    fgets(from, BUF1, stdin);
    
    printf("Betreff: ");
    fgets(sub, BUF1, stdin);
    
    sub[strlen(sub) - 1] = '\0';
    inhalt[0] = '\0';
    
    printf("Inhalt (beenden mit CTR+D) \n>");
    while (fgets(puffer, BUF1 - 1, stdin) != NULL) {
        if (strlen(inhalt) + strlen(puffer) < BUF2) {
            strcat(inhalt, puffer);
            printf(">");
        }
        else {
            break;
        }
    }
    
    inhalt[BUF2 - 1] = '\0';
    
    sendmail(to, from, sub, inhalt);
    
    return EXIT_SUCCESS;
}

