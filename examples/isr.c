/*
 * isr.c:
 *	Wait for Interrupt test program - ISR method
 *
 *	How to test:
 *	  Use the SoC's pull-up and pull down resistors that are avalable
 *	on input pins. So compile & run this program (via sudo), then
 *	in another terminal:
 *		gpio mode 0 up
 *		gpio mode 0 down
 *	at which point it should trigger an interrupt. Toggle the pin
 *	up/down to generate more interrupts to test.
 *
 * Copyright (c) 2013 Gordon Henderson.
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>


// globalCounter:
//	Global variable to count interrupts
//	Should be declared volatile to make sure the compiler doesn't cache it.

static volatile int globalCounter [8] ;


/*
 * myInterrupt:
 *********************************************************************************
 */

void myInterrupt0 (void) { ++globalCounter [0] ; delay(100);if(digitalRead(0)==LOW){ printf ("key 1 press\n");fflush (stdout) ;}}
void myInterrupt1 (void) { ++globalCounter [1] ; delay(100);if(digitalRead(1)==LOW){ printf ("key 2 press\n");fflush (stdout) ;}}
void myInterrupt2 (void) { ++globalCounter [2] ; delay(100);if(digitalRead(2)==LOW){ printf ("key 3 press\n");fflush (stdout) ;}}
void myInterrupt3 (void) { ++globalCounter [3] ; delay(100);if(digitalRead(3)==LOW){ printf ("key 4 press\n");fflush (stdout) ;}}
void myInterrupt4 (void) { ++globalCounter [4] ; delay(100);if(digitalRead(4)==LOW){ printf ("key 5 press\n");fflush (stdout) ;}}
void myInterrupt5 (void) { ++globalCounter [5] ; delay(100);if(digitalRead(5)==LOW){ printf ("key 6 press\n");fflush (stdout) ;}}
void myInterrupt6 (void) { ++globalCounter [6] ; delay(100);if(digitalRead(6)==LOW){ printf ("key 7 press\n");fflush (stdout) ;}}
void myInterrupt7 (void) { ++globalCounter [7] ; delay(100);if(digitalRead(7)==LOW){ printf ("key 8 press\n");fflush (stdout) ;}}


/*
 *********************************************************************************
 * main
 *********************************************************************************
 */

int main (void)
{
  int gotOne, pin ,ret;
  int myCounter [8] ;

  for (pin = 0 ; pin < 8 ; ++pin) 
    globalCounter [pin] = myCounter [pin] = 0 ;

  wiringPiSetup () ;

  wiringPiISR (0, INT_EDGE_FALLING, &myInterrupt0) ;
  wiringPiISR (1, INT_EDGE_FALLING, &myInterrupt1) ;
  wiringPiISR (2, INT_EDGE_FALLING, &myInterrupt2) ;
  wiringPiISR (3, INT_EDGE_FALLING, &myInterrupt3) ;
  wiringPiISR (4, INT_EDGE_FALLING, &myInterrupt4) ;
  wiringPiISR (5, INT_EDGE_FALLING, &myInterrupt5) ;

  // wiringPiISR (6, INT_EDGE_FALLING, &myInterrupt6) ;
  // wiringPiISR (7, INT_EDGE_FALLING, &myInterrupt7) ;
    
    printf ("Waiting ... \n") ; 
    fflush (stdout) ;

  for (;;)
  {
    gotOne = 0 ;

    sleep(1);
  }

  return 0 ;
}
