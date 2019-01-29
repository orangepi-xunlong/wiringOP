/*
 * Copyright (c) 2015, Vladimir Komendantskiy
 * MIT License
 *
 * SSD1306 demo of block and font drawing.
 */

//
// fixed for OrangePiZero by HypHop
//

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "oled.h"
#include "font.h"

int oled_demo(struct display_info *disp) {
	int i;
	char buf[100];

	//putstrto(disp, 0, 0, "Spnd spd  2468 rpm");
	//	oled_putstrto(disp, 0, 9+1, "Spnd cur  0.46 A");
	oled_putstrto(disp, 0, 9+1, "Welcome       to");
	disp->font = font1;
	//	oled_putstrto(disp, 0, 18+2, "Spnd tmp    53 C");
	oled_putstrto(disp, 0, 18+2, "----OrangePi----");
	disp->font = font2;
	//	oled_putstrto(disp, 0, 27+3, "DrvX tmp    64 C");
	oled_putstrto(disp, 0, 27+3, "This is 0.96OLED");
	oled_putstrto(disp, 0, 36+4, "");
	oled_putstrto(disp, 0, 45+5, "");
	disp->font = font1;
	//	oled_putstrto(disp, 0, 54, "Total cur  2.36 A");
	oled_putstrto(disp, 0, 54, "*****************");
	oled_send_buffer(disp);

	disp->font = font3;
	for (i=0; i<100; i++) {
		sprintf(buf, "Spnd spd  %d rpm", i);
		oled_putstrto(disp, 0, 0, buf);
		oled_putstrto(disp, 135-i, 36+4, "===");
		oled_putstrto(disp, 100, 0+i/2, ".");
		oled_send_buffer(disp);
	}
	//oled_putpixel(disp, 60, 45);
	//oled_putstr(disp, 1, "hello");

return 0;
}

void show_error(int err, int add) {
	//const gchar* errmsg;
	//errmsg = g_strerror(errno);
	printf("\nERROR: %i, %i\n\n", err, add);
	//printf("\nERROR\n");
}

void show_usage(char *progname) {
	printf("\nUsage:\n%s <I2C bus device node >\n", progname);
}

int main(int argc, char **argv) {
	int e;
	char filename[32];
	struct display_info disp;

	if (argc < 2) {
		show_usage(argv[0]);
		
		return -1;
	}

	memset(&disp, 0, sizeof(disp));
	sprintf(filename, "%s", argv[1]);
	disp.address = OLED_I2C_ADDR;
	disp.font = font2;

	e = oled_open(&disp, filename);

	if (e < 0) {
		show_error(1, e);
	} else {
		e = oled_init(&disp);
	if (e < 0) {
		show_error(2, e);
	} else {
		printf("---------start--------\n");
		if (oled_demo(&disp) < 0)
			show_error(3, 777);
			printf("----------end---------\n");
		}
	}

	return 0;
}
