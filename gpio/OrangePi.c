#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "OrangePi.h"
#include <wiringPi.h>

#ifdef CONFIG_ORANGEPI_PC2
int physToWpi[64] =
{
  -1,       // 0
  -1, -1,   // 1, 2
   0, -1,  //3, 4
   1, -1,  //5, 6
   2,  3,  //7, 8
  -1,  4, //9,10
   5,  6, //11,12
   7, -1, //13,14
   8,  9, //15,16
  -1, 10, //17,18
  11, -1, //19,20
  12, 13, //21,22
  14, 15, //23, 24
  -1, 16,  // 25, 26
  17, 18,   //27, 28
  19,  -1,  //29, 30
  20, 21,  //31, 32
  22, -1, //33, 34
  23, 24, //35, 36
  25, 26, //37, 38
  -1, 27, //39, 40

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
    -1, -1, -1, -1, -1, -1, -1, -1 // 56-> 63
};

char *physNames[64] =
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
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
   NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};
#endif /* PC2 */

#ifdef CONFIG_ORANGEPI_PRIME
int physToWpi[64] =
{
  -1,       // 0
  -1, -1,   // 1, 2
   0, -1,  //3, 4
   1, -1,  //5, 6
   2,  3,  //7, 8
  -1,  4, //9,10
   5,  6, //11,12
   7, -1, //13,14
   8,  9, //15,16
  -1, 10, //17,18
  11, -1, //19,20
  12, 13, //21,22
  14, 15, //23, 24
  -1, 16,  // 25, 26
  17, 18,   //27, 28
  19,  -1,  //29, 30
  20, 21,  //31, 32
  22, -1, //33, 34
  23, 24, //35, 36
  25, 26, //37, 38
  -1, 27, //39, 40

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
    -1, -1, -1, -1, -1, -1, -1, -1 // 56-> 63
};

char *physNames[64] =
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
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
   NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};
#endif /* PRIME */

#ifdef CONFIG_ORANGEPI_ZEROPLUS
int physToWpi [64] = //return wiringPI pin
{
  -1,       // 0
  -1, -1,   // 1, 2
   0, -1,  //3, 4
   1, -1,  //5, 6
   2, 3,  //7, 8
  -1, 4, //9,10
  5, 6, //11,12
  7, -1, //13,14
  8, 9, //15,16
  -1, 10, //17,18
  11, -1, //19,20
   12, 13, //21,22
  14, 15, //23, 24
  -1,  16,  // 25, 26

  -1, -1,   //27, 28
  -1, -1,  //29, 30
  -1, -1,  //31, 32
  -1, -1, //33, 34
  -1, -1, //35, 36
  -1, -1, //37, 38
  -1, -1, //39, 40
  -1, -1, //41, 42
// Padding:

  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
  -1, -1, -1, -1, -1, -1, -1,   // ... 63
};

char *physNames[64] =
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

       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL,
};
#endif /* ZEROPLUS */

#ifdef CONFIG_ORANGEPI_2G_IOT
char *physNames[64] =
{
    NULL,

 "    3.3V", "5V      ",
 "   SDA.0", "5V      ",
 "   SCL.0", "GND     ",
 "  GPIO.7", "TXD.2   ",
 "     GND", "RXD.2   ",
 "   RXD.1", "GPIO.1  ",
 "   TXD.1", "GND     ",
 "   CTS.1", "GPIO.4  ",
 "    3.3V", "GPIO.5  ",
 "   SDI.2", "GND     ",
 "  SDIO.2", "RTS1    ",
 "  SCLK.2", "CE.0    ",
 "     GND", "CE.1    ",
 "   SDA.1", "SCL.1   ",
 " GPIO.21", "GND     ",
 " GPIO.22", "RTS.2   ",
 " GPIO.23", "GND     ",
 " GPIO.24", "CTS.2   ",
 " GPIO.25", "SCL.2   ",
 "     GND", "SDA.2   ",
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
   NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};

int physToWpi[64] =
{
    -1,        // 0
    -1,  -1,   // 1, 2
    8,   -1,   // 3, 4
    9,   -1,   // 5, 6
    7,   15,   // 7, 8
    -1,  16,   // 9, 10
    0,    1,   //11, 12
    2,   -1,   //13, 14
    3,    4,   //15, 16
    -1,   5,   //17, 18
    12,  -1,   //19, 20
    13,   6,   //21, 22
    14,  10,   //23, 24
    -1,  11,   //25, 26
    30,  31,   //27, 28
    21,  -1,   //29, 30
    22,  26,   //31, 32
    23,  -1,   //33, 34
    24,  27,   //35, 36
    25,  28,   //37, 38
    -1,  29,   //39, 40
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
    -1, -1, -1, -1, -1, -1, -1, -1 // 56-> 63
};
#endif

#ifdef CONFIG_ORANGEPI_WIN
char *physNames [64] =
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

       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
   NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};

int physToWpi [64] =
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

  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
  -1, -1, -1, -1, -1, -1, -1, -1 // 56-> 63
};
#endif

#ifdef CONFIG_ORANGEPI_H3
int physToWpi[64] =
{
	-1,	//0
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
	-1, -1, -1, -1, -1, -1, -1, 				// ... 63
};

char *physNames[64] =
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

       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL, 
       NULL, NULL,
       NULL, NULL,
	  NULL,
};
#endif /* H3 */

#if CONFIG_ORANGEPI_ZEROPLUS2_H3 || CONFIG_ORANGEPI_ZEROPLUS2_H5
int physToWpi [64] = //return wiringPI pin
{
  -1,       // 0
  -1, -1,   // 1, 2
   0, -1,  //3, 4
   1, -1,  //5, 6
   2, 3,  //7, 8
  -1, 4, //9,10
  5, 6, //11,12
  7, -1, //13,14
  8, 9, //15,16
  -1, 10, //17,18
  11, -1, //19,20
   12, 13, //21,22
  14, 15, //23, 24
  -1,  16,  // 25, 26

  -1, -1,   //27, 28
  -1, -1,  //29, 30
  -1, -1,  //31, 32
  -1, -1, //33, 34
  -1, -1, //35, 36
  -1, -1, //37, 38
  -1, -1, //39, 40
  -1, -1, //41, 42
// Padding:

  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
  -1, -1, -1, -1, -1, -1, -1,   // ... 63
};

char *physNames[64] =
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

       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL,
};
#endif /* ZERO_PLUS2 */


#ifdef CONFIG_ORANGEPI_ZERO
int physToWpi [64] =
{
  -1,        // 0

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

  -1, -1,   //27, 28
  -1, -1,   //29, 30
  -1, -1,   //31, 32
  -1, -1,   //33, 34
  -1, -1,   //35, 36
  -1, -1,   //37, 38
  -1, -1,   //39, 40
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
  -1, -1, -1, -1, -1, -1, -1, -1 // 56-> 63
};

char *physNames[64] =
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

       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL,
};
#endif

#ifdef CONFIG_ORANGEPI_LITE2
int physToWpi [64] = 
{
	-1, 		  // 0
	-1, -1, 	  // 1, 2
	 0, -1,
	 1, -1, 	  
	 2,  3, 	  //7, 8
	-1,  4, 	  
	 5,  6, 	  //11, 12
	 7, -1, 	  
	 8,  9, 	  //15, 16
	-1, 10, 	  
	11, -1, 	  //19, 20
	12, 13, 	  
	14, 15, 	  //23, 24
	-1, 16, 	  // 25, 26
	
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
	-1, -1, -1, -1, -1, -1, -1,   // ... 63

};

char *physNames [64] = 
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

       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL,
};
#endif

#ifdef CONFIG_ORANGEPI_3
int physToWpi [64] = 
{
	-1, 		  // 0
	-1, -1, 	  // 1, 2
	 0, -1,
	 1, -1, 	  
	 2,  3, 	  //7, 8
	-1,  4, 	  
	 5,  6, 	  //11, 12
	 7, -1, 	  
	 8,  9, 	  //15, 16
	-1, 10, 	  
	11, -1, 	  //19, 20
	12, 13, 	  
	14, 15, 	  //23, 24
	-1, 16, 	  // 25, 26
	
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
	-1, -1, -1, -1, -1, -1, -1,   // ... 63

};

char *physNames [64] = 
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

       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL,
};
#endif

#ifdef CONFIG_ORANGEPI_ZERO2
int physToWpi [64] = 
{
	-1, 		  // 0
	-1, -1, 	  // 1, 2
	 0, -1,
	 1, -1, 	  
	 2,  3, 	  //7, 8
	-1,  4, 	  
	 5,  6, 	  //11, 12
	 7, -1, 	  
	 8,  9, 	  //15, 16
	-1, 10, 	  
	11, -1, 	  //19, 20
	12, 13, 	  
	14, 15, 	  //23, 24
	-1, 16, 	  // 25, 26
	17, -1,
	18, -1,
	19, -1,
	20, -1,
	
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // ... 56
	-1,  // ... 63

};

char *physNames [64] = 
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
 "     PC1", "        ",
 "    PI16", "        ",
 "     PI6", "        ",
 "    PH10", "        ",


       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL,
};
#endif

#ifdef CONFIG_ORANGEPI_RK3399
int pinToGpioOrangePi [64] =
{
	43,  44,      // 0, 1
	64,  148,      // 2, 3
	147, 80,      // 4  5
	65,  81,      // 6, 7
	82,  66,      // 8, 9
	67,  39,      //10,11
	40,  83,      //12,13
	41,  42,      //14,15
	133, 154,      //16,17
	50,  68,      //18,19
	69,  76,      //20,21
	70,  71,      //22,23
	73,  72,      //24,25
	74,  75,      //26,27
	
	-1,  -1,    //28,29
	-1,  -1,    //30,31

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 47
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,// ... 63	
};

int physToWpi[64] =
{
	-1,		// 0
	-1, -1,	// 1, 2
	0, -1,  //3, 4
	1, -1,  //5, 6
	2, 3,  //7, 8
	-1, 4, //9,10
	5, 6, //11,12
	7, -1, //13,14
	8, 9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23, 24
	-1,  16,	// 25, 26

	17,	18,   //27, 28
	19,  -1,	//29, 30
	20,  21,	//31, 32
	22, -1, //33, 34
	23, 24, //35, 36
	25, 26, //37, 38
	-1, 27, //39, 40
	// Padding:

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 56
	-1, -1, -1, -1, -1, -1, -1,	// ... 63
};

char *physNames[64] =
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
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
  	   NULL, NULL,
       NULL, NULL,
       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};
#endif


#ifdef CONFIG_ORANGEPI_800
int pinToGpioOrangePi [64] =
{
	43,   44, //0,1
	150,  39, //2,3
	40,  148, //4,5
	152, 147, //6,7
	149,  64, //8,9
	65,   74, //10,11
	73,  153, //12,13
	75,   76, //14,15
	154,  -1, //16,17
	-1,   -1, //18,19
	-1,   -1, //20,21
	-1,   -1, //22,23
	-1,   -1, //24,25
	-1,   -1, //26,27
	-1,   -1, //28,29
	-1,   -1, //30,31
	-1,   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 47
	-1,   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,// ... 63
};

int physToWpi[64] =
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
	-1, -1, -1, -1, -1, -1, -1,     // ... 63
};

char *physNames[64] =
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
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};
#endif

#ifdef CONFIG_ORANGEPI_4
int pinToGpioOrangePi [64] =
{
	64,  65,      // 0, 1
	150, 145,      // 2, 3
	144, 33,      // 4  5
	50,  35,      // 6, 7
	92,  54,      // 8, 9
	55,  40,      //10,11
	
	39,  56,      //12,13
	
	41,  42,      //14,15
	149, 64,      //16,17
	65,  -1,      //18,19
	-1,  -1,      //20,21
	-1,  -1,      //22,23
	-1,  -1,      //24,25

	
	-1,  -1,      //26,27
	
	-1,  -1,    //28,29
	-1,   -1,    //30,31

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 47
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,// ... 63	
};
int physToWpi[64] =
{
	-1,		// 0
	-1, -1,	// 1, 2
	0, -1,  //3, 4
	1, -1,  //5, 6
	2, 3,  //7, 8
	-1, 4, //9,10
	5, 6, //11,12
	7, -1, //13,14
	8, 9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23, 24
	-1,  16,	// 25, 26

	17,  18,   //27, 28
	-1,  -1,	//29, 30
	-1,  -1,	//31, 32
	-1, -1, //33, 34
	-1, -1, //35, 36
	-1, -1, //37, 38
	-1, -1, //39, 40
	// Padding:

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 56
	-1, -1, -1, -1, -1, -1, -1,	// ... 63
};

char *physNames[64] =
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
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
  	   NULL, NULL,
       NULL, NULL,
       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};
#endif

#ifdef CONFIG_ORANGEPI_4_LTS
int pinToGpioOrangePi [64] =
{
	52,  53,      // 0, 1
	150, 145,      // 2, 3
	144, 33,      // 4  5
	50,  35,      // 6, 7
	92,  54,      // 8, 9
	55,  40,      //10,11
	39,  56,      //12,13
	41,  42,      //14,15
	149, 64,      //16,17
	65,  -1,      //18,19
	-1,  -1,      //20,21
	-1,  -1,      //22,23
	-1,  -1,      //24,25
	-1,  -1,      //26,27
	-1,  -1,    //28,29
	-1,   -1,    //30,31

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 47
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,// ... 63
};
int physToWpi[64] =
{
	-1,		// 0
	-1, -1,	// 1, 2
	0, -1,  //3, 4
	1, -1,  //5, 6
	2, 3,  //7, 8
	-1, 4, //9,10
	5, 6, //11,12
	7, -1, //13,14
	8, 9, //15,16
	-1, 10, //17,18
	11, -1, //19,20
	12, 13, //21,22
	14, 15, //23, 24
	-1, 16,	// 25, 26

	-1,  -1,   //27, 28
	-1,  -1,	//29, 30
	-1,  -1,	//31, 32
	-1, -1, //33, 34
	-1, -1, //35, 36
	-1, -1, //37, 38
	-1, -1, //39, 40
	// Padding:

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 56
	-1, -1, -1, -1, -1, -1, -1,	// ... 63
};

char *physNames[64] =
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
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};
#endif

#ifdef CONFIG_ORANGEPI_R1PLUS
int physToWpi[64] =
{
	-1,     //0
	-1, -1, //1, 2
	0, 1,  //3, 4
	2, 3,  //5, 6
	-1, -1, //7, 8
	-1, 4,  //9, 10
	5, 6,  //11, 12
	7, -1,  //13, 14

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
	// Padding:

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1,  // ... 63
};

char *physNames[64] =
{
	NULL,

	"5V      ", "GND     ",
	"SDA.0   ", "SCK.0   ",
	"TXD.1   ", "RXD.1   ",
	"        ", "        ",
	"        ", "GPIO3_C0",
	"CTS.1   ", "RTS.1   ",
	"GPIO2_A2", "        ",
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL, NULL,
	NULL,NULL,NULL,NULL,NULL,
};
#endif




/*
 * ReadAll 
 */
void OrangePiReadAll(void)
{
    int pin;
	int tmp = wiringPiDebug;
    wiringPiDebug = FALSE;

#ifdef CONFIG_ORANGEPI_PC2
    printf (" +------+-----+----------+------+---+  OPi PC2 +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_PRIME
    printf (" +------+-----+----------+------+---+   PRIME  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZEROPLUS
    printf (" +------+-----+----------+------+---+ ZEROPLUS +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZEROPLUS2_H5
    printf (" +------+-----+----------+------+---+ZEROPLUS 2+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_2G_IOT
    printf (" +------+-----+----------+------+---+  2G-IOT  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_H3
    printf (" +------+-----+----------+------+---+OrangePiH3+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZEROPLUS2_H3
    printf (" +------+-----+----------+------+---+ZEROPLUS 2+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZERO
    printf (" +------+-----+----------+------+---+  OPi H2  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_WIN
    printf (" +------+-----+----------+------+---+ OPi Win  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_LITE2 
    printf (" +------+-----+----------+------+---+  OPi H6  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_3
    printf (" +------+-----+----------+------+---+   OPi 3  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZERO2
    printf (" +------+-----+----------+------+---+  Zero 2  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_RK3399
    printf (" +------+-----+----------+------+---+OPi RK3399+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_4
    printf (" +------+-----+----------+------+---+OrangePi 4+---+---+--+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_4_LTS
    printf (" +------+-----+----------+------+---+OPi 4 LTS +---+---+--+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_800
    printf (" +------+-----+----------+------+---+   800    +---+---+--+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_R1PLUS
    printf (" +------+-----+----------+------+---+ R1 Plus  +---+---+--+----------+-----+------+\n");
#endif

    printf (" | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |\n");
    printf (" +------+-----+----------+------+---+----++----+---+------+----------+-----+------+\n");

#if defined CONFIG_ORANGEPI_H3 || defined CONFIG_ORANGEPI_RK3399 || CONFIG_ORANGEPI_4 || CONFIG_ORANGEPI_PC2 || CONFIG_ORANGEPI_PRIME || CONFIG_ORANGEPI_WIN
    for (pin = 1 ; pin <= 40; pin += 2)

#elif CONFIG_ORANGEPI_LITE2 || CONFIG_ORANGEPI_ZEROPLUS2_H3 || CONFIG_ORANGEPI_3 || CONFIG_ORANGEPI_ZERO || CONFIG_ORANGEPI_ZEROPLUS || CONFIG_ORANGEPI_R1 || CONFIG_ORANGEPI_ZEROPLUS2_H5 || CONFIG_ORANGEPI_800 || CONFIG_ORANGEPI_4_LTS
	for (pin = 1 ; pin <= 26; pin += 2)

#elif CONFIG_ORANGEPI_R1PLUS
	for (pin = 1 ; pin <= 13; pin += 2)

#elif CONFIG_ORANGEPI_ZERO2
	for (pin = 1 ; pin <= 34; pin += 2)
#endif
        readallPhys(pin);

    printf (" +------+-----+----------+------+---+----++----+---+------+----------+-----+------+\n");
    printf (" | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |\n");

#ifdef CONFIG_ORANGEPI_PC2
    printf (" +------+-----+----------+------+---+  OPi PC2 +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_PRIME
    printf (" +------+-----+----------+------+---+   PRIME  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZEROPLUS
    printf (" +------+-----+----------+------+---+ ZEROPLUS +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZEROPLUS2_H5
    printf (" +------+-----+----------+------+---+ZEROPLUS 2+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_2G_IOT
    printf (" +------+-----+----------+------+---+  2G-IOT  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_H3
    printf (" +------+-----+----------+------+---+OrangePiH3+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZEROPLUS2_H3
    printf (" +------+-----+----------+------+---+ZEROPLUS 2+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZERO
    printf (" +------+-----+----------+------+---+  OPi H2  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_WIN
    printf (" +------+-----+----------+------+---+ OPi Win  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_LITE2 
    printf (" +------+-----+----------+------+---+  OPi H6  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_3
    printf (" +------+-----+----------+------+---+   OPi 3  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZERO2
    printf (" +------+-----+----------+------+---+  Zero 2  +---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_RK3399
    printf (" +------+-----+----------+------+---+OPi RK3399+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_4
    printf (" +------+-----+----------+------+---+OrangePi 4+---+---+--+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_4_LTS
    printf (" +------+-----+----------+------+---+OPi 4 LTS +---+---+--+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_800
    printf (" +------+-----+----------+------+---+   800    +---+---+--+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_R1PLUS
    printf (" +------+-----+----------+------+---+ R1 Plus  +---+---+--+----------+-----+------+\n");
#endif

    wiringPiDebug = tmp;
}
