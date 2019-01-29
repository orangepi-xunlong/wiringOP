/*
 * Copyright (c) 2015, Vladimir Komendantskiy
 * MIT License
 *
 * IOCTL interface to SSD1306 modules.
 *
 * Command sequences are sourced from an Arduino library by Sonal Pinto.
 */

#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include "font.h"

#define OLED_I2C_ADDR                   0x3c

// Control byte
#define OLED_CTRL_BYTE_CMD_SINGLE       0x80
#define OLED_CTRL_BYTE_CMD_STREAM       0x00
#define OLED_CTRL_BYTE_DATA_STREAM      0x40
// Fundamental commands (page 28)
#define OLED_CMD_SET_CONTRAST           0x81
#define OLED_CMD_DISPLAY_RAM            0xA4
#define OLED_CMD_DISPLAY_ALLON          0xA5
#define OLED_CMD_DISPLAY_NORMAL         0xA6
#define OLED_CMD_DISPLAY_INVERTED       0xA7
#define OLED_CMD_DISPLAY_OFF            0xAE
#define OLED_CMD_DISPLAY_ON             0xAF
// Addressing Command Table (page 30)
#define OLED_CMD_SET_MEMORY_ADDR_MODE   0x20
#define OLED_CMD_SET_COLUMN_RANGE       0x21
#define OLED_CMD_SET_PAGE_RANGE         0x22
// Hardware Config (page 31)
#define OLED_CMD_SET_DISPLAY_START_LINE 0x40
#define OLED_CMD_SET_SEGMENT_REMAP      0xA1
#define OLED_CMD_SET_MUX_RATIO          0xA8
#define OLED_CMD_SET_COM_SCAN_MODE      0xC8
#define OLED_CMD_SET_DISPLAY_OFFSET     0xD3
#define OLED_CMD_SET_COM_PIN_MAP        0xDA
#define OLED_CMD_NOP                    0xE3
// Timing and Driving Scheme (page 32)
#define OLED_CMD_SET_DISPLAY_CLK_DIV    0xD5
#define OLED_CMD_SET_PRECHARGE          0xD9
#define OLED_CMD_SET_VCOMH_DESELCT      0xDB
// Charge Pump (page 62)
#define OLED_CMD_SET_CHARGE_PUMP        0x8D
// SH1106 Display
#define OLED_SET_PAGE_ADDRESS            0xB0


static const unsigned char display_config[] = {
	OLED_CTRL_BYTE_CMD_STREAM,
	OLED_CMD_DISPLAY_OFF,

	OLED_SET_PAGE_ADDRESS,
	0x02, /*set lower column address*/
	0x10, /*set higher column address*/

	OLED_CMD_SET_MUX_RATIO, 0x3F,
	// Set the display offset to 0
	OLED_CMD_SET_DISPLAY_OFFSET, 0x00,
	// Display start line to 0
	OLED_CMD_SET_DISPLAY_START_LINE,
	// Mirror the x-axis. In case you set it up such that the pins are north.
	// 0xA0 - in case pins are south - default
	OLED_CMD_SET_SEGMENT_REMAP,
	// Mirror the y-axis. In case you set it up such that the pins are north.
	// 0xC0 - in case pins are south - default
	OLED_CMD_SET_COM_SCAN_MODE,
	// Default - alternate COM pin map
	OLED_CMD_SET_COM_PIN_MAP, 0x12,
	// set contrast
	OLED_CMD_SET_CONTRAST, 0x7F,
	// Set display to enable rendering from GDDRAM (Graphic Display Data RAM)
	OLED_CMD_DISPLAY_RAM,
	// Normal mode!
	OLED_CMD_DISPLAY_NORMAL,
	// Default oscillator clock
	OLED_CMD_SET_DISPLAY_CLK_DIV, 0x80,
	// Enable the charge pump
	OLED_CMD_SET_CHARGE_PUMP, 0x14,
	// Set precharge cycles to high cap type
	OLED_CMD_SET_PRECHARGE, 0x22,
	// Set the V_COMH deselect volatage to max
	OLED_CMD_SET_VCOMH_DESELCT, 0x30,
	// Horizonatal addressing mode - same as the KS108 GLCD
	OLED_CMD_SET_MEMORY_ADDR_MODE, 0x00,
	// Turn the Display ON
	OLED_CMD_DISPLAY_ON
};

static const unsigned char display_draw[] = {
	OLED_CTRL_BYTE_CMD_STREAM,
	// column 0 to 127
	OLED_CMD_SET_COLUMN_RANGE,
	0x00,
	0x7F,
	// page 0 to 7
	OLED_CMD_SET_PAGE_RANGE,
	0x00,
	0x07
};

struct display_info {
	int address;
	int file;
	struct font_info font;
	uint8_t buffer[8][128];
};

struct sized_array {
	int size;
	const uint8_t* array;
};

extern int oled_close       (struct display_info* disp);
extern int oled_open        (struct display_info* disp, char* filename);
extern int oled_send        (struct display_info* disp, struct sized_array* payload);
extern int oled_init        (struct display_info* disp);
extern int oled_send_buffer (struct display_info* disp);
extern void oled_clear(struct display_info *disp);
extern void oled_putstr(struct display_info *disp, uint8_t line, uint8_t *str);
extern void oled_putpixel(struct display_info *disp, uint8_t x, uint8_t y, uint8_t on);
extern void oled_putstrto(struct display_info *disp, uint8_t x, uint8_t y, char *str);

#endif // OLED_H
