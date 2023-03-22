#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

void serial_init_virtual(char *serial_port, int *serial) {
	*serial = open(serial_port, O_RDWR);
	struct termios tty;
	
	if (*serial < 0) {
		printf("Error opening %s\n", serial_port);
  		exit(1);
	}
	
	// Read existing settings
	if(tcgetattr(*serial, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}
	
	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication
	tty.c_cflag &= ~CSIZE; // Clear all the size bits
	tty.c_cflag |= CS8; // 8 bits per byte
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes ( newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	tty.c_cc[VTIME] = 0;  // Wait for up to 1s (10 deciseconds)
	tty.c_cc[VMIN] = 1;	   //block until at least 1 byte received
	// Set in/out baud rate to be 9600
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	// Save new tty settings
	if (tcsetattr(*serial, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}
}

void serial_init_hardware(char *serial_port, int *serial) {
	*serial = open(serial_port, O_RDWR);
	struct termios tty;
	
	if (*serial < 0) {
		printf("Error opening %s\n", serial_port);
  		exit(1);
	}
	
	// Read existing settings
	if(tcgetattr(*serial, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}
	
	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication
	tty.c_cflag &= ~CSIZE; // Clear all the size bits
	tty.c_cflag |= CS8; // 8 bits per byte
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes ( newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	tty.c_cc[VTIME] = 0;  // Wait for up to 1s (10 deciseconds)
	tty.c_cc[VMIN] = 1;	   //block until at least 1 byte received
	// Set in/out baud rate to be 9600
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	// Save new tty settings
	if (tcsetattr(*serial, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}
}

void serial_rx(int serial, char *data, int *size) {
	*size = read(serial, data, 512);
}

void serial_tx(int serial, char *data, int size) {
	write(serial, data, size);
}

void serial_close(int *serial) {
	close(*serial);
}
