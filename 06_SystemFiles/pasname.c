#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>


int main(int argc, char* argv[]) {
	struct passwd *pas_ptr;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: %s Name\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	
	char * str_rest;
	int uid = strtoul(argv[1], &str_rest, 10);
	
	if (*str_rest == '\0')
		pas_ptr = getpwuid(uid);
	else
		pas_ptr = getpwnam(argv[1]);


	if (pas_ptr == NULL) {
		printf("Konnte nichts ueber %s ermitteln\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	
	printf("User Angaben:\n");
	printf("Benutzername        : %s\n", pas_ptr->pw_name);
	printf("Benutzernummer      : %d\n", pas_ptr->pw_uid);
	printf("Gruppennummer       : %d\n", pas_ptr->pw_gid);
	printf("Kommentar           : %s\n", pas_ptr->pw_gecos);
	printf("Home-Verzeichnis    : %s\n", pas_ptr->pw_dir);
	printf("Login-Shell         : %s\n", pas_ptr->pw_shell);

	return EXIT_SUCCESS;
}

