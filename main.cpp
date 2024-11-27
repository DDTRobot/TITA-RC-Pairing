#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "type.h"
#include "crsf_include.h"

taskData crsf_app;

void DataReadCallBack(const uint16_t channels[])
{
  printf("CH1: %d\tCH2: %d\tCH3: %d\tCH4: %d\tCH5: %d\tCH6: %d\tCH7: %d\tCH8: %d\tCH9: %d\tCH10: %d\tCH11: %d\tCH12: %d\tCH13: %d\tCH14: %d\tCH15: %d\tCH16: %d\t\n",       
      channels[0],
      channels[1],
      channels[2],
      channels[3],
      channels[4],
      channels[5],
      channels[6],
      channels[7],
      channels[8],
      channels[9],
      channels[10],
      channels[11],
      channels[12],
      channels[13],
      channels[14],
      channels[15]);
  printf("\n");
}

void EnterWifiModeCallBack(const uint16_t channels[])
{
  printf("Success\n");
  printf("Please connect wifi and upgrade\n");
  sleep(1);
  _exit(0);
}

void EnterBindModeCallBack(const uint16_t channels[])
{
  fflush(stdout); 
  printf("pair success\n");
  crsf_app.result = 1;
}

void EnterRestartModeCallBack(const uint16_t channels[])
{
  printf("restart success\n");
  crsf_app.result = 1;
}

void ElrsRestart(void)
{
  uint8 data_t[2] = {0x31,0x05};
  write(fd1,data_t,2);
}

void ElrsEnterBind(void)
{
  uint8 data_t[2] = {0x31,0x02};
  write(fd1,data_t,2);
}

void ElrsEnterWifi(void)
{
  uint8 data_t[2] = {0x31,0x01};
  write(fd1,data_t,2);
}

int main(int argc, char ** argv)
{
  onDataReceived(DataReadCallBack);
  uart_init();
  printf("Check uart status...\n");
  ElrsRestart();
  while(sleep(1));
  if(uart_is_connected())
  {
    fflush(stdout); 
    printf("Uart connect success\n");
  }
  else
  {
    printf("Uart fail\n");
    return 0;
  }

  if(argv[1] != NULL)
  {
    if(strcmp(argv[1],"-wifi")==0)
    {
      printf("Entering wifi...\n");
      uart_init();
      while(sleep(2))
      {
        ElrsEnterWifi();
      }
      fflush(stdout); 
      printf("Open wifi success\n");
      printf("Please connect wifi and upgrade\n");
      return 0;
    }

		if(strcmp(argv[1],"-bind")==0)
		{
      printf("Entering bind mode...\n");
      uart_init();
      while(sleep(2))
      {
        ElrsEnterBind();
      }
      fflush(stdout); 
      fflush(stdout); 
      printf("Bind mode success\n");
      printf("Entering TOOLS->ExpressLRS->[Bind] and binding in remote control\n");
      ElrsEnterBind();
      while(sleep(1));
      uart_init();
      onDataReceived(EnterBindModeCallBack);
    }
		
    if(strcmp(argv[1],"-restart")==0)
    {
      printf("Restarting...\n");
      while(sleep(3));
      fflush(stdout); 
      printf("Restart success\n");
      ElrsRestart();
      return 0;
    }
	}

  read_data_start(0);
  return 0;
}
