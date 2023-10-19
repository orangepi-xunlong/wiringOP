/*
 * readall.c:
 *	The readall functions - getting a bit big, so split them out.
 *	Copyright (c) 2012-2018 Gordon Henderson
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
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <wiringPi.h>

extern int wpMode ;

#ifndef TRUE
#  define       TRUE    (1==1)
#  define       FALSE   (1==2)
#endif

/*
 * doReadallExternal:
 *	A relatively crude way to read the pins on an external device.
 *	We don't know the input/output mode of pins, but we can tell
 *	if it's an analog pin or a digital one...
 *********************************************************************************
 */

static void doReadallExternal (void)
{
  int pin ;

  printf ("+------+---------+--------+\n") ;
  printf ("|  Pin | Digital | Analog |\n") ;
  printf ("+------+---------+--------+\n") ;

  for (pin = wiringPiNodes->pinBase ; pin <= wiringPiNodes->pinMax ; ++pin)
    printf ("| %4d |  %4d   |  %4d  |\n", pin, digitalRead (pin), analogRead (pin)) ;

  printf ("+------+---------+--------+\n") ;
}

/*
 * doReadall:
 *	Read all the GPIO pins
 *	We also want to use this to read the state of pins on an externally
 *	connected device, so we need to do some fiddling with the internal
 *	wiringPi node structures - since the gpio command can only use
 *	one external device at a time, we'll use that to our advantage...
 *********************************************************************************
 */

static char * alts_common [] =
{
  "IN", "OUT", "ALT2", "ALT3", "ALT4", "ALT5", "ALT6", "OFF"
};

static char * alts_rk3588 [] =
{
  "IN", "OUT", "ALT1", "ALT2", "ALT3", "ALT4", "ALT5", "ALT6", "ALT7", "ALT8", "ALT9", "ALT10", "ALT11", "ALT12", "ALT13", "ALT14",
};

static char ** alts = alts_rk3588;

static int physToWpi_PC_2[64] =
{
	-1,     // 0
	-1, -1, // 1, 2
	 0, -1, // 3, 4
	 1, -1, // 5, 6
   	 2,  3, // 7, 8
	-1,  4, // 9, 10
	 5,  6, //11, 12
	 7, -1, //13, 14
	 8,  9, //15, 16
	-1, 10, //17, 18
	11, -1, //19, 20
	12, 13, //21, 22
	14, 15, //23, 24
	-1, 16, //25, 26
	17, 18, //27, 28
	19, -1, //29, 30
	20, 21, //31, 32
	22, -1, //33, 34
	23, 24, //35, 36
	25, 26, //37, 38
	-1, 27, //39, 40

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
	-1, -1, -1, -1, -1, -1, -1, -1 								// 56-> 63
};

static char * physNames_PC_2[64] =
{
    NULL,
	"    3.3V", "5V      ",
	"   SDA.0", "5V      ",
	"   SCL.0", "GND     ",
	"   PWM.1", "PC05    ",
	"     GND", "PC06    ",
	"   RXD.2", "PD14    ",
	"   TXD.2", "GND     ",
	"   CTS.2", "PC04    ",
	"    3.3V", "PC07    ",
	"  MOSI.1", "GND     ",
	"  MISO.1", "RTS.2   ",
	"  SCLK.1", "CE.1    ",
	"     GND", "PA21    ",
	"   SDA.1", "SCL.1   ",
	"    PA07", "GND     ",
	"    PA08", "RTS.1   ",
	"    PA09", "GND     ",
	"    PA10", "CTS.1   ",
	"    PD11", "TXD.1   ",
	"     GND", "RXD.1   ",
};

static int physToWpi_PRIME[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26
	17, 18, //27,28
	19, -1, //29,30
	20, 21, //31,32
	22, -1, //33,34
	23, 24, //35,36
	25, 26, //37,38
	-1, 27, //39,40

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
	-1, -1, -1, -1, -1, -1, -1, -1 								// 56-> 63
};

static char * physNames_PRIME[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.0", "5V      ",
	"   SCL.0", "GND     ",
	"   PWM.1", "PC05    ",
	"     GND", "PC06    ",
	"   RXD.2", "PD14    ",
	"   TXD.2", "GND     ",
	"   CTS.2", "PC04    ",
	"    3.3V", "PC07    ",
	"  MOSI.1", "GND     ",
	"  MISO.1", "RTS.2   ",
	"  SCLK.1", "CE.1    ",
	"     GND", "PC08    ",
	"   SDA.1", "SCL.1   ",
	"    PA07", "GND     ",
	"    PA08", "PC09    ",
	"    PA09", "GND     ",
	"    PA10", "PC10    ",
	"    PD11", "PC11    ",
	"     GND", "PC12    ",
};

static int physToWpi_ZERO_PLUS[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26

	// Padding:
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40
	-1, -1, //41,42
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
	-1, -1, -1, -1, -1, -1, -1,  							  // ... 63
};

static char * physNames_ZERO_PLUS[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.0", "5V      ",
	"   SCL.0", "GND     ",
	"     PA6", "TXD.1   ",
	"     GND", "RXD.1   ",
	"   RXD.2", "PA07    ",
	"   TXD.2", "GND     ",
	"   CTS.2", "SDA.1   ",
	"    3.3V", "SCL.1   ",
	"  MOSI.1", "GND     ",
	"  MISO.1", "RTS.2   ",
	"  SCLK.1", "CE.1    ",
	"     GND", "PA10    ",
};

static int physToWpi_ZERO_PLUS_2[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26

	// Padding:
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40
	-1, -1, //41,42
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
	-1, -1, -1, -1, -1, -1, -1,   							  // ... 63
};

static char * physNames_ZERO_PLUS_2[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.0", "5V      ",
	"   SCL.0", "GND     ",
	"     PA6", "TXD.2   ",
	"     GND", "RXD.2   ",
	"   S-SCL", "PD11    ",
	"   S-SDA", "GND     ",
	"   CTS.2", "SDA.1   ",
	"    3.3V", "SCL.1   ",
	"  MOSI.1", "GND     ",
	"  MISO.1", "RTS.2   ",
	"  SCLK.1", "CE.1    ",
	"     GND", "PD14    ",
};

static int physToWpi_H3[64] =
{
	-1,	    // 0
	-1, -1,	// 1,  2
	 0, -1, // 3,  4
	 1, -1, // 5,  6
	 2,  3, // 7,  8
	-1,  4, // 9, 10
	 5,  6, //11, 12
	 7, -1, //13, 14
	 8,  9, //15, 16
	-1, 10, //17, 18
	11, -1, //19, 20
	12, 13, //21, 22
	14, 15, //23, 24
	-1, 16,	//25, 26
	17, 18, //27, 28
	19, -1,	//29, 30
	20, 21,	//31, 32
	22, -1, //33, 34
	23, 24, //35, 36
	25, 26, //37, 38
	-1, 27, //39, 40
	28, 29, //41, 42

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 56
	-1, -1, -1, -1, -1, -1, -1, 						    // ... 63
};

static char * physNames_H3[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.0", "5V      ",
	"   SCL.0", "GND     ",
	"     PA6", "TXD.3   ",
	"     GND", "RXD.3   ",
	"   RXD.2", "PD14    ",
	"   TXD.2", "GND     ",
	"   CTS.2", "PC04    ",
	"    3.3V", "PC07    ",
	"  MOSI.0", "GND     ",
	"  MISO.0", "RTS.2   ",
	"  SCLK.0", "CE.0    ",
	"     GND", "PA21    ",
	"   SDA.1", "SCL.1   ",
	"    PA07", "GND     ",
	"    PA08", "RTS.1   ",
	"    PA09", "GND     ",
	"    PA10", "CTS.1   ",
	"    PA20", "TXD.1   ",
	"     GND", "RXD.1   ",
	"    PA04", "PA05    ",
};

static int physToWpi_ZERO[64] =
{
	-1,       // 0
	-1, -1,   // 1, 2
 	 0, -1,   // 3, 4
	 1, -1,   // 5, 6
	 2,  3,   // 7, 8
	-1,  4,   // 9, 10
	 5,  6,   //11, 12
	 7, -1,   //13, 14
	 8,  9,   //15, 16
	-1, 10,   //17, 18
	11, -1,   //19, 20
	12, 13,   //21, 22
	14, 15,   //23, 24
	-1, 16,   //25, 26

	// Padding:
	-1, -1,   //27, 28
	-1, -1,   //29, 30
	-1, -1,   //31, 32
	-1, -1,   //33, 34
	-1, -1,   //35, 36
	-1, -1,   //37, 38
	-1, -1,   //39, 40
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
	-1, -1, -1, -1, -1, -1, -1, -1 								// 56-> 63
};

static char * physNames_ZERO[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.0", "5V      ",
	"   SCL.0", "GND     ",
	"     PA6", "TXD.1   ",
	"     GND", "RXD.1   ",
	"   RXD.2", "PA07    ",
	"   TXD.2", "GND     ",
	"   CTS.2", "SDA.1   ",
	"    3.3V", "SCK.1   ",
	"  MOSI.1", "GND     ",
	"  MISO.1", "RTS.2   ",
	"  SCLK.1", "CE.1    ",
	"     GND", "PA10    ",
};

static int physToWpi_WIN[64] =
{
	-1,        // 0
	-1,  -1,   // 1, 2
	 0,  -1,   // 3, 4
	 1,  -1,   // 5, 6
	 2,   3,   // 7, 8
	-1,   4,   // 9, 10
	 5,   6,   //11, 12
	 7,  -1,   //13, 14
	 8,   9,   //15, 16
	-1,  10,   //17, 18
	11,  -1,   //19, 20
	12,  13,   //21, 22
	14,  15,   //23, 24
	-1,  16,   //25, 26
	17,  18,   //27, 28
	19,  -1,   //29, 30
	20,  21,   //31, 32
	22,  -1,   //33, 34
	23,  24,   //35, 36
	25,  26,   //37, 38
	-1,  27,   //39, 40

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
	-1, -1, -1, -1, -1, -1, -1, -1 								// 56-> 63
};

static char * physNames_WIN[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.1", "5V      ",
	"   SCL.1", "GND     ",
	"    PL10", "PL02    ",
	"     GND", "PL03    ",
	"   RXD.3", "PD04    ",
	"   TXD.3", "GND     ",
	"   CTS.3", "PL09    ",
	"    3.3V", "PC04    ",
	"  MOSI.1", "GND     ",
	"  MISO.1", "RTS.3   ",
	"  SCLK.1", "CE.1    ",
	"     GND", "PD06    ",
	"   SDA.2", "SCL.2   ",
	"    PB04", "GND     ",
	"    PB05", "RTS.2   ",
	"    PB06", "GND     ",
	"    PB07", "CTS.2   ",
	"    PD05", "TXD.2   ",
	"     GND", "RXD.2   ",
};

static int physToWpi_LITE_2[64] = 
{
	-1, 		  //0
	-1, -1, 	  //1,2
	 0, -1,		  //3,4
	 1, -1, 	  //5,6
	 2,  3, 	  //7,8
	-1,  4, 	  //9,10
	 5,  6, 	  //11,12
	 7, -1, 	  //13,14
	 8,  9, 	  //15,16
	-1, 10, 	  //17,18
	11, -1, 	  //19,20
	12, 13, 	  //21,22
	14, 15, 	  //23,24
	-1, 16, 	  //25,26

	// Padding:
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40
	-1, -1, //41,42
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
	-1, -1, -1, -1, -1, -1, -1,   							  // ... 63
};

static char * physNames_LITE_2[64] = 
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.1", "5V      ",
	"   SCL.1", "GND     ",
	"     PH4", "PD21    ",
	"     GND", "PD22    ",
	"   RXD.3", "PC9     ",
	"   TXD.3", "GND     ",
	"   CTS.3", "PC8     ",
	"    3.3V", "PC7     ",
	"  MOSI.0", "GND     ",
	"  MISO.0", "RTS.3   ",
	"  SCLK.0", "CE.0    ",
	"     GND", "PH3     ",
};

static int physToWpi_3[64] = 
{
	-1, 		  //0
	-1, -1, 	  //1,2
	 0, -1,		  //3,4
	 1, -1, 	  //5,6
	 2,  3, 	  //7,8
	-1,  4, 	  //9,10
	 5,  6, 	  //11,12
	 7, -1, 	  //13,14
	 8,  9, 	  //15,16
	-1, 10, 	  //17,18
	11, -1, 	  //19,20
	12, 13, 	  //21,22
	14, 15, 	  //23,24
	-1, 16, 	  //25,26

	// Padding:
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40
	-1, -1, //41,42
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
	-1, -1, -1, -1, -1, -1, -1,   							  // ... 63

};

static char * physNames_3[64] = 
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.0", "5V      ",
	"   SCL.0", "GND     ",
	"   PWM.0", "PL02    ",
	"     GND", "PL03    ",
	"   RXD.3", "PD18    ",
	"   TXD.3", "GND     ",
	"    PL10", "PD15    ",
	"    3.3V", "PD16    ",
	"  MOSI.1", "GND     ",
	"  MISO.1", "PD21    ",
	"  SCLK.1", "CE.1    ",
	"     GND", "PL08    ",
};

static int physToWpi_ZERO_2[64] = 
{
	-1, 	// 0
	-1, -1, // 1, 2
	 0, -1, // 3, 4
	 1, -1, // 5, 6
	 2,  3, // 7, 8
	-1,  4, // 8, 10
	 5,  6, //11, 12
	 7, -1, //13, 14  
	 8,  9, //15, 16
	-1, 10, //17, 18
	11, -1, //19, 20
	12, 13, //21, 22
	14, 15, //23, 24
	-1, 16, //25, 26
	17, 21, //27, 28
	18, 22, //29, 30
	19, -1, //31, 32
	20, -1, //33, 34

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
	-1,  													  // ... 63
};

static char * physNames_ZERO_2[64] = 
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.3", "5V      ",
	"   SCL.3", "GND     ",
	"     PC9", "TXD.5   ",
	"     GND", "RXD.5   ",
	"     PC6", "PC11    ",
	"     PC5", "GND     ",
	"     PC8", "PC15    ",
	"    3.3V", "PC14    ",
	"  MOSI.1", "GND     ",
	"  MISO.1", "PC7     ",
	"  SCLK.1", "CE.1    ",
	"     GND", "PC10    ",
	"     PC1", "PWM3    ",
	"    PI16", "PWM4    ",
	"     PI6", "        ",
	"    PH10", "        ",
};

static int physToWpi_ZERO_2_W[64] =
{
        -1,     // 0
        -1, -1, // 1, 2
         0, -1, // 3, 4
         1, -1, // 5, 6
         2,  3, // 7, 8
        -1,  4, // 9, 10
         5,  6, //11, 12
         7, -1, //13, 14
         8,  9, //15, 16
        -1, 10, //17, 18
        11, -1, //19, 20
        12, 13, //21, 22
        14, 15, //23, 24
        -1, 16, //25, 26
        17, 18, //27, 28
        19, -1, //29, 30
        20, 21, //31, 32
        22, -1, //33, 34
        23, 24, //35, 36
        25, 26, //37, 38
        -1, 27, //39, 40

        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
        -1, -1, -1, -1, -1, -1, -1, -1                              // 56-> 63
};

static char * physNames_ZERO_2_W[64] =
{
	      NULL,
	"    3.3V", "5V      ",
	"   SDA.1", "5V      ",
	"   SCL.1", "GND     ",
	"    PWM3", "TXD.0   ",
	"     GND", "RXD.0   ",
	"   TXD.5", "PI01    ",
	"   RXD.5", "GND     ",
	"   TXD.2", "PWM4    ",
	"    3.3V", "PH04    ",
	"  MOSI.1", "GND     ",
	"  MISO.1", "RXD.2   ",
	"  SCLK.1", "CE.0    ",
	"     GND", "CE.1    ",
	"   SDA.2", "SCL.2   ",
	"    PI00", "GND     ",
	"    PI15", "PWM1    ",
	"    PI12", "GND     ",
	"    PI02", "PC12    ",
	"    PI16", "PI04    ",
	"     GND", "PI03    ",
};

static int physToWpi_RK3399[64] =
{
	-1,		//0
	-1, -1,	//1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16,	//25,26
	17,	18, //27,28
	19, -1,	//29,30
	20, 21,	//31,32
	22, -1, //33,34
	23, 24, //35,36
	25, 26, //37,38
	-1, 27, //39,40

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 56
	-1, -1, -1, -1, -1, -1, -1,										// ... 63
};

static char * physNames_RK3399[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.0", "5V      ",
	"   SCL.0", "GND     ",
	"   GPIO4", "Tx      ",
	"     GND", "Rx      ",
	"  GPIO17", "GPIO18  ",
	"  GPIO27", "GND     ",
	"  GPIO22", "GPIO23  ",
	"    3.3V", "GPIO24  ",
	"    MOSI", "GND     ",
	"    MISO", "GPIO25  ",
	"    SCLK", "CS0     ",
	"     GND", "CS1     ",
	"    DNP1", "DNP2    ",
	"   GPIO5", "GND     ",
	"   GPIO6", "GPIO12  ",
	"  GPIO13", "GND     ",
	"  GPIO19", "GPIO16  ",
	"  GPIO26", "GPIO20  ",
	"     GND", "GPIO21  ",
};

static int physToWpi_4[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26

	// Padding:
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,     								// ... 63
};

static char * physNames_4[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"I2C2_SDA", "5V      ",
	"I2C2_SCL", "GND     ",
	"    PWM1", "I2C3_SCL",
	"     GND", "I2C3_SDA",
	"GPIO1_A1", "GPIO1_C2",
	"GPIO1_A3", "GND     ",
	"GPIO2_D4", "GPIO1_C6",
	"    3.3V", "GPIO1_C7",
	"SPI1_TXD", "GND     ",
	"SPI1_RXD", "GPIO1_D0",
	"SPI1_CLK", "SPI1_CS ",
	"     GND", "GPIO4_C5",
	"I2C2_SDA", "I2C2_SCL",
	" I2S0_RX", "GND     ",
	" I2S0_TX", "I2S_CLK ",
	"I2S0_SCK", "GND     ",
	"I2S0_SI0", "I2S0_SO0",
	"I2S0_SI1", "I2S0_SI2",
	"     GND", "I2S0_SI3",
};

static int physToWpi_4_LTS[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26

	// Padding:
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,    	 								// ... 63
};

static char * physNames_4_LTS[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"I2C8_SDA", "5V      ",
	"I2C8_SCL", "GND     ",
	"    PWM1", "I2C3_SCL",
	"     GND", "I2C3_SDA",
	"GPIO1_A1", "GPIO1_C2",
	"GPIO1_A3", "GND     ",
	"GPIO2_D4", "GPIO1_C6",
	"    3.3V", "GPIO1_C7",
	"SPI1_TXD", "GND     ",
	"SPI1_RXD", "GPIO1_D0",
	"SPI1_CLK", "SPI1_CS ",
	"     GND", "GPIO4_C5",
};

static int physToWpi_800[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,     								// ... 63
};

static char * physNames_800[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.4", "5V      ",
	"   SCL.4", "GND     ",
	"    PWM1", "RXD.4   ",
	"     GND", "TXD.4   ",
	"   TXD.2", "GPIO4_D0",
	"   RXD.2", "GND     ",
	"GPIO4_C5", "SDA.2   ",
	"    3.3V", "SCL.2   ",
	"SPI2_TXD", "GND     ",
	"SPI2_RXD", "GPIO4_D1",
	"SPI2_CLK", "SPI2_CS ",
	"     GND", "GPIO4_D2",
};


static int physToWpi_R1_PLUS[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0,  1, //3,4
	 2,  3, //5,6
	-1, -1, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14

	// Padding:
	-1, -1, //15,16
	-1, -1, //17,18
	-1, -1, //19,20
	-1, -1, //21,22
	-1, -1, //23,24
	-1, -1, //25,26
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,  									// ... 63
};

static char * physNames_R1_PLUS[64] =
{
	NULL,
	"5V      ", "GND     ",
	"SDA.0   ", "SCK.0   ",
	"TXD.1   ", "RXD.1   ",
	"        ", "        ",
	"        ", "GPIO3_C0",
	"CTS.1   ", "RTS.1   ",
	"GPIO2_A2", "        ",
};

static int physToWpi_5[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,    					// ... 63
};

static char * physNames_5[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.5", "5V      ",
	"   SCL.5", "GND     ",
	"   PWM15", "RXD.0   ",
	"     GND", "TXD.0   ",
	" CAN1_RX", "CAN2_TX ",
	" CAN1_TX", "GND     ",
	" CAN2_RX", "SDA.1   ",
	"    3.3V", "SCL.1   ",
	"SPI4_TXD", "GND     ",
	"SPI4_RXD", "GPIO2_D4",
	"SPI4_CLK", "SPI4_CS1",
	"     GND", "PWM1    ",
};

static int physToWpi_5B[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, -1, //21,22
	13, 14, //23,24
	-1, 15, //25,26
	-1, -1, //27,28
	-1, -1, //29,30
	-1, -1, //31,32
	-1, -1, //33,34
	-1, -1, //35,36
	-1, -1, //37,38
	-1, -1, //39,40

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,    					// ... 63
};

static char * physNames_5B[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.5", "5V      ",
	"   SCL.5", "GND     ",
	"   PWM15", "RXD.0   ",
	"     GND", "TXD.0   ",
	" CAN1_RX", "CAN2_TX ",
	" CAN1_TX", "GND     ",
	" CAN2_RX", "SDA.1   ",
	"    3.3V", "SCL.1   ",
	"SPI4_TXD", "GND     ",
	"SPI4_RXD", "PowerKey",
	"SPI4_CLK", "SPI4_CS1",
	"     GND", "PWM1    ",
};

static int physToWpi_5PLUS[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26
	17, 18, //27,28
	19, -1, //29,30
	20, 21, //31,32
	22, -1, //33,34
	23, 24, //35,36
	25, 26, //37,38
	-1, 27, //39,40

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,    					// ... 63
};

static char * physNames_5PLUS[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.2", "5V      ",
	"   SCL.2", "GND     ",
	"   PWM14", "GPIO1_A1",
	"     GND", "GPIO1_A0",
	"GPIO1_A4", "GPIO3_A1",
	"GPIO1_A7", "GND     ",
	"GPIO1_B0", "GPIO3_B5",
	"    3.3V", "GPIO3_B6",
	"SPI0_TXD", "GND     ",
	"SPI0_RXD", "GPIO1_A2",
	"SPI0_CLK", "SPI0_CS0",
	"     GND", "SPI0_CS1",
	"GPIO1_B7", "GPIO1_B6",
	"GPIO1_D7", "GND     ",
	"GPIO3_A0", "GPIO1_A3",
	"GPIO3_C2", "GND     ",
	"GPIO3_A2", "GPIO3_A5",
	"GPIO3_C1", "GPIO3_A4",
	"     GND", "GPIO3_A3",
};

static int physToWpi_900[64] =
{
        -1,     //0
        -1, -1, //1,2
         0, -1, //3,4
         1, -1, //5,6
         2,  3, //7,8
        -1,  4, //9,10
         5,  6, //11,12
         7, -1, //13,14
         8,  9, //15,16
        -1, 10, //17,18
        11, -1, //19,20
        12, 13, //21,22
        14, 15, //23,24
        -1, 16, //25,26
        17, 18, //27,28
        19, -1, //29,30
        20, 21, //31,32
        22, -1, //33,34
        23, 24, //35,36
        25, 26, //37,38
        -1, 27, //39,40

        // Padding:
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
        -1, -1, -1, -1, -1, -1, -1,                                     // ... 63
};

static char * physNames_900[64] =
{
        NULL,
        "    3.3V", "5V      ",
        "   SDA.4", "5V      ",
        "   SCL.4", "GND     ",
        "    PWM3", "GPIO1_B6",
        "     GND", "GPIO1_B7",
        "GPIO3_A5", "GPIO3_A1",
        "GPIO3_A0", "GND     ",
        "GPIO1_C1", "GPIO3_B3",
        "    3.3V", "GPIO1_D6",
        "GPIO1_B2", "GND     ",
        "GPIO1_B1", "GPIO1_D7",
        "GPIO1_B3", "GPIO1_B4",
        "     GND", "GPIO1_B5",
        "GPIO1_A0", "GPIO1_A1",
        "GPIO1_A4", "GND     ",
        "GPIO1_B0", "GPIO3_C2",
        "GPIO3_B2", "GND     ",
        "GPIO3_A2", "GPIO3_B6",
        "GPIO3_B5", "GPIO3_A4",
        "     GND", "GPIO3_A3",
};

static int physToWpi_CM4[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26
	17, 18, //27,28
	19, -1, //29,30
	20, 21, //31,32
	22, -1, //33,34
	23, 24, //35,36
	25, 26, //37,38
	-1, 27, //39,40

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,    					// ... 63
};

static char * physNames_CM4[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"   SDA.2", "5V      ",
	"   SCL.2", "GND     ",
	"   PWM15", "RXD.2   ",
	"     GND", "TXD.2   ",
	"GPIO3_C6", "GPIO3_C7",
	"GPIO4_A0", "GND     ",
	"   TXD.7", "RXD.7   ",
	"    3.3V", "GPIO4_A1",
	"SPI3_TXD", "GND     ",
	"SPI3_RXD", "TXD.9   ",
	"SPI3_CLK", "SPI3_CS1",
	"     GND", "GPIO4_A7",
	"   SDA.3", "SCL.3   ",
	"   RXD.9", "GND     ",
	"GPIO3_D4", "PWM11   ",
	"GPIO3_D7", "GND     ",
	"GPIO3_D0", "GPIO3_D5",
	"GPIO3_D3", "GPIO3_D2",
	"     GND", "GPIO3_D1",
};

static int physToWpi_3PLUS[64] =
{
	-1,     //0
	-1, -1, //1,2
	 0, -1, //3,4
	 1, -1, //5,6
	 2,  3, //7,8
	-1,  4, //9,10
	 5,  6, //11,12
	 7, -1, //13,14
	 8,  9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23,24
	-1, 16, //25,26
	17, 18, //27,28
	19, -1, //29,30
	20, 21, //31,32
	22, -1, //33,34
	23, 24, //35,36
	25, 26, //37,38
	-1, 27, //39,40

	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,    					// ... 63
};

static char * physNames_3PLUS[64] =
{
	NULL,
	"    3.3V", "5V      ",
	"  SDA_M3", "5V      ",
	"  SCL_M3", "GND     ",
	"   PWM_F", "TXD_A   ",
	"     GND", "RXD_A   ",
	"    PAO7", "PA12    ",
	"   RXD_B", "GND     ",
	"   TXD_B", "PH8     ",
	"    3.3V", "PA3     ",
	"  MOSI_B", "GND     ",
	"  MISO_B", "PAO5    ",
	"  SCLK_B", "SSO_B   ",
	"     GND", "PWMAO_C ",
	"  SDA_M0", "SCL_M0  ",
	"     PA2", "GND     ",
	"     PC7", "PA4     ",
	"   PAO10", "GND     ",
	"    PA13", "PA0     ",
	"     PA7", "PA10    ",
	"     GND", "PA9     ",
};


static int * physToWpi;
static char ** physNames;


/*
 * readallPhys:
 *	Given a physical pin output the data on it and the next pin:
 *| BCM | wPi |   Name  | Mode | Val| Physical |Val | Mode | Name    | wPi | BCM |
 *********************************************************************************
 */
void readallPhys (int physPin)
{
	int pin ;

	if (physPinToGpio (physPin) == -1)
		printf (" |      |    ") ;
	else
		printf (" | %4d | %3d", physPinToGpio (physPin), physToWpi [physPin]) ;

	printf (" | %s", physNames[physPin]);
	

	if (physToWpi [physPin] == -1)
	{
		printf (" |        |  ") ;
	}
	else
	{
		if (wpMode == WPI_MODE_GPIO)
			pin = physPinToGpio (physPin) ;
		else if (wpMode == WPI_MODE_PHYS)
			pin = physPin ;
		else
			pin = physToWpi [physPin] ;

		printf (" | %6s", alts[getAlt(pin)]) ;
		printf (" | %d", digitalRead (pin)) ;
	}

	// Pin numbers:
	printf (" | %2d", physPin) ;
	++physPin ;
	printf (" || %-2d", physPin) ;

	// Same, reversed
	if (physToWpi [physPin] == -1)
		printf (" |   |       ") ;
	else
	{
		if (wpMode == WPI_MODE_GPIO)
			pin = physPinToGpio (physPin) ;
		else if (wpMode == WPI_MODE_PHYS)
			pin = physPin ;
		else
			pin = physToWpi [physPin] ;

		printf (" | %d", digitalRead (pin)) ;
		printf (" | %-6s", alts [getAlt (pin)]) ;
	}

	printf (" | %-5s", physNames [physPin]) ;

	if (physToWpi     [physPin] == -1)
		printf (" |     |     ") ;
	else
		printf (" | %-3d | %-4d", physToWpi [physPin], physPinToGpio (physPin)) ;

	printf (" |\n") ;
}


/*
 * allReadall:
 *	Read all the pins regardless of the model. Primarily of use for
 *	the compute module, but handy for other fiddling...
 *********************************************************************************
 */

static void allReadall (void)
{
  int pin ;

  printf ("+-----+------+-------+      +-----+------+-------+\n") ;
  printf ("| Pin | Mode | Value |      | Pin | Mode | Value |\n") ;
  printf ("+-----+------+-------+      +-----+------+-------+\n") ;

  for (pin = 0 ; pin < 14 ; ++pin)
  {
    printf ("| %3d ", pin) ;
    printf ("| %-4s ", alts [getAlt (pin)]) ;
    printf ("| %s  ", digitalRead (pin) == HIGH ? "High" : "Low ") ;
    printf ("|      ") ;
    printf ("| %3d ", pin + 14) ;
    printf ("| %-4s ", alts [getAlt (pin + 14)]) ;
    printf ("| %s  ", digitalRead (pin + 14) == HIGH ? "High" : "Low ") ;
    printf ("|\n") ;
  }

  printf ("+-----+------+-------+      +-----+------+-------+\n") ;

}


/*
 * abReadall:
 *	Read all the pins on the model A or B.
 *********************************************************************************
 */

void abReadall (int model, int rev)
{
  int pin ;
  char *type ;

  if (model == PI_MODEL_A)
    type = " A" ;
  else
    if (rev == PI_VERSION_2)
      type = "B2" ;
    else
      type = "B1" ;

  printf (" +-----+-----+---------+------+---+-Model %s-+---+------+---------+-----+-----+\n", type) ;
  printf (" | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |\n") ;
  printf (" +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+\n") ;
  for (pin = 1 ; pin <= 26 ; pin += 2)
    readallPhys (pin) ;

  if (rev == PI_VERSION_2) // B version 2
  {
    printf (" +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+\n") ;
    for (pin = 51 ; pin <= 54 ; pin += 2)
      readallPhys (pin) ;
  }

  printf (" +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+\n") ;
  printf (" | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |\n") ;
  printf (" +-----+-----+---------+------+---+-Model %s-+---+------+---------+-----+-----+\n", type) ;
}


/*
 * piPlusReadall:
 *	Read all the pins on the model A+ or the B+ or actually, all 40-pin Pi's
 *********************************************************************************
 */

static void plus2header (int model)
{
  /**/ if (model == PI_MODEL_AP)
    printf (" +-----+-----+---------+------+---+---Pi A+--+---+------+---------+-----+-----+\n") ;
  else if (model == PI_MODEL_BP)
    printf (" +-----+-----+---------+------+---+---Pi B+--+---+------+---------+-----+-----+\n") ;
  else if (model == PI_MODEL_ZERO)
    printf (" +-----+-----+---------+------+---+-Pi Zero--+---+------+---------+-----+-----+\n") ;
  else if (model == PI_MODEL_ZERO_W)
    printf (" +-----+-----+---------+------+---+-Pi ZeroW-+---+------+---------+-----+-----+\n") ;
  else if (model == PI_MODEL_2)
    printf (" +-----+-----+---------+------+---+---Pi 2---+---+------+---------+-----+-----+\n") ;
  else if (model == PI_MODEL_3)
    printf (" +-----+-----+---------+------+---+---Pi 3---+---+------+---------+-----+-----+\n") ;
  else if (model == PI_MODEL_3P)
    printf (" +-----+-----+---------+------+---+---Pi 3+--+---+------+---------+-----+-----+\n") ;
  else
    printf (" +-----+-----+---------+------+---+---Pi ?---+---+------+---------+-----+-----+\n") ;
}


static void piPlusReadall (int model)
{
  int pin ;

  plus2header (model) ;

  printf (" | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |\n") ;
  printf (" +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+\n") ;
  for (pin = 1 ; pin <= 40 ; pin += 2)
    readallPhys (pin) ;
  printf (" +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+\n") ;
  printf (" | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |\n") ;

  plus2header (model) ;
}

/*
 * ReadAll 
 */
void OrangePiReadAll(int model)
{
	int pin;
	int tmp = wiringPiDebug;
	wiringPiDebug = FALSE;

	switch (model)
	{
		case PI_MODEL_PC_2:
			printf (" +------+-----+----------+--------+---+  OPi PC2 +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_PC_2;
			physNames =  physNames_PC_2;
			alts = alts_common;
			break;
		case PI_MODEL_PRIME:
			printf (" +------+-----+----------+--------+---+   PRIME  +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_PRIME;
			physNames =  physNames_PRIME;
			alts = alts_common;
			break;
		case PI_MODEL_ZERO_PLUS:
			printf (" +------+-----+----------+--------+---+ ZEROPLUS +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_ZERO_PLUS;
			physNames =  physNames_ZERO_PLUS;
			alts = alts_common;
			break;
		case PI_MODEL_ZERO_PLUS_2:
			printf (" +------+-----+----------+--------+---+ZEROPLUS 2+---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_ZERO_PLUS_2;
			physNames =  physNames_ZERO_PLUS_2;
			alts = alts_common;
			break;
		case PI_MODEL_H3:
			printf (" +------+-----+----------+--------+---+OrangePiH3+---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_H3;
			physNames =  physNames_H3;
			alts = alts_common;
			break;
		case PI_MODEL_ZERO:
			printf (" +------+-----+----------+--------+---+  OPi H2  +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_ZERO;
			physNames =  physNames_ZERO;
			alts = alts_common;
			break;
		case PI_MODEL_WIN:
			printf (" +------+-----+----------+--------+---+  OPi Win +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_WIN;
			physNames =  physNames_WIN;
			alts = alts_common;
			break;
		case PI_MODEL_LTIE_2:
			printf (" +------+-----+----------+--------+---+  OPi H6  +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_LITE_2;
			physNames =  physNames_LITE_2;
			alts = alts_common;
			break;
		case PI_MODEL_3:
			printf (" +------+-----+----------+--------+---+   OPi 3  +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_3;
			physNames =  physNames_3;
			alts = alts_common;
			break;
		case PI_MODEL_ZERO_2:
			printf (" +------+-----+----------+--------+---+   H616   +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_ZERO_2;
			physNames =  physNames_ZERO_2;
			alts = alts_common;
			break;
		case PI_MODEL_ZERO_2_W:
			printf (" +------+-----+----------+--------+---+  ZERO2W  +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_ZERO_2_W;
			physNames =  physNames_ZERO_2_W;
			alts = alts_common;
			break;
		case PI_MODEL_RK3399:
			printf (" +------+-----+----------+--------+---+OPi RK3399+---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_RK3399;
			physNames =  physNames_RK3399;
			alts = alts_common;
			break;
		case PI_MODEL_4:
			printf (" +------+-----+----------+--------+---+OrangePi 4+---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_4;
			physNames =  physNames_4;
			alts = alts_common;
			break;
		case PI_MODEL_4_LTS:
			printf (" +------+-----+----------+--------+---+ OPi 4 LTS+---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_4_LTS;
			physNames =  physNames_4_LTS;
			alts = alts_common;
			break;
		case PI_MODEL_800:
			physToWpi =  physToWpi_800;
			physNames =  physNames_800;
			alts = alts_common;
			printf (" +------+-----+----------+--------+---+  opi800  +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_R1_PLUS:
			printf (" +------+-----+----------+--------+---+  R1 Plus +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_R1_PLUS;
			physNames =  physNames_R1_PLUS;
			alts = alts_common;
			break;
		case PI_MODEL_5:
			printf (" +------+-----+----------+--------+---+   OPI5   +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_5;
			physNames =  physNames_5;
			alts = alts_rk3588;
			break;
		case PI_MODEL_5B:
			printf (" +------+-----+----------+--------+---+   PI5B   +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_5B;
			physNames =  physNames_5B;
			alts = alts_rk3588;
			break;
		case PI_MODEL_5_PLUS:
			printf (" +------+-----+----------+--------+---+ PI5 PLUS +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_5PLUS;
			physNames =  physNames_5PLUS;
			alts = alts_rk3588;
			break;
		case PI_MODEL_900:
			printf (" +------+-----+----------+--------+---+  PI 900  +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_900;
			physNames =  physNames_900;
			alts = alts_rk3588;
			break;
		case PI_MODEL_CM4:
			printf (" +------+-----+----------+--------+---+  PI CM4  +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_CM4;
			physNames =  physNames_CM4;
			alts = alts_rk3588;
			break;
		case PI_MODEL_3B:
			printf (" +------+-----+----------+--------+---+   PI3B   +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_CM4;
			physNames =  physNames_CM4;
			alts = alts_rk3588;
			break;
		case PI_MODEL_3_PLUS:
			printf (" +------+-----+----------+--------+---+ PI3 PLUS +---+--------+----------+-----+------+\n");
			physToWpi =  physToWpi_3PLUS;
			physNames =  physNames_3PLUS;
			alts = alts_common;
			break;
		default:
			printf ("Oops - unable to determine board type... model: %d\n", model);
			break ;
	}

	printf (" | GPIO | wPi |   Name   |  Mode  | V | Physical | V |  Mode  | Name     | wPi | GPIO |\n");
	printf (" +------+-----+----------+--------+---+----++----+---+--------+----------+-----+------+\n");

	switch (model)
	{
		case PI_MODEL_H3:
		case PI_MODEL_RK3399:
		case PI_MODEL_4:
		case PI_MODEL_PC_2:
		case PI_MODEL_PRIME:
		case PI_MODEL_WIN:
		case PI_MODEL_5_PLUS:
		case PI_MODEL_900:
		case PI_MODEL_CM4:
		case PI_MODEL_3B:
		case PI_MODEL_ZERO_2_W:
		case PI_MODEL_3_PLUS:
			for (pin = 1 ; pin <= 40; pin += 2)
				readallPhys(pin);
			break;
		case PI_MODEL_LTIE_2:
		case PI_MODEL_ZERO_PLUS:
		case PI_MODEL_ZERO_PLUS_2:
		case PI_MODEL_3:
		case PI_MODEL_ZERO:
		case PI_MODEL_800:
		case PI_MODEL_4_LTS:
		case PI_MODEL_5:
		case PI_MODEL_5B:
			for (pin = 1 ; pin <= 26; pin += 2)
				readallPhys(pin);
			break;
		case PI_MODEL_R1_PLUS:
			for (pin = 1 ; pin <= 13; pin += 2)
				readallPhys(pin);
			break;
		case PI_MODEL_ZERO_2:
			for (pin = 1 ; pin <= 34; pin += 2)
				readallPhys(pin);
			break;
		default:
			printf ("Oops - unable to determine board type... model: %d\n", model);
			break ;
	}

	printf (" +------+-----+----------+--------+---+----++----+---+--------+----------+-----+------+\n");
	printf (" | GPIO | wPi |   Name   |  Mode  | V | Physical | V |  Mode  | Name     | wPi | GPIO |\n");

	switch (model)
	{
		case PI_MODEL_PC_2:
			printf (" +------+-----+----------+--------+---+  OPi PC2 +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_PRIME:
			printf (" +------+-----+----------+--------+---+   PRIME  +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_ZERO_PLUS:
			printf (" +------+-----+----------+--------+---+ ZEROPLUS +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_ZERO_PLUS_2:
			printf (" +------+-----+----------+--------+---+ZEROPLUS 2+---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_H3:
			printf (" +------+-----+----------+--------+---+OrangePiH3+---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_ZERO:
			printf (" +------+-----+----------+--------+---+  OPi H2  +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_WIN:
			printf (" +------+-----+----------+--------+---+  OPi Win +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_LTIE_2:
			printf (" +------+-----+----------+--------+---+  OPi H6  +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_3:
			printf (" +------+-----+----------+--------+---+   OPi 3  +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_ZERO_2:
			printf (" +------+-----+----------+--------+---+   H616   +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_ZERO_2_W:
			printf (" +------+-----+----------+--------+---+  ZERO2W  +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_RK3399:
			printf (" +------+-----+----------+--------+---+OPi RK3399+---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_4:
			printf (" +------+-----+----------+--------+---+OrangePi 4+---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_4_LTS:
			printf (" +------+-----+----------+--------+---+ OPi 4 LTS+---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_800:
			printf (" +------+-----+----------+--------+---+  opi800  +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_R1_PLUS:
			printf (" +------+-----+----------+--------+---+  R1 Plus +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_5:
			printf (" +------+-----+----------+--------+---+   OPI5   +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_5B:
			printf (" +------+-----+----------+--------+---+   PI5B   +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_5_PLUS:
			printf (" +------+-----+----------+--------+---+ PI5 PLUS +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_900:
			printf (" +------+-----+----------+--------+---+   PI900  +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_CM4:
			printf (" +------+-----+----------+--------+---+  PI CM4  +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_3B:
			printf (" +------+-----+----------+--------+---+   PI3B   +---+--------+----------+-----+------+\n");
			break;
		case PI_MODEL_3_PLUS:
			printf (" +------+-----+----------+--------+---+ PI3 PLUS +---+--------+----------+-----+------+\n");
			break;
		default:
			printf ("Oops - unable to determine board type... model: %d\n", model);
			break ;
	}

    wiringPiDebug = tmp;
}

/*
 * doReadall:
 *	Generic read all pins called from main program. Works out the Pi type
 *	and calls the appropriate function.
 *********************************************************************************
 */

void doReadall (void)
{
	int model = -1;

	if (wiringPiNodes != NULL)	// External readall
	{
		doReadallExternal () ;
		return ;
	}

	piBoardId (&model) ;

	OrangePiReadAll(model);
}


/*
 * doAllReadall:
 *	Force reading of all pins regardless of Pi model
 *********************************************************************************
 */

void doAllReadall (void)
{
  allReadall () ;
}


/*
 * doQmode:
 *	Query mode on a pin
 *********************************************************************************
 */

void doQmode (int argc, char *argv [])
{
  int pin ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s qmode pin\n", argv [0]) ;
    exit (EXIT_FAILURE) ;
  }

  pin = atoi (argv [2]) ;
  printf ("%s\n", alts [getAlt (pin)]) ;
}
