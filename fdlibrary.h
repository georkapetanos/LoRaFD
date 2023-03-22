#ifndef SERIAL_H
#define SERIAL_H

void serial_init_virtual(char *serial_port, int *serial);
void serial_init_hardware(char *serial_port, int *serial);
void serial_rx(int serial, char *data, int *size);
void serial_tx(int serial, char *data, int size);
void serial_close(int *serial);

#endif
