#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define BUF 4096

enum { CPU, DEV, DMA, INT, IOP, MEM, PCI, VERS, SCSI, EXIT };

static const char *info[] = {"/proc/cpuinfo", "/proc devices", "/proc/dma",   "/proc/interrupts", "/proc/ioports", "/proc/meminfo", "/proc/pci", "/proc/version", "/proc/scsi/scsi"};

static void get_info(const int inf) {
	FILE *fp;
	char buffer[BUF];
	size_t bytes_read;
	
	if ((fp = fopen(info[inf], "r")) == NULL) {
		perror("fopen()");
		return;
	}
	
	bytes_read = fread(buffer, 1, sizeof(buffer), fp);
	fclose(fp);
	
	if (bytes_read == 0 || bytes_read == sizeof(buffer))
		return;
	buffer[bytes_read] = '\n';
	
	printf("%s", buffer);
	printf("Weiter mit ENTER");
	getchar();
	return;
}

int main(void) {
	int auswahl;
	do {
		printf("Wozu benoetigen Sie Informationen?\n\n");
		printf("-%d- Prozessor\n", CPU);
		printf("-%d- Geraete\n", DEV);
		printf("-%d- DMA\n", DMA);
		printf("-%d- Interrupts\n", INT);
		printf("-%d- I/O-Ports\n", IOP);
		printf("-%d- Speicher\n", MEM);
		printf("-%d- PCI-Bus\n", PCI);
		printf("-%d- Version\n", VERS);
		printf("-%d- SCSI\n", SCSI);
		printf("-%d- Programmende\n", EXIT);
		printf("\nIhre Auswahl : ");
		do { scanf("%d", &auswahl); } while(getchar() != '\n');
	
		switch (auswahl) {
			case CPU : get_info(CPU);   break;
			case DEV : get_info(DEV);   break;
			case DMA : get_info(DMA);   break;
			case INT : get_info(INT);   break;
			case IOP : get_info(IOP);   break;
			case MEM : get_info(MEM);   break;
			case PCI : get_info(PCI);   break;
			case VERS: get_info(VERS);  break;
			case SCSI: get_info(SCSI);  break;
			case EXIT: printf("Bye\n"); break;
			default  : printf("Falsche Eingabe?\n");
		}
      
	} while (auswahl != EXIT);
      
      return EXIT_SUCCESS;
}

