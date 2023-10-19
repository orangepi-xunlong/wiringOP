#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int OrangePiModel;
extern void sunxi_pwm_set_enable(int en);

typedef struct {
	unsigned int arr;
	unsigned int div;
	unsigned int ccr;
} pwm_info;

static pwm_info pwm_info_t;

static void set_pwm_info(int OrangePiModel)
{
	switch (OrangePiModel)
	{
		case PI_MODEL_ZERO_2:
		case PI_MODEL_ZERO_2_W:
			pwm_info_t.arr = 1024;
			pwm_info_t.div = 1;
			pwm_info_t.ccr = 512;
			break;
		case PI_MODEL_3_PLUS:
			pwm_info_t.arr = 1000;
			pwm_info_t.div = 120;
			pwm_info_t.ccr = 500;
			break;
		default:
			break;
	}
}

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

	if (pin != 3 && pin != 4 && pin != 21 && pin != 22 && pin != 2 && pin != 9 && pin != 16) {
		fprintf (stderr, "the pin you choose doesn't support hardware PWM\n") ;
		exit (1) ;
	}

	// 初始化
	printf("wiringPiSetup start\n");

	wiringPiSetup();
	pinMode(pin,PWM_OUTPUT);
	set_pwm_info(OrangePiModel);

	printf("wiringPiSetup end\n");

	// 开始测试
	while(1) {
		pwmSetRange(pin,pwm_info_t.arr);
		pwmSetClock(pin,pwm_info_t.div);
		pwmWrite(pin,pwm_info_t.ccr);

		//1. 调节PWM占空比
		//1.1 通过设置ARR调节PWM占空比
		printf("Modified ARR test start\n");

		for (i = 0 ; i <= 8 ; i++) {
			pwmSetRange(pin,pwm_info_t.arr + i * (pwm_info_t.arr / 8));
			delay(500);
		}

		delay(5000);

		for (i = 7 ; i >= 0 ; i-- ) {
			pwmSetRange(pin,pwm_info_t.arr + i * (pwm_info_t.arr / 8));
			delay(500);
		}

		delay(5000);

		printf("Modified ARR test end\n");

		sunxi_pwm_set_enable(0);
		delay(5000);
		sunxi_pwm_set_enable(1);

		//1.2 通过设置CCR调节PWM占空比
		printf("Modified CCR test start\n");

		for (i = 0 ; i <= 8 ; i++) {
			pwmWrite(pin,pwm_info_t.ccr + i * (pwm_info_t.ccr / 8));
			delay(500);
		}

		delay(5000);

		for (i = 7 ; i >= 0 ; i-- ) {
			pwmWrite(pin,pwm_info_t.ccr + i * (pwm_info_t.ccr / 8));
			delay(500);
		}

		delay(5000);

		printf("Modified CCR test end\n");

		sunxi_pwm_set_enable(0);
		delay(5000);
		sunxi_pwm_set_enable(1);

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

		sunxi_pwm_set_enable(0);
		delay(5000);
		sunxi_pwm_set_enable(1);

		//2.2 直接设置PWM频率
		printf("Modified PWM frequency test start\n");

		for (i = 1 ; i <= 10; i++) {
			pwmToneWrite(pin,2000 * i);
			delay(2000);
		}

		delay(5000);

		printf("Modified PWM frequency test end\n");

		sunxi_pwm_set_enable(0);
		delay(5000);
		sunxi_pwm_set_enable(1);
	}
}
