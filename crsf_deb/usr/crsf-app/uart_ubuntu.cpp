#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <asm-generic/termbits.h>


#include "type.h"
#include "uart.h"
#include "crsf_include.h"

char tty1[20] = "/dev/ttyTHS1";
int fd1 = 0;

int setSerialOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios2 newtio,oldtio;
	if (0 != ioctl(fd, TCGETS2, &oldtio)) {
         close(fd);
         fd = -1;
	 perror("ioctl failded");
         return -1;
	}
	
    memset(&newtio,0,sizeof(newtio));
    
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
	
	switch( nBits )
    {
    case 7:
	    newtio.c_cflag |= CS7;
	    break;
    case 8:
	    newtio.c_cflag |= CS8;
	    break;
    }
	
    switch( nEvent )
    {
    case 'O':
	    newtio.c_cflag |= PARENB;
	    newtio.c_cflag |= PARODD;
	    newtio.c_iflag |= INPCK;
	    break;
    case 'E': 
	    newtio.c_cflag |= PARENB;
	    newtio.c_cflag &= ~PARODD;
	    newtio.c_iflag |= INPCK;
	    break;
    case 'N':  
	    newtio.c_cflag &= ~PARENB;
	    break;
    }
	
    switch( nSpeed )
    {
	case 2400:
	case 4800:
	case 9600:
	case 38400:
	case 57600:
	case 115200:
	    newtio.c_cflag |= CBAUDEX;
	    newtio.c_ispeed = nSpeed;
	    newtio.c_ospeed = nSpeed;
	break;
	default:
	    newtio.c_cflag |= BOTHER;
	    newtio.c_ispeed = nSpeed;
	    newtio.c_ospeed = nSpeed;
	break;
    }
	

    if( nStop == 1 )
		newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
		newtio.c_cflag |=  CSTOPB;


	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 30;
	uartflush(fd,TCIFLUSH);
	if (0 != ioctl(fd, TCSETS2, &newtio))  {
        close(fd);
        fd = -1;
	 	perror("ioctl failded");
        return -1;
     }
    return 0;
}

bool uart_init(void)
{
	struct sigaction saio; 

	fd1 = open(tty1, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if(setSerialOpt(fd1, 420000, 8, 'N', 1) == -1)
	{
		return false;
	}

	saio.sa_handler = signal_handler_IO; 

	sigemptyset(&saio.sa_mask);

	saio.sa_flags = SA_NODEFER;

	saio.sa_restorer = NULL;

	sigaction(SIGIO, &saio, NULL);

	fcntl(fd1, F_SETOWN, getpid());

	fcntl(fd1, F_SETFL, FASYNC);
	
	return true;
}

