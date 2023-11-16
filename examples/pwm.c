#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
	unsigned int ccr;
	unsigned int arr;
	unsigned int div;
	unsigned int div_stepping;
} pwm_info;

static pwm_info pwm_info_t;

static void set_pwm_info(int pin)
{
	int model;

	piBoardId (&model);

	switch (model)
	{
		case PI_MODEL_ZERO_2:

			if (pin != 3 && pin != 4 && pin != 21 && pin != 22) {
				fprintf (stderr, "the pin you choose doesn't support hardware PWM\n") ;
				exit(1);
			}

			pwm_info_t.ccr = 512;
			pwm_info_t.arr = 1024;
			pwm_info_t.div = 1;
			pwm_info_t.div_stepping = 1;
			break;

		case PI_MODEL_ZERO_2_W:

			if (pin != 2 && pin != 9 && pin != 21 && pin != 22) {
				fprintf (stderr, "the pin you choose doesn't support hardware PWM\n") ;
				exit(1);
			}

			pwm_info_t.ccr = 512;
			pwm_info_t.arr = 1024;
			pwm_info_t.div = 1;
			pwm_info_t.div_stepping = 1;
			break;

		case PI_MODEL_3_PLUS:

			if (pin != 2 && pin != 16) {
				fprintf (stderr, "the pin you choose doesn't support hardware PWM\n") ;
				exit(1);
			}

			pwm_info_t.ccr = 500;
			pwm_info_t.arr = 1000;
			pwm_info_t.div = 120;
			pwm_info_t.div_stepping = 1;
			break;

		case PI_MODEL_5:

			if (pin != 0 && pin != 2 && pin != 5 && pin != 8 && pin != 9 && pin != 10 && pin != 14 && pin != 16) {
				fprintf (stderr, "the pin you choose doesn't support hardware PWM\n") ;
				exit(1);
			}

			pwm_info_t.ccr = 500;
			pwm_info_t.arr = 1000;
			pwm_info_t.div = 120;
			pwm_info_t.div_stepping = 2;
			break;

		case PI_MODEL_5B:

			if (pin != 0 && pin != 2 && pin != 5 && pin != 8 && pin != 9 && pin != 10 && pin != 13 && pin != 15) {
				fprintf (stderr, "the pin you choose doesn't support hardware PWM\n") ;
				exit(1);
			}

			pwm_info_t.ccr = 500;
			pwm_info_t.arr = 1000;
			pwm_info_t.div = 120;
			pwm_info_t.div_stepping = 2;
			break;

		case PI_MODEL_5_PLUS:

			if (pin != 0 && pin != 1 && pin != 2 && pin != 6 && pin != 9 && pin != 10 && pin != 13 && pin != 21 && pin != 22) {
				fprintf (stderr, "the pin you choose doesn't support hardware PWM\n") ;
				exit(1);
			}

			pwm_info_t.ccr = 500;
			pwm_info_t.arr = 1000;
			pwm_info_t.div = 120;
			pwm_info_t.div_stepping = 2;
			break;

		case PI_MODEL_CM4:
		case PI_MODEL_3B:

			if (pin != 2 && pin != 21) {
				fprintf (stderr, "the pin you choose doesn't support hardware PWM\n") ;
				exit(1);
			}

			pwm_info_t.ccr = 500;
			pwm_info_t.arr = 1000;
			pwm_info_t.div = 120;
			pwm_info_t.div_stepping = 2;
			break;

		default:
			printf("Oops - unable to determine board type...");
			exit(1);
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

	// 初始化
	printf("wiringPiSetup start\n");

	wiringPiSetup();
	set_pwm_info(pin);
	pinMode(pin,PWM_OUTPUT);

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

		//2.调节PWM频率
		//2.1通过设置分频系数调节PWM频率

		printf("Modified frequency division test start\n");

		for (i = pwm_info_t.div_stepping ; i <= 10 * pwm_info_t.div_stepping; i += pwm_info_t.div_stepping) {
			pwmSetClock(pin,i);
			delay(500);
		}

		delay(5000);

		for (i = 9 * pwm_info_t.div_stepping; i >= pwm_info_t.div_stepping ; i -= pwm_info_t.div_stepping) {
			pwmSetClock(pin,i);
			delay(500);
		}

		delay(5000);

		printf("Modified frequency division test end\n");

		//2.2 直接设置PWM频率
		printf("Modified PWM frequency test start\n");

		for (i = 1 ; i <= 10; i++) {
			pwmToneWrite(pin,2000 * i);
			delay(2000);
		}

		delay(5000);

		printf("Modified PWM frequency test end\n");
	}
}
