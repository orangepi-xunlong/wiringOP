/*
 * wiringPi.h:
 *	Arduino like Wiring library for the Raspberry Pi.
 *	Copyright (c) 2012-2017 Gordon Henderson
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

#ifndef	__WIRING_PI_H__
#define	__WIRING_PI_H__

// C doesn't have true/false by default and I can never remember which
//	way round they are, so ...
//	(and yes, I know about stdbool.h but I like capitals for these and I'm old)

#ifndef	TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(!TRUE)
#endif

// GCC warning suppressor

#define	UNU	__attribute__((unused))
#define MAX_PIN_NUM        (0x40)

#define PI_MODEL_A               0
#define PI_MODEL_B               1
#define PI_MODEL_AP              2
#define PI_MODEL_BP              3
#define PI_MODEL_2               4
#define PI_ALPHA                 5
#define PI_MODEL_CM              6
#define PI_MODEL_07              7
#define PI_MODEL_CM3            10
#define PI_MODEL_ZERO_W         12
#define PI_MODEL_3P             13

#define PI_VERSION_1            0
#define PI_VERSION_1_1          1
#define PI_VERSION_1_2          2
#define PI_VERSION_2            3

#define PI_MAKER_SONY           0
#define PI_MAKER_EGOMAN         1
#define PI_MAKER_EMBEST         2
#define PI_MAKER_UNKNOWN        3

/*********** Allwinner H3 *************/
#define H3_GPIO_BASE_ADDR                     0x01C20000U
#define H3_R_GPIO_BASE_ADDR                   0x01F02000U
/*********** Allwinner H3 *************/

/*********** Allwinner H6 *************/
#define H6_GPIO_BASE_ADDR                     0x0300B000U
#define H6_R_GPIO_BASE_ADDR                   0x07022000U
/*********** Allwinner H6 *************/

typedef struct {
	unsigned int gpio_base_addr;
	unsigned int r_gpio_base_addr;
	unsigned int * gpio;
	unsigned int * r_gpio;
	unsigned int gpio_base_offset;
	unsigned int r_gpio_base_offset;
} sunxi_gpio_info;


/*********** Rockchip RK3588 *************/

//gpio0~gpio4 register base addr
#define RK3588_GPIO0_BASE 							0xfd8a0000U
#define RK3588_GPIO1_BASE 							0xfec20000U
#define RK3588_GPIO2_BASE 							0xfec30000U
#define RK3588_GPIO3_BASE 							0xfec40000U
#define RK3588_GPIO4_BASE 							0xfec50000U

//gpio offset
#define RK3588_GPIO_SWPORT_DR_L_OFFSET 				0x00U
#define RK3588_GPIO_SWPORT_DR_H_OFFSET 				0x04U
#define RK3588_GPIO_SWPORT_DDR_L_OFFSET 			0x08U
#define RK3588_GPIO_SWPORT_DDR_H_OFFSET 			0x0cU
#define RK3588_GPIO_EXT_PORT_OFFSET					0x70U

//CRU clock-controller base addr
#define RK3588_CRU_BASE 							0xfd7c0000U
#define RK3588_CRU_GATE_CON16_OFFSET 			    0x0840U    //for gpio1 bit 14 15 	30 31
#define RK3588_CRU_GATE_CON17_OFFSET 			    0x0844U    //for gpio2/3/4 - bit 0 1 2 3 4 5 	16 17 18 19 20 21

#define RK3588_PMU1CRU_BASE 						0xfd7f0000U
#define RK3588_PMU1CRU_GATE_CON5_OFFSET 			0x0814U		//for gpio0 - bit 5 6 	21 22

#define RK3588_GPIO_NUM                          	(0x40)
#define RK3588_GPIO_BIT(x)                        	(1UL << (x))

//gpio iomux
#define RK3588_PMU1_IOC_BASE						0xfd5f0000U
#define RK3588_PMU1_IOC_GPIO0A_IOMUX_SEL_L			0x00U	//gpio0a0~gpio0b3
#define RK3588_PMU1_IOC_GPIO0A_IOMUX_SEL_H			0x04U	//gpio0a4~gpio0b7
#define RK3588_PMU1_IOC_GPIO0B_IOMUX_SEL_L			0x08U	//gpio0b0~gpio0b3

#define RK3588_PMU2_IOC_BASE						0xfd5f4000U
#define RK3588_PMU2_IOC_GPIO0B_IOMUX_SEL_H			0x00U	//gpio0a5~gpio0b7
#define RK3588_PMU2_IOC_GPIO0C_IOMUX_SEL_L			0x04U	//gpio0a0~gpio0b3
#define RK3588_PMU2_IOC_GPIO0C_IOMUX_SEL_H			0x08U	//gpio0a4~gpio0b7
#define RK3588_PMU2_IOC_GPIO0D_IOMUX_SEL_L			0x0cU	//gpio0a0~gpio0b3
#define RK3588_PMU2_IOC_GPIO0D_IOMUX_SEL_H			0x10U	//gpio0a4~gpio0b6

#define RK3588_BUS_IOC_BASE							0xfd5f8000U

//gpio pull up/down
#define RK3588_VCCIO1_4_IOC_BASE					0xfd5f9000U
#define RK3588_VCCIO3_5_IOC_BASE					0xfd5fa000U
#define RK3588_VCCIO6_IOC_BASE						0xfd5fc000U

#define RK3588_PMU1_IOC_GPIO0A_P					0x0020U
#define RK3588_PMU1_IOC_GPIO0B_P					0x0024U
#define RK3588_PMU2_IOC_GPIO0B_P					0x0028U
#define RK3588_PMU2_IOC_GPIO0C_P					0x002cU
#define RK3588_PMU2_IOC_GPIO0D_P					0x0030U
#define RK3588_VCCIO1_4_IOC_GPIO1A_P				0x0110U
#define RK3588_VCCIO3_5_IOC_GPIO2A_P				0x0120U
#define RK3588_VCCIO6_IOC_GPIO4A_P					0x0140U

typedef struct {
	unsigned int * gpio0_base;
	unsigned int * gpio1_base;
	unsigned int * gpio2_base;
	unsigned int * gpio3_base;
	unsigned int * gpio4_base;

	unsigned int * pmu1_ioc_base;
	unsigned int * pmu2_ioc_base;
	unsigned int * bus_ioc_base;

	unsigned int * cur_base;
	unsigned int * pmu1cur_base;

	unsigned int * vccio1_4_ioc_base;
	unsigned int * vccio3_5_ioc_base;
	unsigned int * vccio6_ioc_base;
} rk3588_soc_info;

/*********** Rockchip RK3588 *************/

/*********** Rockchip RK3566 *************/

//gpio0~gpio4 register base addr
#define RK3566_GPIO0_BASE 							0xfdd60000U
#define RK3566_GPIO1_BASE 							0xfe740000U
#define RK3566_GPIO2_BASE 							0xfe750000U
#define RK3566_GPIO3_BASE 							0xfe760000U
#define RK3566_GPIO4_BASE 							0xfe770000U

//gpio offset
#define RK3566_GPIO_SWPORT_DR_L_OFFSET 				0x00U
#define RK3566_GPIO_SWPORT_DR_H_OFFSET 				0x04U
#define RK3566_GPIO_SWPORT_DDR_L_OFFSET 			0x08U
#define RK3566_GPIO_SWPORT_DDR_H_OFFSET 			0x0cU
#define RK3566_GPIO_EXT_PORT_OFFSET					0x70U

#define RK3566_PMU_GRF_BASE							0xfdc20000U
#define RK3566_SYS_GRF_BASE							0xfdc60000U
#define RK3566_PMU_CRU_BASE							0xfdd00000U
#define RK3566_CRU_BASE								0xFdd20000U

// clock
#define RK3566_CRU_GATE_CON31_OFFSET				0x37CU
#define RK3566_PMUCRU_PMUGATE_CON01_OFFSET			0x184U  //bit 9 bit10  0 enable

// iomux + pull up/down
#define RK3566_GRF_GPIO1A_IOMUX_L_OFFSET			0x00U
#define RK3566_GRF_GPIO1A_P_OFFSET					0x80U
#define RK3566_PMU_GRF_GPIO0A_IOMUX_L_OFFSET		0x00U
#define RK3566_PMU_GRF_GPIO0A_P_OFFSET				0x20U

typedef struct {
	unsigned int * gpio0_base;
	unsigned int * gpio1_base;
	unsigned int * gpio2_base;
	unsigned int * gpio3_base;
	unsigned int * gpio4_base;

	unsigned int * pmu_grf_base;
	unsigned int * sys_grf_base;

	unsigned int * cru_base;
	unsigned int * pmu_cru_base;
} rk3566_soc_info;

/*********** Rockchip RK3566 *************/


/*********** Rockchip RK3399 *************/

#define RK3399_GPIO1_BASE 							0xff730000U
#define RK3399_GPIO2_BASE 							0xff780000U
#define RK3399_GPIO4_BASE 							0xff790000U
#define RK3399_GPIO_NUM                          	(0x40)
#define RK3399_GPIO_BIT(x)                        	(1UL << (x))
#define RK3399_GPIO_SWPORTA_DR_OFFSET 				0x00U
#define RK3399_GPIO_SWPORTA_DDR_OFFSET 				0x04U
#define RK3399_GPIO_EXT_PORTA_OFFSET				0x50U

#define RK3399_GRF_GPIO2_3_4_P_OFFSET				0x00040U
#define RK3399_PMUGRF_GPIO0_1_P_OFFSET				0x00040U

#define RK3399_PMUGRF_BASE 	     					0xff320000U	 
#define RK3399_GRF_BASE 							0xff77e000U
#define RK3399_CRU_BASE 							0xff760000U
#define RK3399_PMUCRU_BASE 							0xff750000U
#define RK3399_CRU_CLKGATE_CON31_OFFSET 			0x037cU    //bit 3 4 5
#define RK3399_PMUCRU_CLKGATE_CON1_OFFSET 			0x0104U

typedef struct {
	unsigned int * gpio2_base;

	unsigned int * grf_base;
	unsigned int * cru_base;
	unsigned int * pmucru_base;
	unsigned int * pmugrf_base;
	unsigned int * gpio1_base;
	unsigned int * gpio4_base;
} rk3399_soc_info;

/*********** Rockchip RK3399 *************/


/*********** Rockchip RK3328 *************/

#define RK3328_GPIO2_BASE 							0xff230000
#define RK3328_GPIO3_BASE 							0xff240000
#define RK3328_GPIO_NUM                          	(0x40)
#define RK3328_GPIO_SWPORTA_DR_OFFSET 				0x00
#define RK3328_GPIO_SWPORTA_DDR_OFFSET 				0x04
#define RK3328_GPIO_EXT_PORTA_OFFSET				0x50
#define RK3328_GRF_BASE 							0xff100000
#define RK3328_CRU_BASE 							0xff440000
#define RK3328_CRU_CLKGATE_CON16_OFFSET 			0x0240    //bit 7 8 9 10 9877

typedef struct {
	unsigned int * gpio2_base;
	unsigned int * gpio3_base;
	unsigned int * cru_base;
	unsigned int * grf_base;
} rk3328_soc_info;

/*********** Rockchip RK3328 *************/


// Mask for the bottom 64 pins which belong to the Raspberry Pi
//	The others are available for the other devices


#define	PI_GPIO_MASK	(0xFFFFFFC0)


// Handy defines
extern int wiringPiDebug;

// wiringPi modes

#define	WPI_MODE_PINS		 0
#define	WPI_MODE_GPIO		 1
#define	WPI_MODE_GPIO_SYS	 2
#define	WPI_MODE_PHYS		 3
#define	WPI_MODE_PIFACE		 4
#define	WPI_MODE_UNINITIALISED	-1

// Pin modes

#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3
#define	SOFT_PWM_OUTPUT		 4
#define	SOFT_TONE_OUTPUT	 5
#define	PWM_TONE_OUTPUT		 6

#define	LOW			 0
#define	HIGH			 1

// Pull up/down/none

#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2

// PWM

#define	PWM_MODE_MS		0
#define	PWM_MODE_BAL		1

// Interrupt levels

#define	INT_EDGE_SETUP		0
#define	INT_EDGE_FALLING	1
#define	INT_EDGE_RISING		2
#define	INT_EDGE_BOTH		3

// Pi model types and version numbers
//	Intended for the GPIO program Use at your own risk.

/* Allwinner H6 */
#define	PI_MODEL_3		 		0
#define	PI_MODEL_LTIE_2			1

/* Allwinner H2+ */
#define	PI_MODEL_ZERO			2

/* Allwinner H3 */
#define	PI_MODEL_H3				3
#define	PI_MODEL_ZERO_PLUS_2	4

/* Allwinner A64 */
#define	PI_MODEL_WIN			5

/* Allwinner H5 */
#define	PI_MODEL_PRIME			6
#define	PI_MODEL_PC_2			7
#define	PI_MODEL_ZERO_PLUS		8

/* Allwinner H616 */
#define	PI_MODEL_ZERO_2			9


/* Rockchip RK3399 */
#define	PI_MODEL_800			15
#define	PI_MODEL_4				16
#define	PI_MODEL_4_LTS			17
#define	PI_MODEL_RK3399			18

/* Rockchip RK3328 */
#define	PI_MODEL_R1_PLUS		22

/* Rockchip RK3588(s) */
#define	PI_MODEL_5				24
#define	PI_MODEL_5B				25
#define	PI_MODEL_5_PLUS				26

/* Rockchip RK3566 */
#define	PI_MODEL_CM4				27

extern const char *piModelNames    [16] ;

extern const char *piRevisionNames [16] ;
extern const char *piMakerNames    [16] ;
extern const int   piMemorySize    [ 8] ;


//	Intended for the GPIO program Use at your own risk.

// Threads

#define	PI_THREAD(X)	void *X (UNU void *dummy)

// Failure modes

#define	WPI_FATAL	(1==1)
#define	WPI_ALMOST	(1==2)


// wiringPiNodeStruct:
//	This describes additional device nodes in the extended wiringPi
//	2.0 scheme of things.
//	It's a simple linked list for now, but will hopefully migrate to 
//	a binary tree for efficiency reasons - but then again, the chances
//	of more than 1 or 2 devices being added are fairly slim, so who
//	knows....

struct wiringPiNodeStruct
{
  int     pinBase ;
  int     pinMax ;

  int          fd ;	// Node specific
  unsigned int data0 ;	//  ditto
  unsigned int data1 ;	//  ditto
  unsigned int data2 ;	//  ditto
  unsigned int data3 ;	//  ditto

           void   (*pinMode)          (struct wiringPiNodeStruct *node, int pin, int mode) ;
           void   (*pullUpDnControl)  (struct wiringPiNodeStruct *node, int pin, int mode) ;
           int    (*digitalRead)      (struct wiringPiNodeStruct *node, int pin) ;
//unsigned int    (*digitalRead8)     (struct wiringPiNodeStruct *node, int pin) ;
           void   (*digitalWrite)     (struct wiringPiNodeStruct *node, int pin, int value) ;
//         void   (*digitalWrite8)    (struct wiringPiNodeStruct *node, int pin, int value) ;
           void   (*pwmWrite)         (struct wiringPiNodeStruct *node, int pin, int value) ;
           int    (*analogRead)       (struct wiringPiNodeStruct *node, int pin) ;
           void   (*analogWrite)      (struct wiringPiNodeStruct *node, int pin, int value) ;

  struct wiringPiNodeStruct *next ;
} ;

extern struct wiringPiNodeStruct *wiringPiNodes ;

// Export variables for the hardware pointers

extern volatile unsigned int *_wiringPiGpio ;
extern volatile unsigned int *_wiringPiPwm ;
extern volatile unsigned int *_wiringPiClk ;
extern volatile unsigned int *_wiringPiPads ;
extern volatile unsigned int *_wiringPiTimer ;
extern volatile unsigned int *_wiringPiTimerIrqRaw ;


// Function prototypes
//	c++ wrappers thanks to a comment by Nick Lott
//	(and others on the Raspberry Pi forums)

#ifdef __cplusplus
extern "C" {
#endif

// Data

// Internal

#ifdef CONFIG_ORANGEPI
extern void piGpioLayoutOops (const char *why);
#endif

extern int wiringPiFailure (int fatal, const char *message, ...) ;

// Core wiringPi functions

extern struct wiringPiNodeStruct *wiringPiFindNode (int pin) ;
extern struct wiringPiNodeStruct *wiringPiNewNode  (int pinBase, int numPins) ;

extern void wiringPiVersion	(int *major, int *minor) ;
extern int  wiringPiSetup       (void) ;
extern int  wiringPiSetupSys    (void) ;
extern int  wiringPiSetupGpio   (void) ;
extern int  wiringPiSetupPhys   (void) ;

extern          void pinModeAlt          (int pin, int mode) ;
extern          void pinMode             (int pin, int mode) ;
extern          void pullUpDnControl     (int pin, int pud) ;
extern          int  digitalRead         (int pin) ;
extern          void digitalWrite        (int pin, int value) ;
extern unsigned int  digitalRead8        (int pin) ;
extern          void digitalWrite8       (int pin, int value) ;
extern          void pwmWrite            (int pin, int value) ;
extern          int  analogRead          (int pin) ;
extern          void analogWrite         (int pin, int value) ;

// PiFace specifics 
//	(Deprecated)

extern int  wiringPiSetupPiFace (void) ;
extern int  wiringPiSetupPiFaceForGpioProg (void) ;	// Don't use this - for gpio program only

// On-Board Raspberry Pi hardware specific stuff

extern          void piBoardId           (int *model) ;
extern          int  wpiPinToGpio        (int wpiPin) ;
extern          int  physPinToGpio       (int physPin) ;
extern          void setPadDrive         (int group, int value) ;
extern          int  getAlt              (int pin) ;
extern          void pwmToneWrite        (int pin, int freq) ;
extern          void pwmSetMode          (int mode) ;
extern          void pwmSetRange         (unsigned int range) ;
extern          void pwmSetClock         (int divisor) ;
extern          void gpioClockSet        (int pin, int freq) ;
extern unsigned int  digitalReadByte     (void) ;
extern unsigned int  digitalReadByte2    (void) ;
extern          void digitalWriteByte    (int value) ;
extern          void digitalWriteByte2   (int value) ;

// Interrupts
//	(Also Pi hardware specific)

extern int  waitForInterrupt    (int pin, int mS) ;
extern int  wiringPiISR         (int pin, int mode, void (*function)(void)) ;

// Threads

extern int  piThreadCreate      (void *(*fn)(void *)) ;
extern void piLock              (int key) ;
extern void piUnlock            (int key) ;

// Schedulling priority

extern int piHiPri (const int pri) ;

// Extras from arduino land

extern void         delay             (unsigned int howLong) ;
extern void         delayMicroseconds (unsigned int howLong) ;
extern unsigned int millis            (void) ;
extern unsigned int micros            (void) ;

extern unsigned int readR(unsigned int addr);
extern void writeR(unsigned int val, unsigned int addr);
extern int OrangePi_get_gpio_mode(int pin);
extern int OrangePi_set_gpio_mode(int pin, int mode);
extern int OrangePi_digitalRead(int pin);
extern int OrangePi_digitalWrite(int pin, int value);
extern int OrangePi_set_gpio_alt(int pin, int mode);
extern void OrangePi_set_gpio_pullUpDnControl (int pin, int pud);

void set_soc_info(void);

#ifdef __cplusplus
}
#endif

#endif
