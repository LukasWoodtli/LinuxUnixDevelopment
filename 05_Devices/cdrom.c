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

static void play_cdrom(int cdrom, int flag) {
    struct cdrom_tocentry tocentry;
    struct cdrom_msf play;
    int track = flag;
    int lead = flag;
    
    if (flag == FULL) {
        track = 1;
        lead = CDROM_LEADOUT;
    }
    else {
        track = flag;
        lead = flag + 1;
    }
    
    /* beginning of first song */
    tocentry.cdte_track = track;
    tocentry.cdte_format = CDROM_MSF;
    if (ioctl(cdrom, CDROMREADTOCENTRY, &tocentry) == -1) {
        perror("Kann Inhalt der CD nicht ermitteln\n");
        exit(EXIT_FAILURE);
    }
    
    play.cdmsf_min0 = tocentry.cdte_addr.msf.minute;
    play.cdmsf_sec0 = tocentry.cdte_addr.msf.second;
    play.cdmsf_frame0 = tocentry.cdte_addr.msf.frame;
    
    /* end of last song */
    tocentry.cdte_track = lead;
    tocentry.cdte_format = CDROM_MSF;
    if (ioctl(cdrom, CDROMREADTOCENTRY, &tocentry) == -1) {
        perror("Kann Inhalt der CD nicht ermitteln\n");
        exit(EXIT_FAILURE);
    }
    
    play.cdmsf_min1 = tocentry.cdte_addr.msf.minute;
    play.cdmsf_sec1 = tocentry.cdte_addr.msf.second;
    play.cdmsf_frame1 = tocentry.cdte_addr.msf.frame;
    
    if (ioctl(cdrom, CDROMPLAYMSF, &play) == -1) {
        perror("Kann CD nicht abspielen\n");
        exit(EXIT_FAILURE);
    }
}

static void stop_cdrom(int cdrom) {
    if (ioctl(cdrom, CDROMSTOP) == -1) {
        perror("Kann CD nicht anhalten\n");
    }
}

static void pause_cdrom(int cdrom) {
    if (ioctl(cdrom, CDROMPAUSE) == -1) {
        perror("Kann CD nicht pausieren\n");
    }
}


static void resume_cdrom(int cdrom) {
    if (ioctl(cdrom, CDROMRESUME) == -1) {
        perror("Kann CD nicht fortsetzen\n");
    }
}

int main(void) {
    int fd_cdrom;
    int select;
    int track;
    
    fd_cdrom = open_cdrom();
    
    do {
        printf("-1- CD-Tray oeffnen\n");
        printf("-2- CD-Tray schliessen\n");
        printf("-3- CDROM-Faehigkeiten\n");
        printf("-4- Audio-CD abspielen (komplett)\n");
        printf("-5- Einzelnen Track abspielen\n");
        printf("-6- <PAUSE>\n");
        printf("-7- <FORTFAHREN>\n");
        printf("-8- <STOP>\n");
        printf("-9- Aktuellen Status ermitteln\n");
        printf("-10- CD-Inhalt ausgeben\n");
        printf("-11- Programmende\n");
        
        printf("Auswahl: ");
        scanf("%d", &select);
        
        switch (select) {
            case 1: open_tray(fd_cdrom); break;
            case 2: close_tray(fd_cdrom); break;
            case 3: capability_cdrom(fd_cdrom); break;
            case 4: play_cdrom(fd_cdrom, FULL); break;
            case 5:
                printf("Welchen Track: ");
                scanf("%d", &track);
                play_cdrom(fd_cdrom, track);
                break;
            case 6: pause_cdrom(fd_cdrom); break;
            case 7: resume_cdrom(fd_cdrom); break;
            case 8: stop_cdrom(fd_cdrom); break;
            case 9: get_audio_status(fd_cdrom); break;
            case 10: content_cdrom(fd_cdrom); break;
            case 11: printf("Bye!\n"); break;
            default: printf("Was?");
        }
        
    } while (select != 11);
    
    stop_cdrom(fd_cdrom);
    close(fd_cdrom);

    return 0;
}
