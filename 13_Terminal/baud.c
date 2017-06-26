#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

typedef struct termios TTY;

typedef struct {
	speed_t speed;
	long value;
} speed_to_value;

#define SPEED_TO_VALUE(i) {B##i, i}

static const speed_to_value speed_to_values_array[] = {
	SPEED_TO_VALUE(0),
	SPEED_TO_VALUE(50),
	SPEED_TO_VALUE(75),
	SPEED_TO_VALUE(110),
	SPEED_TO_VALUE(134),
	SPEED_TO_VALUE(150),
	SPEED_TO_VALUE(200),
	SPEED_TO_VALUE(300),
	SPEED_TO_VALUE(600),
	SPEED_TO_VALUE(1200),
	SPEED_TO_VALUE(1800),
	SPEED_TO_VALUE(2400),
	SPEED_TO_VALUE(4800),
	SPEED_TO_VALUE(9600),
	SPEED_TO_VALUE(19200),
	SPEED_TO_VALUE(38400),
	SPEED_TO_VALUE(57600),
	SPEED_TO_VALUE(115200),
	SPEED_TO_VALUE(230400)
};

#define SIZE (sizeof(speed_to_values_array)/sizeof(*speed_to_values_array))

static long baudrate_wert(speed_t baud) {
	size_t i;
	for (i = 0; i <= SIZE; ++i) {
		if (baud == speed_to_values_array[i].speed)
			return speed_to_values_array[i].value;
	}

	return -1;
}


int main(int argc, char** argv) {
	int fd;
	long baud_input, baud_output;
	TTY terminal;

	if (argc != 2) {
		printf("Usage: %s Geraetepfad\n", *argv);
		exit(EXIT_FAILURE);
	}

	if ((fd = open(argv[1], O_RDWR | O_NONBLOCK)) == -1) {
		perror("fopen()");
		exit(EXIT_FAILURE);
	}

	if (isatty(fd) == 0) {
		printf("%s ist kein TTY\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (tcgetattr(fd, &terminal) == -1) {
		printf("Fehler bei tcgetattr()\n");
		exit(EXIT_FAILURE);
	}

	if ((baud_input = baudrate_wert(cfgetispeed(&terminal))) == -1) {
		printf("Fehler bei baud_input ...\n");
		exit(EXIT_FAILURE);
	}
	
	
	if ((baud_output = baudrate_wert(cfgetospeed(&terminal))) == -1) {
		printf("Fehler bei baud_output ...\n");
		exit(EXIT_FAILURE);
	}

	printf("Eingabe-Baudrate: %ld\n", baud_input);
	printf("Ausgabe-Baudrate: %ld\n", baud_output);
	
	return EXIT_SUCCESS;
}
