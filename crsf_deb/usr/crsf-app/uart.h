#ifndef UART_LINUX_H
#define UART_LINUX_H

#include "type.h"

int uart_set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);

int uart_read_uint8(uint8* data);

void signal_handler_IO (int status); /* definition of signal handler */

void signal_handler_init(void);

void uart_set_init(void);

#endif