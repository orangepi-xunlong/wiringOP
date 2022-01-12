#ifndef _ORANGEPI_H
#define _ORANGEPI_H

#ifdef CONFIG_ORANGEPI_2G_IOT
/********** OrangePi 2G-IOT *************/
/*
 * GPIOA_BASE                         0x20930000
 * GPIOB_BASE                         0x20931000
 * GPIOC_BASE                         0x11A08000
 * GPIOD_BASE                         0x20932000
 */

/********* local data ************/
#define GPIOA_BASE                         0x20930000
#define GPIOB_BASE                         0x20931000
#define GPIOC_BASE                         0x11A08000
#define GPIOD_BASE                         0x20932000
#define GPIO_NUM                           (0x80)
#define GPIO_BIT(x)                        (1UL << (x))

#define OEN_VAL_REGISTER                   (0x00)
#define OEN_SET_OUT_REGISTER               (0x04)
#define SET_IN_REGISTER                    (0x08)
#define VAL_REGISTER                       (0x0C)
#define SET_REGISTER                       (0x10)
#define CLR_REGISTER                       (0x14)

#define MEM_INFO                           (512)
#define MAP_SIZE_L                         (4 * 4096)

#endif /* CONFIG_ORANGEPI_2G_IOT */

#if CONFIG_ORANGEPI_PC2 || CONFIG_ORANGEPI_ZEROPLUS || CONFIG_ORANGEPI_ZEROPLUS2_H5 || CONFIG_ORANGEPI_PRIME
/************** OrangePi H5 ***********************/
#define GPIOA_BASE                         (0x01C20000)
#define GPIO_NUM                           (0x40)
#define GPIO_BASE_MAP                      (0x01C20800)
#define MEM_INFO                           (1024)
#define GPIOL_BASE                         (0x01F02c00)
#define GPIOL_BASE_MAP                     (0x01F02000)  
#define MAP_SIZE_L                         (4096 * 2)
#define GPIO_PWM_OP			   (0x01C21000)
#endif

/************** OrangePi A64 ***********************/
#ifdef CONFIG_ORANGEPI_WIN
#define GPIOA_BASE                         (0x01C20000)
#define GPIO_NUM                           (0x40)
#define GPIO_BASE_MAP                      (0x01C20800)
#define MEM_INFO                           (1024)
#define GPIOL_BASE                         (0x01F02c00)
#define GPIOL_BASE_MAP                     (0x01F02000)  
#define MAP_SIZE_L                         (4096 * 2)
#define GPIO_PWM_OP			   (0x01C21000)
#endif

/************** OrangePi H3 ***********************/
#if CONFIG_ORANGEPI_H3 || CONFIG_ORANGEPI_ZEROPLUS2_H3 || CONFIG_ORANGEPI_ZERO
#define GPIOA_BASE                         (0x01C20000)
#define GPIO_NUM                           (0x40)
#define GPIO_BASE_MAP                      (0x01C20800)
#define MEM_INFO                           (1024)
#define GPIOL_BASE                         (0x01F02c00)
#define GPIOL_BASE_MAP                     (0x01F02000)  
#define MAP_SIZE_L                         (4096 * 2)
#define GPIO_PWM_OP			   (0x01C21000)
#endif

/*********** OrangePi LITE2/OnePlus/PC3 *************/
#if CONFIG_ORANGEPI_LITE2 || CONFIG_ORANGEPI_3
#define GPIOA_BASE                         (0x0300B000)
#define GPIO_NUM                           (0x40)
#define GPIO_BASE_MAP                      (0x0300B000)
#define MEM_INFO                           (1024)
#define GPIOL_BASE                         (0x07022000)
#define GPIOL_BASE_MAP                     (0x07022000)  
#define MAP_SIZE_L                         (4096 * 1)
#define GPIO_PWM_OP			   (0x0300A000)
#endif

/*********** OrangePi H616 *************/
#if CONFIG_ORANGEPI_ZERO2
#define GPIOA_BASE                         (0x0300B000)
#define GPIO_NUM                           (0x40)
#define GPIO_BASE_MAP                      (0x0300B000)
#define MEM_INFO                           (1024)
#define GPIOL_BASE                         (0x07022000)
#define GPIOL_BASE_MAP                     (0x07022000)  
#define MAP_SIZE_L                         (4096 * 1)
#define GPIO_PWM_OP			   (0x0300A000)
#endif

/*********** OrangePi RK3399 *************/
#if CONFIG_ORANGEPI_RK3399 || CONFIG_ORANGEPI_4 || CONFIG_ORANGEPI_4_LTS || CONFIG_ORANGEPI_800

#define GPIO1_BASE 							0xff730000
#define GPIO2_BASE 							0xff780000
#define GPIO4_BASE 							0xff790000
#define GPIO_NUM                          	(0x40)
#define GPIO_BIT(x)                        	(1UL << (x))
#define GPIO_SWPORTA_DR_OFFSET 				0x00
#define GPIO_SWPORTA_DDR_OFFSET 			0x04
#define GPIO_EXT_PORTA_OFFSET				0x50

#define PMUGRF_BASE 	     0xff320000	 
#define PMUGRF_GPIO1A_IOMUX  0x00010
#define PMUGRF_GPIO1B_IOMUX  0x00014
#define PMUGRF_GPIO1C_IOMUX  0x00018
#define PMUGRF_GPIO1D_IOMUX  0x0001c

#define GRF_BASE 					0xff77e000
#define GRF_GPIO2A_IOMUX_OFFSET 			0x00
#define GRF_GPIO2B_IOMUX_OFFSET 			0x04
#define GRF_GPIO2C_IOMUX_OFFSET 			0x08
#define GRF_GPIO2D_IOMUX_OFFSET 			0x0c

#define GRF_GPIO4A_IOMUX_OFFSET 			0x20
#define GRF_GPIO4B_IOMUX_OFFSET 			0x24
#define GRF_GPIO4C_IOMUX_OFFSET 			0x28
#define GRF_GPIO4D_IOMUX_OFFSET 			0x2c


#define CRU_BASE 					0xff760000
#define PMUCRU_BASE 	0xff750000
#define CRU_CLKGATE_CON31_OFFSET 			0x037c    //bit 3 4 5
#define PMUCRU_CLKGATE_CON1_OFFSET 			0x0104

#define MEM_INFO                           (2048)
#define MAP_SIZE_L                         (4*1024)

extern volatile unsigned int *gpio2_base;
extern volatile unsigned int *grf_base;
extern volatile unsigned int *cru_base;
extern volatile unsigned int *pmucru_base;
extern volatile unsigned int *pmugrf_base;
extern volatile unsigned int *gpio1_base;
extern volatile unsigned int *gpio4_base;

#endif /* CONFIG_ORANGEPI_RK3399 */
//csy 2019.1.8


/*********** OrangePi R1PLUS *************/
#if CONFIG_ORANGEPI_R1PLUS

#define GPIO2_BASE 							0xff230000
#define GPIO3_BASE 							0xff240000
#define GPIO_NUM                          	(0x40)

#define GPIO_SWPORTA_DR_OFFSET 				0x00
#define GPIO_SWPORTA_DDR_OFFSET 			0x04
#define GPIO_EXT_PORTA_OFFSET				0x50

#define GRF_BASE 							0xff100000
#define GRF_GPIO2A_IOMUX_OFFSET 			0x20
#define GRF_GPIO2BL_IOMUX_OFFSET 			0x24
#define GRF_GPIO2BH_IOMUX_OFFSET 			0x28
#define GRF_GPIO2CL_IOMUX_OFFSET 			0x2c
#define GRF_GPIO2CH_IOMUX_OFFSET 			0x30
#define GRF_GPIO2D_IOMUX_OFFSET 			0x34

#define GRF_GPIO3AL_IOMUX_OFFSET 			0x38
#define GRF_GPIO3AH_IOMUX_OFFSET 			0x3c
#define GRF_GPIO3BL_IOMUX_OFFSET 			0x40
#define GRF_GPIO3BH_IOMUX_OFFSET 			0x44
#define GRF_GPIO3C_IOMUX_OFFSET 			0x48
#define GRF_GPIO3D_IOMUX_OFFSET 			0x4c

#define CRU_BASE 							0xff440000
#define CRU_CLKGATE_CON16_OFFSET 			0x0240    //bit 7 8 9 10 9877

#define MEM_INFO                           (2048)
#define MAP_SIZE_L                         (4*1024)

extern volatile unsigned int *gpio2_base;
extern volatile unsigned int *gpio3_base;
extern volatile unsigned int *cru_base;
extern volatile unsigned int *grf_base;


#endif /* CONFIG_ORANGEPI_R1PLUS */
//FG 2020.11.26



/****************** Global data *********************/
/* Current version */
#define PI_MAKER_ORANGEPI  4
#define MAX_PIN_NUM        GPIO_NUM
#define MAP_SIZE           MAP_SIZE_L
#define MAP_MASK           (MAP_SIZE - 1)
#define PI_GPIO_MASK       (~(GPIO_NUM - 1))
#define GPIO_BASE          GPIOA_BASE
#define ORANGEPI_MEM_INFO  MEM_INFO
#define GPIO_PWM 		   GPIO_PWM_OP


//sunxi_pwm
#define SUNXI_PWM_BASE (0x01c21400)
#define SUNXI_PWM_CTRL_REG  (SUNXI_PWM_BASE)
#define SUNXI_PWM_CH0_PERIOD  (SUNXI_PWM_BASE + 0x4)
#define SUNXI_PWM_CH1_PERIOD  (SUNXI_PWM_BASE + 0x8)

#define SUNXI_PWM_CH0_EN   (1 << 4)
#define SUNXI_PWM_CH0_ACT_STA  (1 << 5)
#define SUNXI_PWM_SCLK_CH0_GATING (1 << 6)
#define SUNXI_PWM_CH0_MS_MODE  (1 << 7) //pulse mode
#define SUNXI_PWM_CH0_PUL_START  (1 << 8)

#define PWM_CLK_DIV_120  0
#define PWM_CLK_DIV_180  1
#define PWM_CLK_DIV_240  2
#define PWM_CLK_DIV_360  3
#define PWM_CLK_DIV_480  4
#define PWM_CLK_DIV_12K  8
#define PWM_CLK_DIV_24K  9
#define PWM_CLK_DIV_36K  10
#define PWM_CLK_DIV_48K  11
#define PWM_CLK_DIV_72K  12

extern int pinToGpioOrangePi[64];
extern int physToGpioOrangePi[64];
extern int physToPinOrangePi[64];
extern int physToWpiOrangePi[64];
extern volatile unsigned int *OrangePi_gpio;
extern volatile unsigned int *OrangePi_gpioC;

extern int pinToGpioR3[64];
extern int physToGpioR3[64];
extern int physToPinR3[64];
extern int pwmmode;

extern unsigned int readR(unsigned int addr);
extern void writeR(unsigned int val, unsigned int addr);
extern int OrangePi_set_gpio_mode(int pin, int mode);
#if !(defined CONFIG_ORANGEPI_RK3399 || defined CONFIG_ORANGEPI_4 || defined CONFIG_ORANGEPI_4_LTS || defined CONFIG_ORANGEPI_800 || defined CONFIG_ORANGEPI_R1PLUS || CONFIG_ORANGEPI_2G_IOT)
extern int OrangePi_set_gpio_alt(int pin, int mode);
#endif
extern int OrangePi_get_gpio_mode(int pin);
extern int isOrangePi_2G_IOT(void);
extern int isOrangePi(void);
extern unsigned int readR(unsigned int addr);
extern void writeR(unsigned int val, unsigned int addr);
extern int OrangePi_digitalWrite(int pin, int value);
extern int OrangePi_digitalRead(int pin);

extern void print_pwm_reg(void);
extern void sunxi_pwm_set_enable(int en);
extern void sunxi_pwm_set_mode(int mode);
extern void sunxi_pwm_set_clk(int clk);
extern int sunxi_pwm_get_period(void);
extern int sunxi_pwm_get_act(void);
extern void sunxi_pwm_set_period(int period_cys);
extern void sunxi_pwm_set_act(int act_cys);


#ifdef CONFIG_ORANGEPI
extern const char *piModelNames[6];
#endif

#ifdef CONFIG_ORANGEPI_2G_IOT
extern int ORANGEPI_PIN_MASK[4][32];
#elif CONFIG_ORANGEPI_PC2 || CONFIG_ORANGEPI_PRIME
extern int ORANGEPI_PIN_MASK[9][32];
#elif CONFIG_ORANGEPI_WIN
extern int ORANGEPI_PIN_MASK[12][32];
#elif CONFIG_ORANGEPI_H3
extern int ORANGEPI_PIN_MASK[9][32];
#elif CONFIG_ORANGEPI_ZERO || CONFIG_ORANGEPI_ZEROPLUS2_H3 || CONFIG_ORANGEPI_ZEROPLUS || CONFIG_ORANGEPI_ZEROPLUS2_H5
extern int ORANGEPI_PIN_MASK[12][32];
#elif CONFIG_ORANGEPI_RK3399
extern int ORANGEPI_PIN_MASK[5][32];
#elif CONFIG_ORANGEPI_R1PLUS
extern int ORANGEPI_PIN_MASK[5][32];


#endif
#endif
