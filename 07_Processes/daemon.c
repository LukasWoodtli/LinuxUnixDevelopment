// see man 7 daemon
// see also http://stackoverflow.com/questions/3095566/linux-daemonize/3095624#3095624
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>

typedef void (*sighandler_t)(int);

static sighandler_t handle_signal(int sig_nr, sighandler_t signalhandler) {
	struct sigaction neu_sig, alt_sig;
	neu_sig.sa_handler = signalhandler;
	sigemptyset(&neu_sig.sa_mask);
	neu_sig.sa_flags = SA_RESTART;
	
	if (sigaction(sig_nr, &neu_sig, &alt_sig) < 0)
		return SIG_ERR;
	return alt_sig.sa_handler;
}

static void start_deamon(const char *log_name, int facility) {
	int i;
	pid_t pid;
	
	/* 1. Eltern process beenden */
	if ((pid = fork()) != 0)
		exit(EXIT_FAILURE);
	
	/* 2. Kindprozess wird zum Sessionfuehrer */
	if (setsid() < 0) {
		printf("%s kann nicht Sessionfuehrer werden!\n", log_name);
		exit(EXIT_FAILURE);
	}
	
	/* 3. Signal SIGHUP ignorieren, Kind terminieren */
	handle_signal(SIGHUP, SIG_IGN);
	if ((pid = fork()) != 0)
		exit(EXIT_FAILURE);
	
	/* 4. chdir("/") */
	chdir("/");
	
	/* 5. complete control over the permissions */
	umask(0);
	
	/* 6. close fd's */
	for (i = sysconf(_SC_OPEN_MAX); i > 0; --i)
		close(i);

	/* logging */
	openlog(log_name, LOG_PID | LOG_CONS | LOG_NDELAY, facility);
}

int main(void) {
	int time = 15;

	start_deamon("meinDeamon", LOG_LOCAL0);
	
	while(1) {
		syslog(LOG_NOTICE, "Daemon gestartet ...\n");
		/* put code for daemon here */
		sleep(time);
		syslog(LOG_NOTICE, "Daemon laeuft %d sek\n", time);
		
		break;
	}
	
	syslog(LOG_NOTICE, "Daemon hat sich beendet\n");
	closelog();
	
	return EXIT_SUCCESS;
}
	