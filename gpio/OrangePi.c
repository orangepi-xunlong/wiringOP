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

 "    3.3v", "5v      ",
 "   SDA.0", "5v      ",
 "   SCL.0", "GND     ",
 "    PWM1", "PC05    ",
 "     GND", "PC06    ",
 "   RxD.2", "PD14    ",
 "   TxD.2", "GND     ",
 "   CTS.2", "PC04    ",
 "    3.3v", "PC07    ",
 "  MOSI.1", "GND     ",
 "  MISO.1", "RTS.2   ",
 "  SCLK.1", "CS.1    ",
 "     GND", "PA21    ",
 "   SDA.1", "SCL.1   ",
 "    PA07", "GND     ",
 "    PA08", "RTS.1   ",
 "    PA09", "GND     ",
 "    PA10", "CTS.1   ",
 "    PD11", "TxD.1   ",
 "     GND", "RxD.1   ",
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

 "    3.3v", "5v      ",
 "   SDA.0", "5v      ",
 "   SCL.0", "GND     ",
 "    PWM1", "PC05    ",
 "     GND", "PC06    ",
 "   RxD.2", "PD14    ",
 "   TxD.2", "GND     ",
 "   CTS.2", "PC04    ",
 "    3.3v", "PC07    ",
 "  MOSI.1", "GND     ",
 "  MISO.1", "RTS.2   ",
 "  SCLK.1", "CS.1    ",
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

  "    3.3v", "5v      ",
  "   SDA.0", "5v      ",
  "   SCL.0", "GND     ",
  "     PA6", "TxD.1   ",
  "     GND", "RxD.1   ",
  "   RXD.2", "PA07    ",
  "   TXD.2", "GND     ",
  "   CTS.2", "SDA.1   ",
  "    3.3v", "SCL.1   ",
  "  MOSI.1", "GND     ",
  "  MISO.1", "RTS.2   ",
  "  SCLK.1", "CS.1    ",
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

 "    3.3v", "5v      ",
 "   SDA.0", "5V      ",
 "   SCL.0", "0v      ",
 "  GPIO.7", "TxD2    ",
 "      0v", "RxD2    ",
 "    RxD1", "GPIO.1  ",
 "    TxD1", "0v      ",
 "    CTS1", "GPIO.4  ",
 "    3.3v", "GPIO.5  ",
 " SPI2_DI", "0v      ",
 "SPI2_DIO", "RTS1    ",
 "SPI2_CLK", "SPI2_CS0",
 "      0v", "SPI2_CS1",
 "   SDA.1", "SCL.1   ",
 " GPIO.21", "0v      ",
 " GPIO.22", "RTS2    ",
 " GPIO.23", "0v      ",
 " GPIO.24", "CTS2    ",
 " GPIO.25", "SCL.2   ",
 "      0v", "SDA.2   ",
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

#ifdef CONFIG_ORANGEPI_A64
char *physNames [64] =
{
  NULL,

 "    3.3v", "5v      ",
 "   SDA.1", "5V      ",
 "   SCL.1", "0v      ",
 "  GPIO.7", "S_TX    ",
 "      0v", "S_RX    ",
 "    RxD3", "GPIO.1  ",
 "    TxD3", "0v      ",
 "    CTS3", "GPIO.4  ",
 "    3.3v", "GPIO.5  ",
 "    MOSI", "0v      ",
 "    MISO", "RTS3    ",
 "    SCLK", "CE0     ",
 "      0v", "GPIO.11 ",
 "   SDA.2", "SCL.2   ",
 " GPIO.21", "0v      ",
 " GPIO.22", "RTS2    ",
 " GPIO.23", "0v      ",
 " GPIO.24", "CTS2    ",
 " GPIO.25", "TxD2    ",
 "      0v", "RxD2    ",
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
  "GPIO.17", "GPIO.18",
  "GPIO.19", "GPIO.20",
   NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};

int physToWpi [64] =
{
  -1,        // 0
  -1,  -1,   // 1, 2
   8,  -1,   // 3, 4
   9,  -1,   // 5, 6
   7,  15,   // 7, 8
  -1,  16,   // 9, 10
   0,   1,   //11, 12
   2,  -1,   //13, 14
   3,   4,   //15, 16
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

#ifdef CONFIG_ORANGEPI_H3
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
	28, 29, //41, 42
	// Padding:

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 56
	-1, -1, -1, -1, -1, -1, -1,	// ... 63
};

char *physNames[64] =
{
  NULL,

 "    3.3v", "5v      ",
 "   SDA.0", "5V      ",
 "   SCL.0", "0v      ",
 "     PA6", "TxD3    ",
 "      0v", "RxD3    ",
 "    RxD2", "PD14    ",
 "    TxD2", "0v      ",
 "    CTS2", "PC04    ",
 "    3.3v", "PC07    ",
 "    MOSI", "0v      ",
 "    MISO", "RTS2    ",
 "    SCLK", "CE0     ",
 "      0v", "PA21    ",
 "   SDA.1", "SCL.1   ",
 "    PA07", "0v      ",
 "    PA08", "RTS1    ",
 "    PA09", "0v      ",
 "    PA10", "CTS1    ",
 "    PA20", "TxD1    ",
 "      0v", "RxD1    ",
 "    PA04", "PA05    ",
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
       NULL, NULL,
  	   NULL, NULL,
       NULL, NULL,
   NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};
#endif

#ifdef CONFIG_ORANGEPI_H3_ZEROPLUS2
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

  "    3.3v", "5v      ",
  "   SDA.0", "5v      ",
  "   SCL.0", "0v      ",
  "     PA6", "TxD2    ",
  "      0v", "RxD2    ",
  "   S-TWI", "PD11    ",
  "   S-SDA", "0v      ",
  "    CTS2", "SDA.1   ",
  "    3.3v", "SCL.1   ",
  "  MOSI.1", "0v      ",
  "  MISO.1", "RTS2    ",
  "  SCLK.1", "CS.1    ",
  "      0v", "PD14    ",

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

 "    3.3v", "5v      ",
 "   SDA.0", "5V      ",
 "   SCL.0", "0v      ",
 "   PWM.1", "TXD.1   ",
 "     GND", "RxD.1   ",
 "   RxD.2", "PA07    ",
 "   TxD.2", "GND     ",
 "   CTS.2", "SDA.1   ",
 "    3.3v", "SCK.1   ",
 "  MOSI.1", "GND     ",
 "  MISO.1", "RTS.2   ",
 "  SCLK.1", "CS.1    ",
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

 "    3.3v", "5v      ",
 "   SDA.1", "5V      ",
 "   SCL.1", "0v      ",
 "    PWM1", "PD21    ",
 "      0v", "PD22    ",
 "    RxD3", "PC09    ",
 "    TxD3", "0v      ",
 "    CTS3", "PC08    ",
 "    3.3v", "PC07    ",
 "  MOSI.0", "0v      ",
 "  MISO.0", "RTS3    ",
 "  SCLK.0", "CE.0    ",
 "      0v", "PH03    ",

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

 "    3.3v", "5v      ",
 "   SDA.0", "5V      ",
 "   SCL.0", "0v      ",
 "    PWM0", "PL02    ",
 "      0v", "PL03    ",
 "    RxD3", "PD18    ",
 "    TxD3", "0v      ",
 "    PL10", "PD15    ",
 "    3.3v", "PD16    ",
 "  MOSI.1", "0v      ",
 "  MISO.1", "PD21    ",
 "  SCLK.1", "CE.1    ",
 "      0v", "PL08    ",

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
	133,  154,      //16,17
	50,  68,      //18,19
	69,  76,      //20,21
	70,  71,      //22,23
	73,  72,      //24,25
	74,  75,      //26,27
	
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
  "    3.3v", "5v      ",
  "   SDA.0", "5V      ",
  "   SCL.0", "0v      ",
  "   GPIO4", "Tx      ",
  "      0v", "Rx      ",
  "  GPIO17", "GPIO18  ",
  "  GPIO27", "0v      ",
  "  GPIO22", "GPIO23  ",
  "    3.3v", "GPIO24  ",
  "    MOSI", "0v      ",
  "    MISO", "GPIO25  ",
  "    SCLK", "CS0     ",
  "      0v", "CS1     ",
  "    DNP1", "DNP2    ",
  "   GPIO5", "0v      ",
  "   GPIO6", "GPIO12  ",
  "  GPIO13", "0v      ",
  "  GPIO19", "GPIO16  ",
  "  GPIO26", "GPIO20  ",
  "      0v", "GPIO21  ",
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
  "    3.3v", "5v      ",
  "I2C2_SDA", "5V      ",
  "I2C2_SCL", "0v      ",
  "    PWM1", "I2C3_SCL",
  "      0v", "I2C3_SDA",
  "GPIO1_A1", "GPIO1_C2",
  "GPIO1_A3", "0v      ",
  "GPIO2_D4", "GPIO1_C6",
  "    3.3v", "GPIO1_C7",
  "SPI1_TXD", "0v      ",
  "SPI1_RXD", "GPIO1_D0",
  "SPI1_CLK", "SPI1_CS ",
  "      0v", "GPIO4_C5",
  "I2C2_SDA", "I2C2_SCL",
  " I2S0_RX", "0v      ",
  " I2S0_TX", "I2S_CLK ",
  "I2S0_SCK", "0v      ",
  "I2S0_SI0", "I2S0_SO0",
  "I2S0_SI1", "I2S0_SI2",
  "      0v", "I2S0_SI3",
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
#elif CONFIG_ORANGEPI_2G_IOT
    printf (" +-----+-----+----------+------+---+OrangePi 2G-IOT+---+---+------+---------+-----+--+\n");
#elif CONFIG_ORANGEPI_H3 || CONFIG_ORANGEPI_H3_ZEROPLUS2 
    printf (" +------+-----+----------+------+---+OrangePiH3+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZERO
    printf (" +------+-----+----------+------+---+OrangePi Zero+---+---+------+---------+-----+--+\n");
#elif CONFIG_ORANGEPI_A64
    printf (" +------+-----+----------+------+---+OrangePi Win/Win+ +---+---+------+---------+-----+--+\n");
#elif CONFIG_ORANGEPI_LITE2 || CONFIG_ORANGEPI_3
    printf (" +------+-----+----------+------+---+OrangePiH6+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_RK3399
	printf (" +------+-----+----------+------+---+OrangePi RK3399---+---+------+---------+-----+--+\n");
#elif CONFIG_ORANGEPI_4
	printf (" +------+-----+----------+------+---+OrangePi 4---+---+------+---------+-----+--+\n");

#endif
    printf (" | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |\n");
    printf (" +------+-----+----------+------+---+----++----+---+------+----------+-----+------+\n");

#if defined CONFIG_ORANGEPI_H3 || defined CONFIG_ORANGEPI_RK3399 || CONFIG_ORANGEPI_4 || CONFIG_ORANGEPI_PC2 || CONFIG_ORANGEPI_PRIME
    for (pin = 1 ; pin <= 40; pin += 2)

#elif CONFIG_ORANGEPI_LITE2 || CONFIG_ORANGEPI_H3_ZEROPLUS2 || CONFIG_ORANGEPI_3 || CONFIG_ORANGEPI_ZERO || CONFIG_ORANGEPI_ZEROPLUS || CONFIG_ORANGEPI_H5_ZEROPLUS2 || CONFIG_ORANGEPI_R1
	for (pin = 1 ; pin <= 26; pin += 2)
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
#elif CONFIG_ORANGEPI_2G_IOT
    printf (" +------+-----+----------+------+---+OrangePi 2G-IOT+---+------+----------+-----+-----+\n");
#elif CONFIG_ORANGEPI_H3 || CONFIG_ORANGEPI_H3_ZEROPLUS2
    printf (" +------+-----+----------+------+---+OrangePiH3+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_ZERO
    printf (" +------+-----+----------+------+---+OrangePi Zero+---+------+----------+-----+-----+\n");
#elif CONFIG_ORANGEPI_A64
    printf (" +------+-----+----------+------+---+OrangePi Win/Win+ +---+------+----------+-----+-----+\n");
#elif CONFIG_ORANGEPI_LITE2 || CONFIG_ORANGEPI_3
    printf (" +------+-----+----------+------+---+OrangePiH6+---+------+----------+-----+------+\n");
#elif CONFIG_ORANGEPI_RK3399
	printf (" +------+-----+----------+------+---+OrangePi RK3399---+---+------+---------+-----+--+\n");
#elif CONFIG_ORANGEPI_4
	printf (" +------+-----+----------+------+---+OrangePi 4---+---+------+---------+-----+--+\n");


#endif
    wiringPiDebug = tmp;
}
