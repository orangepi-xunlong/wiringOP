/*
 * pinout.c:
 *	Renderer for "gpio pinout": board facts, the main header as a
 *	mirrored table that matches the physical pin layout, and an
 *	optional secondary header next to it.
 *
 *	No hardware access in this file, see pinout.h.
 *
 *	Colour is used only on a TTY, never when NO_COLOR is set.
 *	Only the basic ANSI colours are needed; two extra hues (USB,
 *	audio) use the 256 colour palette when the terminal has one.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "pinout.h"

#define	C_DIM	"90"
#define	C_BOLD	"1;97"
#define	C_NOTE	"33"

static int useColor = 0 ;
static int use256   = 0 ;

static const char *kindLabels [PINOUT_KIND_COUNT] =
{
	"5V", "3.3V", "GND", "GPIO", "I2C", "UART", "SPI", "USB", "Audio", "Video"
} ;

static const char *kindCode (int kind)
{
	switch (kind)
	{
		case PINOUT_KIND_5V:	return "31" ;
		case PINOUT_KIND_3V3:	return "36" ;
		case PINOUT_KIND_GND:	return "90" ;
		case PINOUT_KIND_GPIO:	return "32" ;
		case PINOUT_KIND_I2C:	return "35" ;
		case PINOUT_KIND_UART:	return "33" ;
		case PINOUT_KIND_SPI:	return "34" ;
		case PINOUT_KIND_USB:	return use256 ? "38;5;208" : "37" ;
		case PINOUT_KIND_AUDIO:	return use256 ? "38;5;213" : "37" ;
		case PINOUT_KIND_VIDEO:	return "97" ;
	}
	return NULL ;
}

/*
 * cell:
 *	Print text in a fixed width field. The padding is printed outside
 *	the escape codes so the columns line up with or without colour.
 */

static void cell (FILE *out, const char *text, int width, int right, const char *code)
{
	int len = (int)strlen (text) ;
	int pad ;

	if (len > width)
		len = width ;
	pad = width - len ;

	if (right)
		fprintf (out, "%*s", pad, "") ;

	if (useColor && (code != NULL) && (len > 0))
		fprintf (out, "\033[%sm%.*s\033[0m", code, len, text) ;
	else
		fprintf (out, "%.*s", len, text) ;

	if (!right)
		fprintf (out, "%*s", pad, "") ;
}

static void trimCopy (const char *src, char *dst, size_t size)
{
	size_t len ;

	while (*src == ' ')
		++src ;

	strncpy (dst, src, size - 1) ;
	dst [size - 1] = 0 ;

	len = strlen (dst) ;
	while ((len > 0) && (dst [len - 1] == ' '))
		dst [--len] = 0 ;
}

static int hasWord (const char *name, const char *word)
{
	size_t n = strlen (word) ;

	for ( ; *name ; ++name)
		if (strncasecmp (name, word, n) == 0)
			return 1 ;
	return 0 ;
}

/*
 * classify:
 *	Work out the kind of a main header pin from its wiringOP name.
 *	SPI is tested before I2C because "SCLK" also starts with "SCL".
 */

static int classify (const char *name)
{
	if (hasWord (name, "5V"))
		return PINOUT_KIND_5V ;
	if (hasWord (name, "3.3V") || hasWord (name, "3V3"))
		return PINOUT_KIND_3V3 ;
	if (hasWord (name, "GND"))
		return PINOUT_KIND_GND ;

	if (hasWord (name, "SPI")  || hasWord (name, "MOSI") || hasWord (name, "MISO") ||
	    hasWord (name, "SCLK") || hasWord (name, "CE.")  || hasWord (name, "CS."))
		return PINOUT_KIND_SPI ;

	if (hasWord (name, "I2C") || hasWord (name, "TWI") || hasWord (name, "SDA") ||
	    hasWord (name, "SCL") || hasWord (name, "SCK"))
		return PINOUT_KIND_I2C ;

	if (hasWord (name, "UART") || hasWord (name, "TXD") || hasWord (name, "RXD") ||
	    hasWord (name, "CTS")  || hasWord (name, "RTS"))
		return PINOUT_KIND_UART ;

	return PINOUT_KIND_GPIO ;
}

// The text and colour of every column for one side of a table row

typedef struct
{
	char mode [32], gpio [16], wpi [16], soc [16], name [16], pin [16] ;
	const char *cMode, *cNum, *cName, *cPin ;
} rowText ;

static void fillRow (rowText *r, const pinout_pin *p, int phys, unsigned *kinds)
{
	char mode [16] ;
	int  kind ;

	memset (r, 0, sizeof (*r)) ;

	trimCopy ((p->name != NULL) ? p->name : "", r->name, sizeof (r->name)) ;
	snprintf (r->pin, sizeof (r->pin), "%d", phys) ;
	snprintf (r->soc, sizeof (r->soc), "%s", p->soc) ;

	if (p->wpi >= 0)
		snprintf (r->wpi, sizeof (r->wpi), "%d", p->wpi) ;
	if (p->gpio >= 0)
		snprintf (r->gpio, sizeof (r->gpio), "%d", p->gpio) ;

	r->cMode = C_DIM ;
	if ((p->mode != NULL) && (p->wpi >= 0))
	{
		trimCopy (p->mode, mode, sizeof (mode)) ;
		if (strcasecmp (mode, "OFF") == 0)
			snprintf (r->mode, sizeof (r->mode), "off") ;
		else
		{
			snprintf (r->mode, sizeof (r->mode), "%s %d", mode, p->value) ;
			r->cMode = C_BOLD ;
		}
	}

	kind      = classify (r->name) ;
	*kinds   |= 1u << kind ;
	r->cName  = kindCode (kind) ;
	r->cNum   = C_DIM ;
	r->cPin   = C_BOLD ;
}

static void headRow (rowText *r)
{
	memset (r, 0, sizeof (*r)) ;
	strcpy (r->mode, "Mode V") ; strcpy (r->gpio, "GPIO") ; strcpy (r->wpi, "wPi") ;
	strcpy (r->soc,  "SoC") ;    strcpy (r->name, "Name") ; strcpy (r->pin, "Pin") ;
	r->cMode = r->cNum = r->cName = r->cPin = C_DIM ;
}

/*
 * Column layout, from the outside in towards the pin numbers:
 *	Mode V, [GPIO], [wPi], SoC, Name, Pin | Pin, Name, SoC, [wPi], [GPIO], Mode V
 */

static int leftWidth (int flags)
{
	return 8 + ((flags & PINOUT_SHOW_GPIO) ? 6 : 0) + ((flags & PINOUT_SHOW_WPI) ? 5 : 0) + ((flags & PINOUT_DEMO) ? 0 : 6) + 1 + 8 + 1 + 4 ;
}

static int rightWidth (int flags)
{
	return 3 + 2 + 8 + ((flags & PINOUT_DEMO) ? 0 : 6) + ((flags & PINOUT_SHOW_WPI) ? 5 : 0) + ((flags & PINOUT_SHOW_GPIO) ? 6 : 0) + 1 + 6 ;
}

static void printLeft (FILE *out, const rowText *r, int flags)
{
	cell (out, r->mode, 8, 1, r->cMode) ;
	if (flags & PINOUT_SHOW_GPIO)
		cell (out, r->gpio, 6, 1, r->cNum) ;
	if (flags & PINOUT_SHOW_WPI)
		cell (out, r->wpi, 5, 1, r->cNum) ;
	if (!(flags & PINOUT_DEMO))
		cell (out, r->soc, 6, 1, r->cNum) ;
	fputc (' ', out) ;
	cell (out, r->name, 8, 1, r->cName) ;
	fputc (' ', out) ;
	cell (out, r->pin, 4, 1, r->cPin) ;
}

static void printRight (FILE *out, const rowText *r, int flags, int padEnd)
{
	cell (out, r->pin, 3, 0, r->cPin) ;
	fputs ("  ", out) ;

	// Power and ground pins end after the name: no trailing blanks
	// unless another table follows on the same line

	if (!padEnd && (r->wpi [0] == 0) && (r->mode [0] == 0))
	{
		cell (out, r->name, (int)strlen (r->name), 0, r->cName) ;
		return ;
	}

	cell (out, r->name, 8, 0, r->cName) ;
	if (!(flags & PINOUT_DEMO))
	{
		fputc (' ', out) ;
		cell (out, r->soc, 5, 0, r->cNum) ;
	}
	if (flags & PINOUT_SHOW_WPI)
	{
		cell (out, r->wpi, 4, 1, r->cNum) ;
		fputc (' ', out) ;
	}
	if (flags & PINOUT_SHOW_GPIO)
	{
		cell (out, r->gpio, 5, 1, r->cNum) ;
		fputc (' ', out) ;
	}
	fputc (' ', out) ;
	cell (out, r->mode, padEnd ? 6 : (int)strlen (r->mode), 0, r->cMode) ;
}

static void dashes (FILE *out, int n)
{
	char buf [128] ;

	if (n > (int)sizeof (buf) - 1)
		n = (int)sizeof (buf) - 1 ;
	memset (buf, '-', n) ;
	buf [n] = 0 ;
	cell (out, buf, n, 0, C_DIM) ;
}

#define	AUX_GAP		4
#define	AUX_WIDTH	14	// "Pin" + 2 + name of up to 9

static void printAux (FILE *out, const pinout_aux_pin *a, unsigned *kinds)
{
	char num [16] ;

	snprintf (num, sizeof (num), "%d", a->pin) ;
	cell (out, num, 3, 1, C_BOLD) ;
	fputs ("  ", out) ;
	cell (out, a->name, (int)strlen (a->name), 0, kindCode (a->kind)) ;
	*kinds |= 1u << a->kind ;
}

static void printAuxHead (FILE *out, int line)
{
	if (line == 0)
		cell (out, "Pin  Name", 9, 0, C_DIM) ;
	else
		cell (out, "---  ---------", 14, 0, C_DIM) ;
}

static long roundRam (long mb)
{
	long step = (mb > 2048) ? 1024 : 256 ;

	return ((mb + step - 1) / step) * step ;
}

static void infoLine (FILE *out, const char *label, const char *value, const char *note)
{
	if (value == NULL)
		return ;

	fprintf (out, "%-18s: %s", label, value) ;
	if (note != NULL)
	{
		fputc (' ', out) ;
		cell (out, note, (int)strlen (note), 0, C_NOTE) ;
	}
	fputc ('\n', out) ;
}

static int detectWidth (FILE *out)
{
	struct winsize ws ;

	if (!isatty (fileno (out)))
		return 0 ;	// piped: always stack the tables
	if ((ioctl (fileno (out), TIOCGWINSZ, &ws) == 0) && (ws.ws_col > 0))
		return ws.ws_col ;
	return 80 ;
}

void pinoutSunxiPinName (int gpio, char *buf, size_t size)
{
	if (gpio < 0)
		buf [0] = 0 ;
	else
		snprintf (buf, size, "P%c%d", 'A' + gpio / 32, gpio % 32) ;
}

void pinoutRender (FILE *out, const pinout_board *board,
	const pinout_pin *pins, int pinCount, long ramMB, int width, int flags)
{
	const char *term = getenv ("TERM") ;
	const char *noColor = getenv ("NO_COLOR") ;
	char     title [32], ram [32] ;
	rowText  l, r ;
	unsigned kinds = 0 ;
	int      rows  = (pinCount + 1) / 2 ;	// an odd count leaves the last right hand cell empty
	int      wl    = leftWidth (flags) ;
	int      wr    = rightWidth (flags) ;
	int      total = wl + 3 + wr ;
	int      hasAux, beside, lines, i, k, first ;

	useColor = isatty (fileno (out)) && ((noColor == NULL) || (*noColor == 0)) ;
	if (flags & PINOUT_FORCE_COLOR)
		useColor = 1 ;
	if (flags & PINOUT_MONOCHROME)
		useColor = 0 ;

	use256 = ((term != NULL) && (strstr (term, "256color") != NULL)) || (getenv ("COLORTERM") != NULL) ;

	if (width == 0)
		width = detectWidth (out) ;

	hasAux = (board != NULL) && (board->aux != NULL) && (board->auxCount > 0) ;
	beside = hasAux && (width >= total + AUX_GAP + AUX_WIDTH) ;

	// Board facts

	if (board != NULL)
	{
		infoLine (out, "Board", board->board, NULL) ;
		infoLine (out, "SoC",   board->soc,   NULL) ;
		if (ramMB > 0)
		{
			snprintf (ram, sizeof (ram), "%ldMB", roundRam (ramMB)) ;
			infoLine (out, "RAM", ram, NULL) ;
		}
		infoLine (out, "Storage",        board->storage,   NULL) ;
		infoLine (out, "USB ports",      board->usb,       board->usbNote) ;
		infoLine (out, "Ethernet ports", board->ethernet,  NULL) ;
		infoLine (out, "Wi-fi",          board->wifi,      NULL) ;
		infoLine (out, "Bluetooth",      board->bluetooth, NULL) ;
		infoLine (out, "Headers",        board->headers,   NULL) ;
		fputc ('\n', out) ;
	}

	// Titles, column labels, rule

	snprintf (title, sizeof (title), "%d-pin header", pinCount) ;
	cell (out, title, beside ? total : (int)strlen (title), 0, C_BOLD) ;
	if (beside)
	{
		fprintf (out, "%*s", AUX_GAP, "") ;
		cell (out, board->auxTitle, (int)strlen (board->auxTitle), 0, C_BOLD) ;
	}
	fputc ('\n', out) ;

	headRow (&l) ;
	printLeft (out, &l, flags) ;
	cell (out, " | ", 3, 0, C_DIM) ;
	printRight (out, &l, flags, beside) ;
	if (beside)
	{
		fprintf (out, "%*s", AUX_GAP, "") ;
		printAuxHead (out, 0) ;
	}
	fputc ('\n', out) ;

	dashes (out, wl) ;
	cell (out, "-+-", 3, 0, C_DIM) ;
	dashes (out, wr) ;
	if (beside)
	{
		fprintf (out, "%*s", AUX_GAP, "") ;
		printAuxHead (out, 1) ;
	}
	fputc ('\n', out) ;

	// Pin rows, with the secondary header alongside when it fits

	lines = rows ;
	if (beside && (board->auxCount > lines))
		lines = board->auxCount ;

	for (i = 0 ; i < lines ; ++i)
	{
		if (i < rows)
		{
			fillRow (&l, &pins [i * 2],     i * 2 + 1, &kinds) ;
			printLeft (out, &l, flags) ;
			cell (out, " | ", 3, 0, C_DIM) ;
			if (i * 2 + 2 <= pinCount)
			{
				fillRow (&r, &pins [i * 2 + 1], i * 2 + 2, &kinds) ;
				printRight (out, &r, flags, beside) ;
			}
			else if (beside)
				fprintf (out, "%*s", wr, "") ;
		}
		else
			fprintf (out, "%*s", total, "") ;

		if (beside && (i < board->auxCount))
		{
			fprintf (out, "%*s", AUX_GAP, "") ;
			printAux (out, &board->aux [i], &kinds) ;
		}
		fputc ('\n', out) ;
	}

	// Secondary header underneath when the terminal is too narrow

	if (hasAux && !beside)
	{
		fputc ('\n', out) ;
		cell (out, board->auxTitle, (int)strlen (board->auxTitle), 0, C_BOLD) ;
		fputc ('\n', out) ;
		printAuxHead (out, 0) ; fputc ('\n', out) ;
		printAuxHead (out, 1) ; fputc ('\n', out) ;
		for (i = 0 ; i < board->auxCount ; ++i)
		{
			printAux (out, &board->aux [i], &kinds) ;
			fputc ('\n', out) ;
		}
	}

	// One legend for everything that was printed

	fputc ('\n', out) ;
	first = 1 ;
	for (k = 0 ; k < PINOUT_KIND_COUNT ; ++k)
	{
		if ((kinds & (1u << k)) == 0)
			continue ;
		if (!first)
			fputs ("  ", out) ;
		cell (out, kindLabels [k], (int)strlen (kindLabels [k]), 0, kindCode (k)) ;
		first = 0 ;
	}
	fputc ('\n', out) ;

	{
		char note [128] ;

		if (flags & PINOUT_DEMO)
			snprintf (note, sizeof (note), "demo: the pin state was not read") ;
		else
			snprintf (note, sizeof (note), "%soff: unclaimed%s%s%s",
			useColor ? "bold: pin in use, " : "",
			hasAux ? ", " : "", hasAux ? board->auxTitle : "", hasAux ? " has no live state" : "") ;
		cell (out, note, (int)strlen (note), 0, C_DIM) ;
		fputc ('\n', out) ;
	}
}
