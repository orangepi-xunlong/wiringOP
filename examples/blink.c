#include <stdio.h>
#include <wiringPi.h>

//#define NUM 17
#define NUM 28

int main (void)
{
	int i = 0;

	wiringPiSetup () ;

	for (i = 0; i < NUM; i++)
		pinMode (i, OUTPUT) ;

	for ( ;; )
	{
      		for (i = 0; i < NUM; i++)
      			digitalWrite (i, HIGH) ;	// On
      		delay (500) ;		// mS

      		for (i = 0; i < NUM; i++)
      			digitalWrite (i, LOW) ;	// Off
      		delay (500) ;
	}

	return 0;
}
