#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/cdrom.h>
#include <errno.h>

/* gegebenfalls anpassen */
#define CDROM "/dev/cdrom"
#define FULL 0

static int open_cdrom(void) {
	int fd = open(CDROM, O_RDONLY | O_NONBLOCK);
	if (fd == -1) {
		if (errno == ENOMEDIUM)
			printf("Keine CD im Laufwerk!\n");
		else
			printf("Fehler bei open()");

		exit(EXIT_FAILURE);
	}

	return fd;
}

static void open_tray(int cdrom) {
	if (ioctl(cdrom, CDROMEJECT) == -1) {
		perror("Eject yourself\n");
		exit(EXIT_FAILURE);
	}
}

/* funktionier nicht ueberall */
static void close_tray(int cdrom) {
	if (ioctl(cdrom, CDROMCLOSETRAY) == -1) {
		perror("Close by hand\n");
	}
}

static void capability_cdrom(int cdrom) {
	const char *j[] = {"nein", "ja"};
	int caps = ioctl(cdrom, CDROM_GET_CAPABILITY);
	if (caps == -1) {
		perror("CDROM_GET_CAPABILITY");
		return;
	}

	printf("CDROM-Faehigkeiten:\n"
		"\tCD-R      : %s\n"
		"\tCD-RW     : %s\n"
		"\tDVD       : %s\n"
		"\tDVD-R     : %s\n"
		"\tDVD-RAM   : %s\n",
		j[!!(caps & CDC_CD_R)],
		j[!!(caps & CDC_CD_RW)],
		j[!!(caps & CDC_DVD)],
		j[!!(caps & CDC_DVD_R)],
		j[!!(caps & CDC_DVD_RAM)]);
}	

/* static void get_audio_status(int cdrom) { */

int main(void) {
	return 1;
}









