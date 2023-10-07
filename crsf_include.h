#ifndef CRSF_INCLUDE_H
#define CRSF_INCLUDE_H

extern int fd1;

void read_data_start(void);

void onDataReceived(void (*pCallback)(const uint16_t channels[]));

bool uart_init(void);

bool uart_is_connected(void);

void uartflush(int fildes, int queue_selector);
#endif