/*
 * pinout.h:
 *	Renderer for "gpio pinout": a colour-coded view of the board
 *	headers. Pure printing code, no hardware access in here: the
 *	caller fills the structs below and hands them to pinoutRender().
 */

#ifndef WIRINGOP_GPIO_PINOUT_H
#define WIRINGOP_GPIO_PINOUT_H

#include <stdio.h>
#include <stddef.h>

// Flags for pinoutRender

#define	PINOUT_SHOW_WPI		(1 << 0)	// add the wPi number column
#define	PINOUT_SHOW_GPIO	(1 << 1)	// add the Linux GPIO number column
#define	PINOUT_MONOCHROME	(1 << 2)	// never emit colour
#define	PINOUT_FORCE_COLOR	(1 << 3)	// emit colour even when not on a TTY
#define	PINOUT_DEMO		(1 << 4)	// table data only: no SoC column, state not read

// Pin kinds, used for the colour coding and the legend

enum
{
	PINOUT_KIND_5V = 0,
	PINOUT_KIND_3V3,
	PINOUT_KIND_GND,
	PINOUT_KIND_GPIO,
	PINOUT_KIND_I2C,
	PINOUT_KIND_UART,
	PINOUT_KIND_SPI,
	PINOUT_KIND_USB,
	PINOUT_KIND_AUDIO,
	PINOUT_KIND_VIDEO,
	PINOUT_KIND_COUNT
} ;

// One pin of the main (GPIO) header. Pins are passed in physical
// order: element 0 is physical pin 1.

typedef struct
{
	const char *name ;	// wiringOP name, padding is trimmed ("   SDA.0")
	char        soc [16] ;	// SoC pin name ("PA12"), empty for power pins
	int         wpi ;	// wPi number, -1 for power and ground
	int         gpio ;	// Linux GPIO number, -1 for power and ground
	const char *mode ;	// "ALT2", "OUT", "IN", "OFF"; NULL for power pins
	int         value ;	// digitalRead result
} pinout_pin ;

// One pin of a secondary header that carries no GPIO state
// (USB, audio, video...). The kind is given explicitly.

typedef struct
{
	int         pin ;
	const char *name ;
	int         kind ;
} pinout_aux_pin ;

// Static facts about a board. Optional: pass NULL to pinoutRender
// and only the main header table is printed.

typedef struct
{
	const char *board ;
	const char *soc ;
	const char *storage ;
	const char *usb ;
	const char *usbNote ;		// printed highlighted after usb, may be NULL
	const char *ethernet ;
	const char *wifi ;
	const char *bluetooth ;
	const char *headers ;

	const char           *auxTitle ;	// "13-pin header", NULL if none
	const pinout_aux_pin *aux ;		// in display order, top to bottom
	int                   auxCount ;
} pinout_board ;

// ramMB: MemTotal in MB, 0 to skip the RAM line.
// width: terminal width in columns, 0 to detect it.

extern void pinoutRender (FILE *out, const pinout_board *board,
	const pinout_pin *pins, int pinCount, long ramMB, int width, int flags) ;

// Allwinner helper: Linux GPIO number to SoC pin name (12 -> "PA12")

extern void pinoutSunxiPinName (int gpio, char *buf, size_t size) ;

#endif
