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

#define PI_VERSION_1             0
#define PI_VERSION_1_1           1
#define PI_VERSION_1_2           2
#define PI_VERSION_2             3

#define PI_MAKER_SONY            0
#define PI_MAKER_EGOMAN          1
#define PI_MAKER_EMBEST          2
#define PI_MAKER_UNKNOWN         3

/*********** Allwinner H3 *************/
#define H3_GPIO_BASE_ADDR                       0x01C20000U
#define H3_R_GPIO_BASE_ADDR                     0x01F02000U
/*********** Allwinner H3 *************/

/*********** Allwinner H6 *************/
#define H6_GPIO_BASE_ADDR                       0x0300B000U
#define H6_R_GPIO_BASE_ADDR                     0x07022000U
/*********** Allwinner H6 *************/

typedef struct {
	unsigned int gpio_base_addr;
	unsigned int r_gpio_base_addr;
	unsigned int * gpio;
	unsigned int * r_gpio;
	unsigned int gpio_base_offset;
	unsigned int r_gpio_base_offset;
	unsigned int pwm_base_addr;
	unsigned int * pwm;
	unsigned int pwm_ctrl;
	unsigned int pwm_period;
	unsigned int pwm_clk;		// H616
	unsigned int pwm_en;		// H616
	unsigned int pwm_type;		// type:V1 H3/H6, type:V2 H616
	unsigned int pwm_bit_en; 	// SUNXI_PWM_CH0_EN
	unsigned int pwm_bit_act;	// SUNXI_PWM_CH0_ACT_STA
	unsigned int pwm_bit_sclk;	// SUNXI_PWM_SCLK_CH0_GATING
	unsigned int pwm_bit_mode;	// SUNXI_PWM_CH0_MS_MODE
	unsigned int pwm_bit_pulse;	// SUNXI_PWM_CH0_PUL_START
} sunxi_gpio_info;
#define GPIO_PWM                                GPIO_PWM_OP

//sunxi_pwm
#ifdef OPI
#define SUNXI_PWM_BASE                          (0x01c21400)
#define SUNXI_PWM_CTRL_REG                      (SUNXI_PWM_BASE)
#define SUNXI_PWM_CH0_PERIOD                    (SUNXI_PWM_BASE + 0x4)
#define SUNXI_PWM_CH1_PERIOD                    (SUNXI_PWM_BASE + 0x8)

#define SUNXI_PWM_CH0_EN                        (1 << 4)
#define SUNXI_PWM_CH0_ACT_STA                   (1 << 5)
#define SUNXI_PWM_SCLK_CH0_GATING               (1 << 6)
#define SUNXI_PWM_CH0_MS_MODE                   (1 << 7)    //pulse mode
#define SUNXI_PWM_CH0_PUL_START                 (1 << 8)

#else
#define SUNXI_PWM_BASE                          (sunxi_gpio_info_t.pwm_base_addr)
#define SUNXI_PWM_CTRL_REG                      (sunxi_gpio_info_t.pwm_ctrl)
#define SUNXI_PWM_PERIOD                        (sunxi_gpio_info_t.pwm_period)
#define SUNXI_PWM_CLK_REG                       (sunxi_gpio_info_t.pwm_clk)    // H616
#define SUNXI_PWM_EN_REG                        (sunxi_gpio_info_t.pwm_en)    // H616
#define SUNXI_PWM_TYPE                          (sunxi_gpio_info_t.pwm_type)
#define SUNXI_PWM_EN                            (sunxi_gpio_info_t.pwm_bit_en)
#define SUNXI_PWM_ACT_STA                       (sunxi_gpio_info_t.pwm_bit_act)
#define SUNXI_PWM_SCLK_GATING                   (sunxi_gpio_info_t.pwm_bit_sclk)
#define SUNXI_PWM_MS_MODE                       (sunxi_gpio_info_t.pwm_bit_mode)    //pulse mode
#define SUNXI_PWM_PUL_START                     (sunxi_gpio_info_t.pwm_bit_pulse)
#endif

#define H3_PWM_BASE                             (0x01c21400)
#define H6_PWM_BASE                             (0x0300A000)
#define H616_PWM_BASE                           (0x0300A000)

#define SUNXI_V1_PWM_TYPE                       (1)
#define SUNXI_V2_PWM_TYPE                       (2)

#define SUNXI_V1_PWM_EN_REG                     (SUNXI_PWM_BASE + 0x0)
#define SUNXI_V1_PWM_CLK_REG                    (SUNXI_PWM_BASE + 0x0)
#define SUNXI_V1_PWM_CTRL_REG                   (SUNXI_PWM_BASE + 0x0)
#define SUNXI_V1_PWM_CH0_PERIOD                 (SUNXI_PWM_BASE + 0x4)

#define SUNXI_V1_PWM_CH0_EN                     (1 << 4)
#define SUNXI_V1_PWM_CH0_ACT_STA                (1 << 5)
#define SUNXI_V1_PWM_SCLK_CH0_GATING            (1 << 6)
#define SUNXI_V1_PWM_CH0_MS_MODE                (1 << 7)    //pulse mode
#define SUNXI_V1_PWM_CH0_PUL_START              (1 << 8)

//-----------------------------------------------------
//SUNXI_V2_PWM Fixed parameters
#define SUNXI_V2_PWM_EN_REG                     (SUNXI_PWM_BASE + 0x40)
#define SUNXI_V2_PWM_ACT_STA                    (1 << 8)
#define SUNXI_V2_PWM_SCLK_GATING                (1 << 4)
#define SUNXI_V2_PWM_MS_MODE                    (1 << 9)
#define SUNXI_V2_PWM_PUL_START                  (1 << 10)

//SUNXI_V2_PWM Variable parameters
#define SUNXI_V2_PWM1_PERIOD                    (SUNXI_PWM_BASE + 0x84)
#define SUNXI_V2_PWM1_CTRL_REG                  (SUNXI_PWM_BASE + 0x80)
#define SUNXI_V2_PWM1_CLK_REG                   (SUNXI_PWM_BASE + 0x20)
#define SUNXI_V2_PWM1_EN                        (1 << 1)

#define SUNXI_V2_PWM2_PERIOD                    (SUNXI_PWM_BASE + 0xA4)
#define SUNXI_V2_PWM2_CTRL_REG                  (SUNXI_PWM_BASE + 0xA0)
#define SUNXI_V2_PWM2_CLK_REG                   (SUNXI_PWM_BASE + 0x24)
#define SUNXI_V2_PWM2_EN                        (1 << 2)

#define SUNXI_V2_PWM3_PERIOD                    (SUNXI_PWM_BASE + 0xC4)
#define SUNXI_V2_PWM3_CTRL_REG                  (SUNXI_PWM_BASE + 0xC0)
#define SUNXI_V2_PWM3_CLK_REG                   (SUNXI_PWM_BASE + 0x24)
#define SUNXI_V2_PWM3_EN                        (1 << 3)

#define SUNXI_V2_PWM4_PERIOD                    (SUNXI_PWM_BASE + 0xE4)
#define SUNXI_V2_PWM4_CTRL_REG                  (SUNXI_PWM_BASE + 0xE0)
#define SUNXI_V2_PWM4_CLK_REG                   (SUNXI_PWM_BASE + 0x28)
#define SUNXI_V2_PWM4_EN                        (1 << 4)

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

#define SUNXI_PUD_OFF    0
#define SUNXI_PUD_UP     1
#define SUNXI_PUD_DOWN   2


/*********** Rockchip RK3588 *************/

//gpio0~gpio4 register base addr
#define RK3588_GPIO0_BASE                       0xfd8a0000U
#define RK3588_GPIO1_BASE                       0xfec20000U
#define RK3588_GPIO2_BASE                       0xfec30000U
#define RK3588_GPIO3_BASE                       0xfec40000U
#define RK3588_GPIO4_BASE                       0xfec50000U

//gpio offset
#define RK3588_GPIO_SWPORT_DR_L_OFFSET          0x00U
#define RK3588_GPIO_SWPORT_DR_H_OFFSET          0x04U
#define RK3588_GPIO_SWPORT_DDR_L_OFFSET         0x08U
#define RK3588_GPIO_SWPORT_DDR_H_OFFSET         0x0cU
#define RK3588_GPIO_EXT_PORT_OFFSET             0x70U

//CRU clock-controller base addr
#define RK3588_CRU_BASE                         0xfd7c0000U
#define RK3588_CRU_GATE_CON16_OFFSET            0x0840U    //for gpio1 bit 14 15    30 31
#define RK3588_CRU_GATE_CON17_OFFSET            0x0844U    //for gpio2/3/4 - bit 0 1 2 3 4 5    16 17 18 19 20 21

#define RK3588_PMU1CRU_BASE                     0xfd7f0000U
#define RK3588_PMU1CRU_GATE_CON5_OFFSET         0x0814U    //for gpio0 - bit 5 6    21 22

#define RK3588_GPIO_NUM                         (0x40)
#define RK3588_GPIO_BIT(x)                      (1UL << (x))

//gpio iomux
#define RK3588_PMU1_IOC_BASE                    0xfd5f0000U
#define RK3588_PMU1_IOC_GPIO0A_IOMUX_SEL_L      0x00U    //gpio0a0~gpio0b3
#define RK3588_PMU1_IOC_GPIO0A_IOMUX_SEL_H      0x04U    //gpio0a4~gpio0b7
#define RK3588_PMU1_IOC_GPIO0B_IOMUX_SEL_L      0x08U    //gpio0b0~gpio0b3

#define RK3588_PMU2_IOC_BASE                    0xfd5f4000U
#define RK3588_PMU2_IOC_GPIO0B_IOMUX_SEL_H      0x00U    //gpio0a5~gpio0b7
#define RK3588_PMU2_IOC_GPIO0C_IOMUX_SEL_L      0x04U    //gpio0a0~gpio0b3
#define RK3588_PMU2_IOC_GPIO0C_IOMUX_SEL_H      0x08U    //gpio0a4~gpio0b7
#define RK3588_PMU2_IOC_GPIO0D_IOMUX_SEL_L      0x0cU    //gpio0a0~gpio0b3
#define RK3588_PMU2_IOC_GPIO0D_IOMUX_SEL_H      0x10U    //gpio0a4~gpio0b6

#define RK3588_BUS_IOC_BASE                     0xfd5f8000U

//gpio pull up/down
#define RK3588_VCCIO1_4_IOC_BASE                0xfd5f9000U
#define RK3588_VCCIO3_5_IOC_BASE                0xfd5fa000U
#define RK3588_VCCIO6_IOC_BASE                  0xfd5fc000U

#define RK3588_PMU1_IOC_GPIO0A_P                0x0020U
#define RK3588_PMU1_IOC_GPIO0B_P                0x0024U
#define RK3588_PMU2_IOC_GPIO0B_P                0x0028U
#define RK3588_PMU2_IOC_GPIO0C_P                0x002cU
#define RK3588_PMU2_IOC_GPIO0D_P                0x0030U
#define RK3588_VCCIO1_4_IOC_GPIO1A_P            0x0110U
#define RK3588_VCCIO3_5_IOC_GPIO2A_P            0x0120U
#define RK3588_VCCIO6_IOC_GPIO4A_P              0x0140U

//pwm register base addr
//#define RK3588_CRU_BASE                       0xfd7c0000U
//#define RK3588_PMU1CRU_BASE                   0xfd7f0000U
#define RK3588_PWM0_BASE                        0xfd8b0000U
#define RK3588_PWM1_BASE                        0xfe8d0000U
#define RK3588_PWM2_BASE                        0xfebe0000U
#define RK3588_PWM3_BASE                        0xfebf0000U

//cru
#define RK3588_CRU_GATE_CON19                   (RK3588_CRU_BASE + 0x084CU)    //for busioc_clk_en
#define RK3588_CRU_GATE_CON15                   (RK3588_CRU_BASE + 0x083CU)    //for pwm123_clk_en
#define RK3588_PMU1CRU_GATE_CON1                (RK3588_PMU1CRU_BASE +0x0804U)    //for pmu1pwm_clk_en

//CH0
#define RK3588_CH0_PERIOD_HPR                   (RK3588_PWM_BASE + 0x04)
#define RK3588_CH0_DUTY_LPR                     (RK3588_PWM_BASE + 0x08)
#define RK3588_CH0_CTRL                         (RK3588_PWM_BASE + 0x0C)

//CH1
#define RK3588_CH1_PERIOD_HPR                   (RK3588_PWM_BASE + 0x14)
#define RK3588_CH1_DUTY_LPR                     (RK3588_PWM_BASE + 0x18)
#define RK3588_CH1_CTRL                         (RK3588_PWM_BASE + 0x1C)

//CH2
#define RK3588_CH2_PERIOD_HPR                   (RK3588_PWM_BASE + 0x24)
#define RK3588_CH2_DUTY_LPR                     (RK3588_PWM_BASE + 0x28)
#define RK3588_CH2_CTRL                         (RK3588_PWM_BASE + 0x2C)

//CH3
#define RK3588_CH3_PERIOD_HPR                   (RK3588_PWM_BASE + 0x34)
#define RK3588_CH3_DUTY_LPR                     (RK3588_PWM_BASE + 0x38)
#define RK3588_CH3_CTRL                         (RK3588_PWM_BASE + 0x3C)

//for short——pwm
#define RK3588_PWM_BASE                         (rk3588_soc_info_t.pwm_base)
#define RK3588_PWM_MUX                          (rk3588_soc_info_t.pwm_mux)
#define RK3588_PWM_MUX_VAL                      (rk3588_soc_info_t.pwm_mux_val)
#define RK3588_PWM_MUX_OFFSET                   (rk3588_soc_info_t.pwm_mux_offset)
#define RK3588_CH_PERIOD_HPR                    (rk3588_soc_info_t.ch_period_hpr)
#define RK3588_CH_DUTY_LPR                      (rk3588_soc_info_t.ch_duty_lpr)
#define RK3588_CH_CTRL                          (rk3588_soc_info_t.ch_crtl)

//pwm_ctrl_offset
#define RK3588_RPT                              (24)    // 24 ~ 31
#define RK3588_SCALE                            (16)    // 16 ~ 23
#define RK3588_PRESCALE                         (12)    // 12 ~ 14
#define RK3588_CLK_SRC_SEL                      (10)
#define RK3588_CLK_SEL                          (9)
#define RK3588_FORCE_CLK_EN                     (8)
#define RK3588_CH_CNT_EN                        (7)
#define RK3588_CONLOCK                          (6)
#define RK3588_OUTPUT_MODE                      (5)
#define RK3588_INACTIVE_POL                     (4)
#define RK3588_DUTY_POL                         (3)
#define RK3588_PWM_MODE                         (1)     // 1 ~ 2
#define RK3588_PWM_EN                           (0)

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

	unsigned int * pwm0_base;
	unsigned int * pwm1_base;
	unsigned int * pwm2_base;
	unsigned int * pwm3_base;
	unsigned int   pwm_base;
	unsigned int   pwm_mux;
	unsigned int   pwm_mux_val;
	unsigned int   pwm_mux_offset;
	unsigned int   ch_period_hpr;
	unsigned int   ch_duty_lpr;
	unsigned int   ch_crtl;
} rk3588_soc_info;

/*********** Rockchip RK3588 *************/

/*********** Rockchip RK3566 *************/

//gpio0~gpio4 register base addr
#define RK3566_GPIO0_BASE                       0xfdd60000U
#define RK3566_GPIO1_BASE                       0xfe740000U
#define RK3566_GPIO2_BASE                       0xfe750000U
#define RK3566_GPIO3_BASE                       0xfe760000U
#define RK3566_GPIO4_BASE                       0xfe770000U

//gpio offset
#define RK3566_GPIO_SWPORT_DR_L_OFFSET          0x00U
#define RK3566_GPIO_SWPORT_DR_H_OFFSET          0x04U
#define RK3566_GPIO_SWPORT_DDR_L_OFFSET         0x08U
#define RK3566_GPIO_SWPORT_DDR_H_OFFSET         0x0cU
#define RK3566_GPIO_EXT_PORT_OFFSET             0x70U

#define RK3566_PMU_GRF_BASE                     0xfdc20000U
#define RK3566_SYS_GRF_BASE                     0xfdc60000U
#define RK3566_PMU_CRU_BASE                     0xfdd00000U
#define RK3566_CRU_BASE                         0xFdd20000U

//clock
#define RK3566_CRU_GATE_CON31_OFFSET            0x37CU
#define RK3566_CRU_GATE_CON31                   (RK3566_CRU_BASE + 0x37CU)
#define RK3566_CRU_GATE_CON32_OFFSET            0x380U
#define RK3566_CRU_GATE_CON32                   (RK3566_CRU_BASE + 0x380U)
#define RK3566_PMUCRU_PMUGATE_CON01_OFFSET      0x184U  //bit 9 bit10  0 enable

//iomux + pull up/down
#define RK3566_GRF_GPIO1A_IOMUX_L_OFFSET        0x00U
#define RK3566_GRF_GPIO1A_P_OFFSET              0x80U
#define RK3566_PMU_GRF_GPIO0A_IOMUX_L_OFFSET    0x00U
#define RK3566_PMU_GRF_GPIO0A_P_OFFSET          0x20U

//pwm mux register
#define RK3566_PWM_MUX_REG                      (RK3566_SYS_GRF_BASE + 0x70)

//pwm register base addr
#define RK3566_PWM2_BASE                        0xfe6f0000U
#define RK3566_PWM3_BASE                        0xfe700000U

//CH3
#define RK3566_CH3_PERIOD_HPR                   (RK3566_PWM_BASE + 0x34)
#define RK3566_CH3_DUTY_LPR                     (RK3566_PWM_BASE + 0x38)
#define RK3566_CH3_CTRL                         (RK3566_PWM_BASE + 0x3C)

//for short——pwm
#define RK3566_CRU_GATE_CON                     (rk3566_soc_info_t.cru_gate_con)
#define RK3566_CRU_GATE_CON_OFFSET              (rk3566_soc_info_t.cru_gate_con_offset)
#define RK3566_PWM_BASE                         (rk3566_soc_info_t.pwm_base)
#define RK3566_PWM_MUX                          (rk3566_soc_info_t.pwm_mux)
#define RK3566_PWM_MUX_VAL                      (rk3566_soc_info_t.pwm_mux_val)
#define RK3566_PWM_MUX_OFFSET                   (rk3566_soc_info_t.pwm_mux_offset)
#define RK3566_CH_PERIOD_HPR                    (rk3566_soc_info_t.ch_period_hpr)
#define RK3566_CH_DUTY_LPR                      (rk3566_soc_info_t.ch_duty_lpr)
#define RK3566_CH_CTRL                          (rk3566_soc_info_t.ch_crtl)

//pwm_ctrl_offset
#define RK3566_RPT                              (24)    // 24 ~ 31
#define RK3566_SCALE                            (16)    // 16 ~ 23
#define RK3566_PRESCALE                         (12)    // 12 ~ 14
#define RK3566_CLK_SRC_SEL                      (10)
#define RK3566_CLK_SEL                          (9)
#define RK3566_FORCE_CLK_EN                     (8)
#define RK3566_CH_CNT_EN                        (7)
#define RK3566_CONLOCK                          (6)
#define RK3566_OUTPUT_MODE                      (5)
#define RK3566_INACTIVE_POL                     (4)
#define RK3566_DUTY_POL                         (3)
#define RK3566_PWM_MODE                         (1)     // 1 ~ 2
#define RK3566_PWM_EN                           (0)

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

	unsigned int * pwm2_base;
	unsigned int * pwm3_base;
	unsigned int   cru_gate_con;
	unsigned int   cru_gate_con_offset;
	unsigned int   pwm_base;
	unsigned int   pwm_mux;
	unsigned int   pwm_mux_val;
	unsigned int   pwm_mux_offset;
	unsigned int   ch_period_hpr;
	unsigned int   ch_duty_lpr;
	unsigned int   ch_crtl;
} rk3566_soc_info;

/*********** Rockchip RK3566 *************/


/*********** Rockchip RK3399 *************/

#define RK3399_GPIO1_BASE                       0xff730000U
#define RK3399_GPIO2_BASE                       0xff780000U
#define RK3399_GPIO4_BASE                       0xff790000U
#define RK3399_GPIO_NUM                         (0x40)
#define RK3399_GPIO_BIT(x)                      (1UL << (x))
#define RK3399_GPIO_SWPORTA_DR_OFFSET           0x00U
#define RK3399_GPIO_SWPORTA_DDR_OFFSET          0x04U
#define RK3399_GPIO_EXT_PORTA_OFFSET            0x50U

#define RK3399_GRF_GPIO2_3_4_P_OFFSET           0x00040U
#define RK3399_PMUGRF_GPIO0_1_P_OFFSET          0x00040U

#define RK3399_PMUGRF_BASE                      0xff320000U
#define RK3399_GRF_BASE                         0xff77e000U
#define RK3399_CRU_BASE                         0xff760000U
#define RK3399_PMUCRU_BASE                      0xff750000U
#define RK3399_CRU_CLKGATE_CON31_OFFSET         0x037cU    //bit 3 4 5
#define RK3399_PMUCRU_CLKGATE_CON1_OFFSET       0x0104U

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

#define RK3328_GPIO2_BASE                       0xff230000
#define RK3328_GPIO3_BASE                       0xff240000
#define RK3328_GPIO_NUM                         (0x40)
#define RK3328_GPIO_SWPORTA_DR_OFFSET           0x00
#define RK3328_GPIO_SWPORTA_DDR_OFFSET          0x04
#define RK3328_GPIO_EXT_PORTA_OFFSET            0x50
#define RK3328_GRF_BASE                         0xff100000
#define RK3328_CRU_BASE                         0xff440000
#define RK3328_CRU_CLKGATE_CON16_OFFSET         0x0240    //bit 7 8 9 10 9877

typedef struct {
	unsigned int * gpio2_base;
	unsigned int * gpio3_base;
	unsigned int * cru_base;
	unsigned int * grf_base;
} rk3328_soc_info;

/*********** Rockchip RK3328 *************/


/*************** S905D3 ******************/

#define S905D3_GPIO_BASE                        0xFF634000
#define S905D3_GPIO_AO_BASE                     0xFF800000
#define S905D3_GPIO_PWM_BASE                    0xFFD19000
#define S905D3_GPIO_PWM_AO_BASE                 0xFF802000

//GPIOA
#define S905D3_GPIOA_OUT_EN_REG                 (S905D3_GPIO_BASE + 0x120)
#define S905D3_GPIOA_OUT_REG                    (S905D3_GPIO_BASE + 0x121)
#define S905D3_GPIOA_IN_REG                     (S905D3_GPIO_BASE + 0x122)
#define S905D3_GPIOA_PUPD_REG                   (S905D3_GPIO_BASE + 0x13F)
#define S905D3_GPIOA_PUEN_REG                   (S905D3_GPIO_BASE + 0x14D)
#define S905D3_GPIOA_MUX_REG1                   (S905D3_GPIO_BASE + 0x1BD)
#define S905D3_GPIOA_MUX_REG2                   (S905D3_GPIO_BASE + 0x1BE)

//GPIOC
#define S905D3_GPIOC_OUT_EN_REG                 (S905D3_GPIO_BASE + 0x113)
#define S905D3_GPIOC_OUT_REG                    (S905D3_GPIO_BASE + 0x114)
#define S905D3_GPIOC_IN_REG                     (S905D3_GPIO_BASE + 0x115)
#define S905D3_GPIOC_PUPD_REG                   (S905D3_GPIO_BASE + 0x13B)
#define S905D3_GPIOC_PUEN_REG                   (S905D3_GPIO_BASE + 0x149)
#define S905D3_GPIOC_MUX_REG                    (S905D3_GPIO_BASE + 0x1B9)

//GPIOH
#define S905D3_GPIOH_OUT_EN_REG                 (S905D3_GPIO_BASE + 0x119)
#define S905D3_GPIOH_OUT_REG                    (S905D3_GPIO_BASE + 0x11A)
#define S905D3_GPIOH_IN_REG                     (S905D3_GPIO_BASE + 0x11B)
#define S905D3_GPIOH_PUPD_REG                   (S905D3_GPIO_BASE + 0x13D)
#define S905D3_GPIOH_PUEN_REG                   (S905D3_GPIO_BASE + 0x14B)
#define S905D3_GPIOH_MUX_REG1                   (S905D3_GPIO_BASE + 0x1BB)
#define S905D3_GPIOH_MUX_REG2                   (S905D3_GPIO_BASE + 0x1BC)

//GPIOAO
#define S905D3_GPIOAO_OUT_EN_REG                (S905D3_GPIO_AO_BASE + 0x109)
#define S905D3_GPIOAO_OUT_REG                   (S905D3_GPIO_AO_BASE + 0x10D)
#define S905D3_GPIOAO_IN_REG                    (S905D3_GPIO_AO_BASE + 0x10A)
#define S905D3_GPIOAO_PUPD_REG                  (S905D3_GPIO_AO_BASE + 0x10B)
#define S905D3_GPIOAO_PUEN_REG                  (S905D3_GPIO_AO_BASE + 0x10C)
#define S905D3_GPIOAO_MUX_REG1                  (S905D3_GPIO_AO_BASE + 0x105)
#define S905D3_GPIOAO_MUX_REG2                  (S905D3_GPIO_AO_BASE + 0x106)

//for short——gpio
#define S905D3_GPIO_OUT_EN                      (s905d3_gpio_info_t.gpio_out_en)
#define S905D3_GPIO_OUT                         (s905d3_gpio_info_t.gpio_out)
#define S905D3_GPIO_IN                          (s905d3_gpio_info_t.gpio_in)
#define S905D3_GPIO_PUPD                        (s905d3_gpio_info_t.gpio_pupd)
#define S905D3_GPIO_PUEN                        (s905d3_gpio_info_t.gpio_puen)
#define S905D3_GPIO_MUX                         (s905d3_gpio_info_t.gpio_mux)
#define S905D3_GPIO_OUT_EN_OFFSET               (s905d3_gpio_info_t.gpio_out_en_offset)
#define S905D3_GPIO_OUT_OFFSET                  (s905d3_gpio_info_t.gpio_out_offset)
#define S905D3_GPIO_IN_OFFSET                   (s905d3_gpio_info_t.gpio_in_offset)
#define S905D3_GPIO_PUPD_OFFSET                 (s905d3_gpio_info_t.gpio_pupd_offset)
#define S905D3_GPIO_PUEN_OFFSET                 (s905d3_gpio_info_t.gpio_puen_offset)
#define S905D3_GPIO_MUX_OFFSET                  (s905d3_gpio_info_t.gpio_mux_offset)

//PWM
#define S905D3_PWM_DUTY_CYCLE_F_REG             (S905D3_GPIO_PWM_BASE + 0x01)
#define S905D3_PWM_MISC_EF_REG                  (S905D3_GPIO_PWM_BASE + 0x02)
#define S905D3_PWM_DUTY_CYCLE_AO_C_REG          (S905D3_GPIO_PWM_AO_BASE)
#define S905D3_PWM_MISC_AO_CD_REG               (S905D3_GPIO_PWM_AO_BASE + 0x02)

//for short——pwm
#define S905D3_PWM_DUTY_CYCLE                   (s905d3_gpio_info_t.pwm_duty_cycle)
#define S905D3_PWM_MISC                         (s905d3_gpio_info_t.pwm_misc)

//PWM_MISC_OFFSET
#define S905D3_PWM_CLK_EN_1                     (23)
#define S905D3_PWM_CLK_DIV_1                    (16)    // 22 ~ 16
#define S905D3_PWM_CLK_EN_0                     (15)
#define S905D3_PWM_CLK_DIV_0                    (8)     // 8 ~ 14
#define S905D3_PWM_CLK_SEL_1                    (6)     // 7 ~ 6
#define S905D3_PWM_CLK_SEL_0                    (4)     // 5 ~ 4
#define S905D3_PWM_EN_1                         (1)
#define S905D3_PWM_EN_0                         (0)

typedef struct {
	unsigned int * gpio_base;
	unsigned int * gpio_ao_base;
	unsigned int * gpio_pwm_base;
	unsigned int * gpio_pwm_ao_base;
	unsigned int gpio_out_en;
	unsigned int gpio_out;
	unsigned int gpio_in;
	unsigned int gpio_pupd;
	unsigned int gpio_puen;
	unsigned int gpio_mux;
	unsigned int gpio_out_en_offset;
	unsigned int gpio_out_offset;
	unsigned int gpio_in_offset;
	unsigned int gpio_pupd_offset;
	unsigned int gpio_puen_offset;
	unsigned int gpio_mux_offset;
	unsigned int pwm_duty_cycle;
	unsigned int pwm_misc;
} s905d3_gpio_info;

/*************** S905D3 ******************/


// Mask for the bottom 64 pins which belong to the Raspberry Pi
//	The others are available for the other devices


#define	PI_GPIO_MASK	(0xFFFFFFC0)


// Handy defines
extern int wiringPiDebug;

// wiringPi modes

#define WPI_MODE_PINS            0
#define WPI_MODE_GPIO            1
#define WPI_MODE_GPIO_SYS        2
#define WPI_MODE_PHYS            3
#define WPI_MODE_PIFACE          4
#define WPI_MODE_UNINITIALISED  -1

// Pin modes

#define INPUT                    0
#define OUTPUT                   1
#define PWM_OUTPUT               2
#define GPIO_CLOCK               3
#define SOFT_PWM_OUTPUT          4
#define SOFT_TONE_OUTPUT         5
#define PWM_TONE_OUTPUT          6

#define LOW                      0
#define HIGH                     1

// Pull up/down/none

#define PUD_OFF                  0
#define PUD_DOWN                 1
#define PUD_UP                   2

// PWM

#define PWM_MODE_MS              0
#define PWM_MODE_BAL             1

// Interrupt levels

#define INT_EDGE_SETUP           0
#define INT_EDGE_FALLING         1
#define INT_EDGE_RISING          2
#define INT_EDGE_BOTH            3

// Pi model types and version numbers
//	Intended for the GPIO program Use at your own risk.

/* Allwinner H6 */
#define PI_MODEL_3               0
#define PI_MODEL_LTIE_2          1

/* Allwinner H2+ */
#define PI_MODEL_ZERO            2

/* Allwinner H3 */
#define PI_MODEL_H3              3
#define PI_MODEL_ZERO_PLUS_2     4

/* Allwinner A64 */
#define PI_MODEL_WIN             5

/* Allwinner H5 */
#define PI_MODEL_PRIME           6
#define PI_MODEL_PC_2            7
#define PI_MODEL_ZERO_PLUS       8

/* Allwinner H616 */
#define PI_MODEL_ZERO_2          9
#define PI_MODEL_ZERO_2_W        10


/* Rockchip RK3399 */
#define PI_MODEL_800             15
#define PI_MODEL_4               16
#define PI_MODEL_4_LTS           17
#define PI_MODEL_RK3399          18

/* Rockchip RK3328 */
#define PI_MODEL_R1_PLUS         22

/* Rockchip RK3588(s) */
#define PI_MODEL_900             23
#define PI_MODEL_5               24
#define PI_MODEL_5B              25
#define PI_MODEL_5_PLUS          26

/* Rockchip RK3566 */
#define PI_MODEL_CM4             27
#define PI_MODEL_3B              28

/* S905D3 */
#define PI_MODEL_3_PLUS          29

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

extern          void piBoardId(int *model) ;
extern           int wpiPinToGpio(int wpiPin) ;
extern           int physPinToGpio(int physPin) ;
extern          void setPadDrive(int group, int value) ;
extern           int getAlt(int pin) ;
extern          void H618_set_pwm_reg(int pin,sunxi_gpio_info *sunxi_gpio_info_ptr) ;
extern          void s905d3_set_gpio_reg(int pin,s905d3_gpio_info *s905d3_gpio_info_ptr) ;
extern          void rk3588_set_pwm_reg(int pin,rk3588_soc_info *rk3588_soc_info_ptr) ;
extern          void rk3566_set_pwm_reg(int pin,rk3566_soc_info *rk3566_soc_info_ptr) ;
extern          void sunxi_pwm_set_enable(int en) ;
extern          void pwmToneWrite(int pin, int freq) ;
extern          void pwmSetMode(int pin,int mode) ;
extern          void pwmSetRange(int pin,unsigned int range) ;
extern          void pwmSetClock(int pin,int divisor) ;
extern          void gpioClockSet(int pin, int freq) ;
extern unsigned  int digitalReadByte(void) ;
extern unsigned  int digitalReadByte2(void) ;
extern          void digitalWriteByte(int value) ;
extern          void digitalWriteByte2(int value) ;

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
extern void sunxi_pwm_set_act(int pin,int act_cys);
extern void sunxi_pwm_set_period(int pin,unsigned int period_cys);
extern void sunxi_pwm_set_clk(int pin,int clk);
extern void sunxi_pwm_set_tone(int pin,int freq);

void set_soc_info(void);

#ifdef __cplusplus
}
#endif

#endif
