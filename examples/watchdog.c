#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <sys/signal.h>
#include <termios.h>

struct watchdog_info{
    unsigned int options;   //options the card/driver supprots 19        
    unsigned int firmware_version;  //firmcard version of the card
    unsigned char identity[32];     //identity of the board 21
 };

#define WATCHDOG_IOCTL_BASE 'W'
#define WDIOC_GETSUPPORT _IOR(WATCHDOG_IOCTL_BASE, 0, struct watchdog_info)
#define WDIOC_SETTIMEOUT _IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define WDIOC_GETTIMEOUT _IOR(WATCHDOG_IOCTL_BASE, 7, int) //27
#define WDIOS_DISABLECARD 0x0001       
#define WDIOS_ENABLECARD 0x0002
#define WDIOC_SETOPTIONS _IOR(WATCHDOG_IOCTL_BASE, 4, int)
#define WDIOC_KEEPALIVE _IOR(WATCHDOG_IOCTL_BASE, 5, int)

int Getch (void)   //无回显的从屏幕输入字符，来达到喂狗的目的

{

     int ch;
     struct termios oldt, newt;   //终端设备结构体
     tcgetattr(STDIN_FILENO, &oldt);   //获得终端属性
     newt = oldt;
     newt.c_lflag &= ~(ECHO|ICANON);   //设置无回显属性
     tcsetattr(STDIN_FILENO, TCSANOW, &newt);  //设置新的终端属性
     ch = getchar();   //从键盘输入一个数据
     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  //恢复终端设备初始设置
     return ch;

}
 //suspend some seconds
int zsleep(int millisecond)

{
     unsigned long usec;
     usec=1000*millisecond;
     usleep(usec); //usleep(1)睡眠一微秒（10E-6),这里也就是0.1s
}
int Init()
{
     int fd;
     //open device file
     fd = open("/dev/watchdog",O_RDWR);   //打开看门狗设备
      if(fd < 0)
     {
         printf("device open fail\n");
         return -1;
     }
     printf("open success\n");
     return fd;
}

int main(int argc,char **argv)
{
     int fd,ch;
     int i,j;
     char c;
     struct watchdog_info wi;
	 if(argc != 2){
		 printf("Usage : ./watchdog 10\n");
		 return -1;
	 }
     fd=Init();  //打开终端看门狗设备
	 ioctl(fd, WDIOC_SETOPTIONS, WDIOS_ENABLECARD); 
     //读板卡信息，但不常用
     ioctl(fd,WDIOC_GETSUPPORT,&wi);
     printf("options is %d,identity is %s\n",wi.options,wi.identity);
     //读看门狗溢出时间

     printf("put_usr return,if 0,success:%d\n",ioctl(fd,WDIOC_GETTIMEOUT,&i));
	
	 printf("The old reset time is: %d\n", i);
     //关闭
      i=WDIOS_DISABLECARD;//WDIOC_SETOPTIONS=0X0001
     printf("return ENOTTY,if -1,success:%d\n",ioctl(fd,WDIOC_SETOPTIONS,&i));
     //打开
      i=WDIOS_ENABLECARD;//WDIOS_ENABLECARD 0x0002
     printf("return ENOTTY,if -1,success:%d\n",ioctl(fd,WDIOC_SETOPTIONS,&i));
     i=atoi(argv[1]);
     printf("put_user return,if 0,success:%d\n",ioctl(fd,WDIOC_SETTIMEOUT,&i));
     //读新的设置时间

     printf("put_usr return,if 0,success:%d\n",ioctl(fd,WDIOC_GETTIMEOUT,&i));


     while(1)
     {
           zsleep(100);
           if((c=Getch())!=27){
                //输入如果不是ESC，就喂狗，否则不喂狗，到时间后系统重启
        printf("keep alive \n");
                ioctl(fd,WDIOC_KEEPALIVE,NULL);
                //write(fd,NULL,1);     //同样是喂狗

           }
     }
    close(fd);   //关闭设备
     return 0;
}
