#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#if 0
 +------+-----+----------+------+---+   OPi 3  +---+------+----------+-----+------+
 | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
 +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
 |      |     |     3.3V |      |   |  1 || 2  |   |      | 5V       |     |      |
 |  122 |   0 |    SDA.0 |  OFF | 0 |  3 || 4  |   |      | 5V       |     |      |
 |  121 |   1 |    SCL.0 |  OFF | 0 |  5 || 6  |   |      | GND      |     |      |
 |  118 |   2 |    PWM.0 |  OFF | 0 |  7 || 8  | 0 | OFF  | PL02     | 3   | 354  |
 |      |     |      GND |      |   |  9 || 10 | 0 | OFF  | PL03     | 4   | 355  |
 |  120 |   5 |    RXD.3 | ALT4 | 0 | 11 || 12 | 0 | OFF  | PD18     | 6   | 114  |
 |  119 |   7 |    TXD.3 | ALT4 | 0 | 13 || 14 |   |      | GND      |     |      |
 |  362 |   8 |     PL10 |  OFF | 0 | 15 || 16 | 0 | OFF  | PD15     | 9   | 111  |
 |      |     |     3.3V |      |   | 17 || 18 | 0 | OFF  | PD16     | 10  | 112  |
 |  229 |  11 |   MOSI.1 | ALT2 | 0 | 19 || 20 |   |      | GND      |     |      |
 |  230 |  12 |   MISO.1 | ALT2 | 0 | 21 || 22 | 0 | OFF  | PD21     | 13  | 117  |
 |  228 |  14 |   SCLK.1 | ALT2 | 0 | 23 || 24 | 0 | ALT2 | CE.1     | 15  | 227  |
 |      |     |      GND |      |   | 25 || 26 | 0 | OFF  | PL08     | 16  | 360  |
 +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
 | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
 +------+-----+----------+------+---+   OPi 3  +---+------+----------+-----+------+
#endif

#define PD15_PIN 9

static void usage(const char* argv0)
{
	printf("Usage: %s <on|off>\n", argv0);
	exit(1);
}

int main(int argc, char* argv[])
{
	printf ("OrangePi Pi wiringPi PD15 test program\n");

	if (argc != 2) usage(argv[0]);

	int mode = 0;
	if (!strcmp(argv[1], "on"))
		mode = HIGH;
	else if (!strcmp(argv[1], "off"))
		mode = LOW;
	else
		usage(argv[0]);

	if (wiringPiSetup() == -1)
	{
		fprintf(stderr, "Error setting up wiringPi\n");
		return -1;
	}

	pinMode(PD15_PIN, OUTPUT);
	digitalWrite(PD15_PIN, mode);

	return 0;
}

