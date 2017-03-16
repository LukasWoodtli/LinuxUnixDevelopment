#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);

static sighandler_t my_signal(int sig_nr, sighandler_t signalhandler) {
	struct sigaction neu_sig, alt_sig;

	neu_sig.sa_handler = signalhandler;
	sigemptyset(&neu_sig.sa_mask);
	neu_sig.sa_flags = SA_RESTART;

	if (sigaction(sig_nr, &neu_sig, &alt_sig) < 0)
		return SIG_ERR;

	return alt_sig.sa_handler;
}

static sighandler_t signal_add(int sig_nr, sighandler_t signalhandler) {
	struct sigaction add_sig;

	if (sigaction(sig_nr, NULL, &add_sig) < 0)
		return SIG_ERR;

	add_sig.sa_handler = signalhandler;
	sigaddset(&add_sig.sa_mask, sig_nr);
	add_sig.sa_flags = SA_RESTART;
	
	if (sigaction(sig_nr, &add_sig, NULL) < 0)
		return SIG_ERR;

	return add_sig.sa_handler;
}

static sighandler_t signal_del(int sig_nr) {
	struct sigaction del_sig;

	if (sigaction(sig_nr, NULL, &del_sig) < 0)
		return SIG_ERR;

	del_sig.sa_handler = SIG_DFL;
	sigdelset(&del_sig.sa_mask, sig_nr);
	del_sig.sa_flags = SA_RESTART;
	
	if (sigaction(sig_nr, &del_sig, NULL) < 0)
		return SIG_ERR;

	return del_sig.sa_handler;
}


static void check_signal(int sig_nr) {
	struct sigaction check_sig;

	if (sigaction(sig_nr, NULL, &check_sig) < 0)
		return;

	printf("Aktion beim Auftreten von Signal %d: ", sig_nr);
	if (check_sig.sa_handler == SIG_DFL)
		printf("Standardaktion (default)\n");
	else if (check_sig.sa_handler == SIG_IGN)
		printf("Wird ignoriert\n");
	else
		printf("Signalhandler wird aufgerufeb\n");
}


static void check_sig(int signr) {
	int nr;
	if (signr == SIGINT) {
		printf("SIGINT ausgeloest\n");
		printf("- 1 - Signal aus Maske entfernen\n");
		printf("- 2 - Signal ignorieren\n");
		printf("- 3 - SIGALARM in 5 Sek ausloesen\n");
		printf("- 4 - Programm beenden\n");
		printf("Ihre Wahl: ");
		scanf("%d", &nr);
		
		switch (nr) {
			case 1:
				signal_del(SIGINT);
				printf("SIGINT wurde entfernt\n");
				break;
			case 2:
				return;
			case 3:
				alarm(5);
				break;
			case 4:
				raise(SIGKILL);
				break;
			default:
				printf("Unbekannte Eingabe!\n");
				break;
		}
	}
	else if (signr == SIGALRM) {
		printf("SIGALRM ausgeloest\n");
		raise(SIGKILL);
	}
}


int main(void) {
	my_signal(SIGINT, check_sig);
	signal_add(SIGALRM, check_sig);
	signal_add(SIGABRT, SIG_IGN);
	
	check_signal(SIGINT);
	check_signal(SIGALRM);
	check_signal(SIGABRT);
	check_signal(SIGFPE);
	printf("<ctrl>-<c> druecken");
	fflush(stdout);
	while(1);
	return EXIT_SUCCESS;
}
