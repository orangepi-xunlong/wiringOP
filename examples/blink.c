#include <stdio.h>
#include <wiringPi.h>

//#define NUM 17   //26pin
//#define NUM 19   //for orangepi4
#define NUM 28     //40pin

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
