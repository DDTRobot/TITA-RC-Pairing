#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "type.h"
#include "crsf_include.h"

void DataReadCallBack(const uint16_t channels[])
{
  printf("CH1: %d\tCH2: %d\tCH3: %d\tCH4: %d\tCH5: %d\tCH6: %d\tCH7: %d\tCH8: %d\n",       
      channels[0],
      channels[1],
      channels[2],
      channels[3],
      channels[4],
      channels[5],
      channels[6],
      channels[7]);
  printf("\n");
}

void EnterWifiModeCallBack(const uint16_t channels[])
{
  printf("成功\n");
  printf("请打开手机设备连接wifi,手动点击升级\n");
  sleep(1);
  _exit(0);
}

void EnterBindModeCallBack(const uint16_t channels[])
{
  printf("pair success\n");
  sleep(1);
  _exit(0);
}

void EnterRestartModeCallBack(const uint16_t channels[])
{
  printf("resta\n");
  sleep(1);
  _exit(0);
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
  printf("正在检查串口通讯状态...\n");
  ElrsRestart();
  while(sleep(1));
  if(uart_is_connected())
  {
    printf("uart connect success\n");
  }
  else
  {
    printf("失败，请检查接收机接线是否正确，是否正常亮灯\n");
    return 0;
  }

  if(argv[1] != NULL)
  {
    if(strcmp(argv[1],"-wifi")==0)
    {
      printf("正在进入wifi模式...\n");
      while(sleep(3));
      ElrsEnterBind();
      while(sleep(3));
      printf("open wifi success\n");
      printf("请打开手机设备连接wifi,手动点击升级\n");
      ElrsEnterWifi();
      return 0;
    }

		if(strcmp(argv[1],"-bind")==0)
		{
      printf("正在进入配对模式...\n");
      uart_init();
      while(sleep(2))
      {
        ElrsEnterBind();
      }
      printf("bind mode success\n");
      printf("请打开遥控器,左长按右侧按键进入TOOLS->ExpressLRS->[Bind],手动搜索配对\n");
      ElrsEnterBind();
      while(sleep(1));
      uart_init();
      onDataReceived(EnterBindModeCallBack);
    }
		
    if(strcmp(argv[1],"-restart")==0)
    {
      printf("正在重启...\n");
      while(sleep(3));
      printf("restart success\n");
      ElrsRestart();
      return 0;
    }
	}

  read_data_start(0);
  return 0;
}
