#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <shadow.h>
#include <string.h>
#include <time.h>

/* needs to be run as root (e.g. sudo) */

struct spwd *getshadow(const char* name) {
	struct spwd *shadow_ptr;
	while((shadow_ptr=getspent())) {
		if(strcmp(shadow_ptr->sp_namp, name) == 0) {
			endspent();
			return shadow_ptr;
		}
	}
	endspent();
	return NULL;
}

int main(int argc, char* argv[]) {
	struct spwd* shadow_ptr;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s Name\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	shadow_ptr = getshadow(argv[1]);
	
	if (shadow_ptr == NULL) {
		printf("Konnte nichts ueber %s ermitteln\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	
	printf("User (passwd) Angaben:\n");
	printf("Benutzername                    : %s\n",  shadow_ptr->sp_namp);
	printf("Passwort (verschluesselt)       : %s\n",  shadow_ptr->sp_pwdp);
	printf("Tag der letzten Aenderung       : %li Tage\n", shadow_ptr->sp_lstchg);
	printf("Naechste Aenderung moeglich     : %li Tage\n", shadow_ptr->sp_min);
	printf("Naechste Aenderung faelllig     : %li Tage\n", shadow_ptr->sp_max);
	printf("Warnung wenn Aenderung faelllig : %li Tage vorher\n", shadow_ptr->sp_warn);
	printf("Konto nach %li Tagen sperren\n",           shadow_ptr->sp_inact);
		
		return EXIT_SUCCESS;
}

