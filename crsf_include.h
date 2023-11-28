#ifndef CRSF_INCLUDE_H
#define CRSF_INCLUDE_H

typedef struct
{
  uint8_t result;
}taskData;

extern int fd1;
extern taskData crsf_app;

void read_data_start(uint8 mode);

void onDataReceived(void (*pCallback)(const uint16_t channels[]));

bool uart_init(void);

bool uart_is_connected(void);

bool elrs_is_binding(void);

void uartflush(int fildes, int queue_selector);
#endif