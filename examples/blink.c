#include <stdio.h>
#include <wiringPi.h>

unsigned char getGpioNum(void)
{
	int model = -1;

	piBoardId (&model);

	switch (model)
	{
		case PI_MODEL_5B:
			return 16;
			break;
		case PI_MODEL_3:
		case PI_MODEL_LTIE_2:
		case PI_MODEL_ZERO:
		case PI_MODEL_ZERO_PLUS_2:
		case PI_MODEL_ZERO_PLUS:
		case PI_MODEL_4_LTS:
		case PI_MODEL_800:
		case PI_MODEL_5:
			return 17;
			break;
		case PI_MODEL_ZERO_2:
			return 21;
			break;
		case PI_MODEL_WIN:
		case PI_MODEL_PRIME:
		case PI_MODEL_PC_2:
		case PI_MODEL_H3:
		case PI_MODEL_RK3399:
		case PI_MODEL_4:
		case PI_MODEL_5_PLUS:
		case PI_MODEL_900:
		case PI_MODEL_CM4:
		case PI_MODEL_3B:
		case PI_MODEL_ZERO_2_W:
			return 28;
			break;
		case PI_MODEL_R1_PLUS:
			return 8;
			break;
		default:
			printf ("Oops - unable to determine board type... model: %d\n", model);
			return -1;
			break;
	}
}

int main (void)
{
	int i = 0;
	unsigned char gpio_num = 0;

	wiringPiSetup();

	gpio_num = getGpioNum();
	if (-1 == gpio_num)
		printf("Failed to get the number of GPIO!\n");

	for (i = 0; i < gpio_num; i++)
		pinMode (i, OUTPUT) ;

	for ( ;; )
	{
		for (i = 0; i < gpio_num; i++)
			digitalWrite (i, HIGH);	// On
		delay (2000);

		for (i = 0; i < gpio_num; i++)
			digitalWrite (i, LOW);	// Off
		delay (2000);
	}

	return 0;
}
