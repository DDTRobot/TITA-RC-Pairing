#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

#include "uart-crsf.h"
#include "uart.h"
#include "crsf_include.h"

CRSF crsf;
int wait_flag = 1;
extern int fd1;

#define UART_BUFFER (512)

void onDataReceived(void (*pCallback)(const uint16_t channels[]))
{
  crsf.onDataReceived(pCallback);
}

bool uart_is_connected(void)
{
  return crsf.isUartConnected();
}

bool elrs_is_binding(void)
{
  return crsf.is_binding;
}

void read_data_start(uint8 mode)
{
  uint8 buf[UART_BUFFER];
  if(mode == 0)
  {
    while (crsf_app.result == 0)
    {
      if (wait_flag == 0)
      {
        memset(buf, 0, UART_BUFFER);
        int RxLen = read(fd1, buf, UART_BUFFER);

        if (RxLen >= CRSF_PACKET_SIZE)
        {
          crsf.readPacket(buf,CRSF_PACKET_SIZE);
        }
        wait_flag = 1;
      }
    }
    close(fd1);
  }
  else
  {
    if (wait_flag == 0)
    {
      memset(buf, 0, UART_BUFFER);
      int RxLen = read(fd1, buf, UART_BUFFER);

      if (RxLen >= CRSF_PACKET_SIZE)
      {
        crsf.readPacket(buf,CRSF_PACKET_SIZE);
      }
      wait_flag = 1;
    }
  }
}

void signal_handler_IO(int status)
{
  crsf.setUartConnected(true);
  wait_flag = 0;
}

void uartflush(int fildes, int queue_selector)
{
  tcflush(fildes, queue_selector);
}
