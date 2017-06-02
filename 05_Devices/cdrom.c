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

static void get_audio_status(int cdrom) { 
	struct cdrom_subchnl sub;
	printf("Audio Status: ");
	fflush(stdout);
	sub.cdsc_format = CDROM_MSF;

	if (ioctl(cdrom, CDROMSUBCHNL, sub)) {
		printf("FAILED!\n");
	}
	else {
		switch (sub.cdsc_audiostatus) {
		case CDROM_AUDIO_INVALID:
			printf("Invalid\n");
			break;
		case CDROM_AUDIO_PLAY:
			printf("Playing\n");
			break;
		case CDROM_AUDIO_PAUSED:
			printf("Pause\n");
			break;
		case CDROM_AUDIO_COMPLETED:
			printf("Completed\n");
			break;
		case CDROM_AUDIO_ERROR:
			printf("Error\n");
			break;
		case CDROM_AUDIO_NO_STATUS:
			printf("No status\n");
			break;
		default:
			printf("Oops: unknown\n");
		}

		if (sub.cdsc_audiostatus == CDROM_AUDIO_PLAY ||		
		    sub.cdsc_audiostatus == CDROM_AUDIO_PAUSED) {
			printf("At: %02d:%02d abs/  %02d:%02d track %d\n",
				sub.cdsc_absaddr.msf.minute,	
				sub.cdsc_absaddr.msf.second,		
				sub.cdsc_reladdr.msf.minute,
				sub.cdsc_reladdr.msf.second,
				sub.cdsc_trk);

		}
	}
}

static void content_cdrom(int cdrom) {
	struct cdrom_tochdr tochdr;
	struct cdrom_tocentry tocentry;
	int track;

	if (ioctl(cdrom, CDROMREADTOCHDR, &tochdr) == -1) {
		perror("Kann den Header nicht holen");
		exit(EXIT_FAILURE);
	}

	printf("\nInhalt %d Tracks:\n", tochdr.cdth_trk1);
	track = tochdr.cdth_trk0;
	while (track <= tochdr.cdth_trk1) {
		tocentry.cdte_track = track;
		tocentry.cdte_format = CDROM_MSF;
		if (ioctl(cdrom, CDROMREADTOCENTRY, &tocentry) == -1) {
			perror("Kann den Inhalt der CD nicht ermitteln");
			exit(EXIT_FAILURE);
		}

		printf("%3d: %02d:%02d:%02d (%06d, %06d) %s%s\n",
			tocentry.cdte_track,
			tocentry.cdte_addr.msf.minute,
			tocentry.cdte_addr.msf.second,
			tocentry.cdte_addr.msf.frame,
			tocentry.cdte_addr.msf.frame +
			tocentry.cdte_addr.msf.second * 75,
			tocentry.cdte_addr.msf.minute * 75 * 60 - 150,
			(tocentry.cdte_ctrl & CDROM_DATA_TRACK) ?
			"data" : "audio",
			CDROM_LEADOUT == track ? " (leadout)" : "");
		track++;
	}
}

int main(void) {
	return 1;
}









