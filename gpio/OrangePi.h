#ifndef _ORANGEPI_H_
#define _ORANGEPI_H_

extern char *physNames[64];
extern int physToWpi[64];
extern int pinToGpioOrangePi[64];

extern void OrangePiReadAll(void);
extern void readallPhys(int physPin);

#endif
