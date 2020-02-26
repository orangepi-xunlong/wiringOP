#include <stdio.h>
#include <wiringPi.h>

int main (void)
{
  int i = 0;

  wiringPiSetup () ;

  for (i=0; i<28; i++)
  	pinMode (i, OUTPUT) ;

  for (;;)
  {
    for (i=0; i<28; i++)
    	digitalWrite (i, HIGH) ;	// On
    delay (500) ;		// mS

    for (i=0; i<28; i++)
        digitalWrite (i, LOW) ;	// Off
    delay (500) ;
  }
  return 0 ;
}
