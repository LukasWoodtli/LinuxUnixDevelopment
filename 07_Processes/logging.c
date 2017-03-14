#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

/* register in /etc/syslog.conf, /etc/rsyslog.conf ... */
int main(void) {
	openlog("logging", LOG_PID | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);

	/* ... viel code ... */
	syslog(LOG_NOTICE, "Hallo Welt mit syslog\n");
	closelog();
	
	return EXIT_SUCCESS;
}
