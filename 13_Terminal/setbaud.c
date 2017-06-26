#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>

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



// R. W. Stevens-like error handling
static void fehler_exit(char *error_msg, ...) {
    va_list az;
    va_start(az, error_msg);
    vprintf(error_msg, az);
    va_end(az);
    exit(EXIT_FAILURE);
}


int main(int argc, char** argv) {
	int fd;
	long baud_input, baud_output;
	TTY terminal;

	if (argc != 2) {
		printf("Usage: %s Geraetepfad\n", *argv);
		exit(EXIT_FAILURE);
	}

	if ((fd = open(argv[1], O_RDWR | O_NONBLOCK)) == -1)
		fehler_exit("Fehler bei fopen() kann %s nicht oeffnen", argv[1]);

	if (isatty(fd) == 0)
		fehler_exit("%s ist kein TTY\n", argv[1]);

	if (tcgetattr(fd, &terminal) == -1)
		fehler_exit("Fehler bei tcgetattr()\n");

	if ((baud_input = baudrate_wert(cfgetispeed(&terminal))) == -1)
		fehler_exit("Fehler bei baud_input ...\n");
	if ((baud_output = baudrate_wert(cfgetospeed(&terminal))) == -1)
		fehler_exit("Fehler bei baud_output ...\n");

	printf("Eingabe-Baudrate: %ld\n", baud_input);
	printf("Ausgabe-Baudrate: %ld\n", baud_output);

    
    if ((cfsetispeed(&terminal, B115200)) == -1)
        fehler_exit("Fehler beim Setzen von Eingabe-Baudrate ...\n");
    if ((cfsetospeed(&terminal, B115200)) == -1)
        fehler_exit("Fehler beim Setzen von Ausgabe-Baudrate ...\n");
    
    if (tcsetattr(fd, TCSAFLUSH, &terminal) == -1)
        fehler_exit("Fehler bei tcsetattr ...\n");
    if (tcgetattr(fd, &terminal) == -1)
        fehler_exit("Fehler bei tcgetattr ...\n");
    
    if ((baud_input = baudrate_wert(cfgetispeed(&terminal))) == -1)
        fehler_exit("Fehler bei baud_input ...\n");
    if ((baud_output = baudrate_wert(cfgetospeed(&terminal))) == -1)
        fehler_exit("Fehler bei baud_output ...\n");
    
    printf("Eingabe-Baudrate: %ld\n", baud_input);
    printf("Ausgabe-Baudrate: %ld\n", baud_output);

	return EXIT_SUCCESS;
}
