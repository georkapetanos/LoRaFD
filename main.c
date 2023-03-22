#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "fdlibrary.h"
#define SERIAL_BUFFER 512

struct arg_struct {
    int serial_virtual;
    int serial_hardware;
} args;

unsigned int lock;

//read block to virtual, write to hardware thread
void *virtual2hardware(void *arguments) {
	struct arg_struct *args = arguments;
	int size, bytes = 0;
	char data[SERIAL_BUFFER];

	//printf("v2f: %d, %d\n", args->serial_virtual, args->serial_hardware);
	while(1) {
		serial_rx(args->serial_virtual, data, &size);
		while(1) {
			ioctl(args->serial_hardware, FIONREAD, &bytes);
			//printf("bytes = %d\n", bytes);
			if((bytes == 0) && (lock == 0)) {
				serial_tx(args->serial_hardware, data, size);
				break;
			}
			else {
				printf("bytes = %d, lock = %d\n", bytes, lock);
				printf("sleeping\n");
				usleep(500000);
				printf("sleeped\n");
			}
		}
	}
	
	return NULL;
}

//read block to hardware, write to virtual thread
void *hardware2virtual(void *arguments) {
	struct arg_struct *args = arguments;
	int size, bytes = 0;
	char data[SERIAL_BUFFER];

	//printf("f2v: %d, %d\n", args->serial_virtual, args->serial_hardware);
	while(1) {
		serial_rx(args->serial_hardware, data, &size);
		lock = 1;
		serial_tx(args->serial_virtual, data, size);
		ioctl(args->serial_hardware, FIONREAD, &bytes);
		if(bytes == 0) {
			lock = 0;
		}
	}
	
	return NULL;
}

int main(int argc, char *argv[]) {
	//int serial_virtual, serial_hardware;
	pthread_t thread_id_v2h, thread_id_h2v;
	struct arg_struct *args = malloc(sizeof(struct arg_struct));
	
	lock = 0;
	
	if(argc < 3) {
		printf("Pass tty arguments: ./lorafd /dev/tty* /dev/tty*\n");
		return 0;
	}

	serial_init_virtual(argv[1], &args->serial_virtual);
	serial_init_hardware(argv[2], &args->serial_hardware);

	pthread_create(&thread_id_v2h, NULL, virtual2hardware, args);
	pthread_create(&thread_id_h2v, NULL, hardware2virtual, args);
	pthread_join(thread_id_h2v, NULL);

	serial_close(&args->serial_virtual);
	serial_close(&args->serial_hardware);

	return 0;
}
