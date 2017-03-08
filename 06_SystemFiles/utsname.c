#define __USE_GNU
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <string.h>


int main(void) {
	struct utsname uts_info;
	
	if (uname(&uts_info) < 0) {
		fprintf(stderr, "Fehler bei uname()\n");
		return EXIT_FAILURE;
	}
	
	printf("Folgende Angaben wurden ermittelt:\n");
	printf("Betriebsystem   : %s\n", uts_info.sysname);
	printf("Knotenname      : %s\n", uts_info.nodename);
	printf("Release         : %s\n", uts_info.release);
	printf("Version         : %s\n", uts_info.version);
	printf("Architektur     : %s\n", uts_info.machine);
	
#ifndef __APPLE__
	printf("Domainname      : %s\n", uts_info.__domainname);
#endif
	return EXIT_SUCCESS;
}

