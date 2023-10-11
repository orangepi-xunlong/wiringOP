#include<wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv [])
{
	int i = 0;
	int val = 0;
	int pin= 0;

	if (argc != 2) {
		fprintf (stderr, "Usage: %s <pin>\n", argv [0]) ;
		exit (1) ;
	}

	pin = (unsigned int)strtoul (argv [1], NULL, 10) ;

	if (pin != 3 && pin != 4 && pin != 21 && pin != 22) {
		fprintf (stderr, "pin 3 for PWM2 / pin 4 for PWM1 / pin 21 for PWM3 / pin 22 for PWM4\n") ;
		exit (1) ;
	}

	// 初始化
	printf("wiringPiSetup start\n");

	wiringPiSetup();
	pinMode(pin,PWM_OUTPUT);

	printf("wiringPiSetup end\n");

	// 开始测试
	while(1)
	{
		pwmSetRange(pin,1024);
		pwmSetClock(pin,1);
		pwmWrite(pin,512);

		//1. 调节PWM占空比
		//1.1 通过设置ARR调节PWM占空比
		printf("Modified ARR test start\n");

		for (i = 0 ; i <= 8 ; i++) {
			pwmSetRange(pin,1024+i*128);
			delay(500);
		}

		delay(5000);

		for (i = 7 ; i >= 0 ; i-- ) {
			pwmSetRange(pin,1024+i*128);
			delay(500);
		}

		delay(5000);

		printf("Modified ARR test end\n");

		pwmSetMode(pin,PWM_MODE_BAL);
		delay(5000);
		pwmSetMode(pin,PWM_MODE_MS);

		//1.2 通过设置CRR调节PWM占空比
		printf("Modified CRR test start\n");

		for (i = 0 ; i <= 8 ; i++) {
			pwmWrite(pin,512 + i*64);
			delay(500);
		}

		delay(5000);

		for (i = 7 ; i >= 0 ; i-- ) {
			pwmWrite(pin,512 + i*64);
			delay(500);
		}

		delay(5000);

		printf("Modified active range test end\n");

		pwmSetMode(pin,PWM_MODE_BAL);
		delay(5000);
		pwmSetMode(pin,PWM_MODE_MS);

		//2.调节PWM频率
		//2.1通过设置分频系数调节PWM频率

		printf("Modified frequency division test start\n");

		for (i = 1 ; i <= 10; i++) {
			pwmSetClock(pin,i);
			delay(500);
		}

		delay(5000);

		for (i = 9 ; i >= 1 ; i--) {
			pwmSetClock(pin,i);
			delay(500);
		}

		delay(5000);

		printf("Modified frequency division test end\n");

		pwmSetMode(pin,PWM_MODE_BAL);
		delay(5000);
		pwmSetMode(pin,PWM_MODE_MS);

		//2.2 直接设置PWM频率
		printf("Modified PWM frequency test start\n");

		for (i = 1 ; i <= 10; i++) {
			pwmToneWrite(pin,2000*i);
			delay(2000);
		}

		delay(5000);

		printf("Modified PWM frequency test end\n");

		pwmSetMode(pin,PWM_MODE_BAL);
		delay(5000);
		pwmSetMode(pin,PWM_MODE_MS);
	}
	return 0;
}
