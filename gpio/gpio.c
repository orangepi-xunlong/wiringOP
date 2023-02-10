/*
 * gpio.c:
 *	Swiss-Army-Knife, Set-UID command-line interface to the Raspberry
 *	Pi's GPIO.
 *	Copyright (c) 2012-2018 Gordon Henderson
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


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <wiringPi.h>
#include <wpiExtensions.h>

#include <gertboard.h>
#include <piFace.h>

#include "../version.h"

extern int wiringPiDebug ;

// External functions I can't be bothered creating a separate .h file for:

extern void doReadall    (void) ;
extern void doAllReadall (void) ;
extern void doQmode      (int argc, char *argv []) ;

#ifndef TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(1==2)
#endif

#define	PI_USB_POWER_CONTROL	38
#define	I2CDETECT		"i2cdetect"
#define	MODPROBE		"modprobe"
#define	RMMOD			"rmmod"

int wpMode ;

#ifdef CONFIG_ORANGEPI
const char *piModelNames[6] =
{
    "Unknown",
    "Model A",
    "Model B",
    "Model B+",
    "Compute Module",
#ifdef CONFIG_ORANGEPI_2G_IOT
	"OrangePi 2G-IOT",
#elif CONFIG_ORANGEPI_PC2 || CONFIG_ORANGEPI_ZEROPLUS2_H5 || CONFIG_ORANGEPI_ZEROPLUS || CONFIG_ORANGEPI_PRIME
	"OrangePi PC2",
#elif CONFIG_ORANGEPI_A64
	"OrangePi Win/Winplus",
#elif CONFIG_ORANGEPI_H3 || CONFIG_ORANGEPI_ZEROPLUS2_H3
	"OrangePi H3 family",
#elif CONFIG_ORANGEPI_ZERO
	"OrangePi Zero",
#endif
};
#endif

char *usage = "Usage: gpio -v\n"
              "       gpio -h\n"
              "       gpio [-g|-1] ...\n"
     //         "       gpio [-d] ...\n"
     //         "       [-x extension:params] [[ -x ...]] ...\n"
     //         "       gpio [-p] <read/write/wb> ...\n"
              "       gpio <mode/qmode/read/write/pwm> ...\n"
     //		  "       gpio <mode/qmode/read/write> ...\n"
              "       gpio <toggle/blink> <pin>\n"
	      "       gpio readall\n"
	      "       gpio unexportall/exports\n"
	      "       gpio export/edge/unexport ...\n"
	//      "       gpio wfi <pin> <mode>\n"
	//      "       gpio drive <group> <value>\n"
	     "       gpio pwm-bal/pwm-ms \n"
	//     "       gpio pwmr <range> \n"
	//     "       gpio pwmc <divider> \n"
	//     "       gpio load spi/i2c\n"
	//     "       gpio unload spi/i2c\n"
	      "       gpio i2cd/i2cdetect port\n"
	      "       gpio serial port\n"
	      "       gpio rbx/rbd\n"
	      "       gpio wb <value>\n";
	//      "       gpio usbp high/low\n"
	//      "       gpio gbr <channel>\n"
	//      "       gpio gbw <channel> <value>" ;	// No trailing newline needed here.


#ifdef	NOT_FOR_NOW
/*
 * decodePin:
 *	Decode a pin "number" which can actually be a pin name to represent
 *	one of the Pi's on-board pins.
 *********************************************************************************
 */

static int decodePin (const char *str)
{

// The first case - see if it's a number:

  if (isdigit (str [0]))
    return atoi (str) ;

  return 0 ;
}
#endif


/*
 * findExecutable:
 *	Code to locate the path to the given executable. We have a fixed list
 *	of locations to try which completely overrides any $PATH environment.
 *	This may be detrimental, however it avoids the reliance on $PATH
 *	which may be a security issue when this program is run a set-uid-root.
 *********************************************************************************
 */

static const char *searchPath [] =
{
  "/sbin",
  "/usr/sbin",
  "/bin",
  "/usr/bin",
  NULL,
} ;

static char *findExecutable (const char *progName)
{
  static char *path = NULL ;
  int len = strlen (progName) ;
  int i = 0 ;
  struct stat statBuf ;

  for (i = 0 ; searchPath [i] != NULL ; ++i)
  {
    path = malloc (strlen (searchPath [i]) + len + 2) ;
    sprintf (path, "%s/%s", searchPath [i], progName) ;

    if (stat (path, &statBuf) == 0)
      return path ;
    free (path) ;
  }

  return NULL ;
}


/*
 * changeOwner:
 *	Change the ownership of the file to the real userId of the calling
 *	program so we can access it.
 *********************************************************************************
 */

static void changeOwner (char *cmd, char *file)
{
  uid_t uid = getuid () ;
  uid_t gid = getgid () ;

  if (chown (file, uid, gid) != 0)
  {

// Removed (ignoring) the check for not existing as I'm fed-up with morons telling me that
//	the warning message is an error.

    if (errno != ENOENT)
      fprintf (stderr, "%s: Unable to change ownership of %s: %s\n", cmd, file, strerror (errno)) ;
  }
}


/*
 * moduleLoaded:
 *	Return true/false if the supplied module is loaded
 *********************************************************************************
 */

static int moduleLoaded (char *modName)
{
  int len   = strlen (modName) ;
  int found = FALSE ;
  FILE *fd = fopen ("/proc/modules", "r") ;
  char line [80] ;

  if (fd == NULL)
  {
    fprintf (stderr, "gpio: Unable to check /proc/modules: %s\n", strerror (errno)) ;
    exit (1) ;
  }

  while (fgets (line, 80, fd) != NULL)
  {
    if (strncmp (line, modName, len) != 0)
      continue ;

    found = TRUE ;
    break ;
  }

  fclose (fd) ;

  return found ;
}


/*
 * doLoad:
 *	Load either the spi or i2c modules and change device ownerships, etc.
 *********************************************************************************
 */

static void checkDevTree (char *argv [])
{
  struct stat statBuf ;

  if (stat ("/proc/device-tree", &statBuf) == 0)	// We're on a devtree system ...
  {
    fprintf (stderr,
"%s: Unable to load/unload modules as this Pi has the device tree enabled.\n"
"  You need to run the raspi-config program (as root) and select the\n"
"  modules (SPI or I2C) that you wish to load/unload there and reboot.\n", argv [0]) ;
    exit (1) ;
  }
}

static void _doLoadUsage (char *argv [])
{
  fprintf (stderr, "Usage: %s load <spi/i2c> [I2C baudrate in Kb/sec]\n", argv [0]) ;
  exit (1) ;
}

static void doLoad (int argc, char *argv [])
{
  char *module1, *module2 ;
  char cmd [80] ;
  char *file1, *file2 ;
  char args1 [32], args2 [32] ;

  checkDevTree (argv) ;

  if (argc < 3)
    _doLoadUsage (argv) ;

  args1 [0] = args2 [0] = 0 ;

  /**/ if (strcasecmp (argv [2], "spi") == 0)
  {
    module1 = "spidev";
    module2 = "spi_bcm2708";
    file1  = "/dev/spidev0.0";
    file2  = "/dev/spidev0.1";
    if (argc == 4)
    {
      fprintf (stderr, "%s: Unable to set the buffer size now. Load aborted. Please see the man page.\n", argv [0]) ;
      exit (1) ;
    }
    else if (argc > 4)
      _doLoadUsage (argv) ;
  }
  else if (strcasecmp (argv [2], "i2c") == 0)
  {
    module1 = "i2c_dev" ;
    module2 = "i2c_bcm2708" ;
    file1  = "/dev/i2c-0" ;
    file2  = "/dev/i2c-1" ;
    if (argc == 4)
      sprintf (args2, " baudrate=%d", atoi (argv [3]) * 1000) ;
    else if (argc > 4)
      _doLoadUsage (argv) ;
  }
  else
    _doLoadUsage (argv) ;

  if (findExecutable ("modprobe") == NULL)
    printf ("No found\n") ;

  if (!moduleLoaded (module1))
  {
    sprintf (cmd, "%s %s%s", findExecutable (MODPROBE), module1, args1) ;
    if ( system (cmd) ){;}
  }

  if (!moduleLoaded (module2))
  {
    sprintf (cmd, "%s %s%s", findExecutable (MODPROBE), module2, args2) ;
    if ( system (cmd) ){;}
  }

  if (!moduleLoaded (module2))
  {
    fprintf (stderr, "%s: Unable to load %s\n", argv [0], module2) ;
    exit (1) ;
  }

  sleep (1) ;	// To let things get settled

  changeOwner (argv [0], file1) ;
  changeOwner (argv [0], file2) ;
}


/*
 * doUnLoad:
 *	Un-Load either the spi or i2c modules and change device ownerships, etc.
 *********************************************************************************
 */

static void _doUnLoadUsage (char *argv [])
{
  fprintf (stderr, "Usage: %s unload <spi/i2c>\n", argv [0]) ;
  exit (1) ;
}

static void doUnLoad (int argc, char *argv [])
{
  char *module1, *module2 ;
  char cmd [80] ;

  checkDevTree (argv) ;

  if (argc != 3)
    _doUnLoadUsage (argv) ;

  /**/ if (strcasecmp (argv [2], "spi") == 0)
  {
    module1 = "spidev" ;
    module2 = "spi_bcm2708" ;
  }
  else if (strcasecmp (argv [2], "i2c") == 0)
  {
    module1 = "i2c_dev" ;
    module2 = "i2c_bcm2708" ;
  }
  else
    _doUnLoadUsage (argv) ;

  if (moduleLoaded (module1))
  {
    sprintf (cmd, "%s %s", findExecutable (RMMOD), module1) ;
    if ( system (cmd) ){;}
  }

  if (moduleLoaded (module2))
  {
    sprintf (cmd, "%s %s", findExecutable (RMMOD), module2) ;
    if ( system (cmd) ){;}
  }
}


/*
 * doI2Cdetect:
 *	Run the i2cdetect command with the right runes for this Pi revision
 *********************************************************************************
 */

static void doI2Cdetect (UNU int argc, char *argv [])
{
//  int port = piGpioLayout () == 1 ? 0 : 1 ;
  int port = 0;
  char *c, *command ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s i2cd/i2cdetect port\n", argv [0]) ;
    exit (1) ;
  }
  
  port = atoi (argv [2]);

  if ((c = findExecutable (I2CDETECT)) == NULL)
  {
    fprintf (stderr, "%s: Unable to find i2cdetect command: %s\n", argv [0], strerror (errno)) ;
    return ;
  }

#ifndef CONFIG_ORANGEPI
  if (!moduleLoaded ("i2c_dev"))
  {
    fprintf (stderr, "%s: The I2C kernel module(s) are not loaded.\n", argv [0]) ;
    return ;
  }
#endif

  command = malloc (strlen (c) + 16) ;
  sprintf (command, "%s -y %d", c, port) ;
  if (system (command) < 0)
    fprintf (stderr, "%s: Unable to run i2cdetect: %s\n", argv [0], strerror (errno)) ;

}


/*
 * doSerialTest:
 *	Very simple program to test the serial port. Expects
 *	the port to be looped back to itself
 *********************************************************************************
 */

static void doSerialTest (UNU int argc, char *argv [])
{
	int fd ;
	int count ;
	unsigned int nextTime;
	char * port;

	port = argv [2];

	if (argc != 3) {
		fprintf (stderr, "Usage: %s serial port(/dev/ttySX)\n", argv [0]) ;
		exit (1) ;
	}

	if ((fd = serialOpen (port, 115200)) < 0)	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return;
	}

	if (wiringPiSetup () == -1)	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return;
	}

	nextTime = millis () + 300 ;

	for (count = 0 ; count < 256 ; ) {
		if (millis () > nextTime) {
			printf ("\nOut: %3d: ", count) ;
			fflush (stdout) ;
			serialPutchar (fd, count) ;
			nextTime += 300 ;
			++count ;
		}

		delay (3) ;

		while (serialDataAvail (fd)) {
			printf (" -> %3d", serialGetchar (fd)) ;
			fflush (stdout) ;
		}
	}

	printf ("\n") ;
	return;
}


/*
 * doExports:
 *	List all GPIO exports
 *********************************************************************************
 */

static void doExports (UNU int argc, UNU char *argv [])
{
  int fd ;
  int i = 0, j = 0, l, first ;
  char fName [128] ;
  char buf [16] ;

#ifdef CONFIG_ORANGEPI
  for (first = 0, j = 0 ; j < 64 ; ++j)   // Crude, but effective
#else
  for (first = 0, i = 0 ; i < 64 ; ++i)	// Crude, but effective
#endif
  {
 
// Try to read the direction
#ifdef CONFIG_ORANGEPI
	i = pinToGpioOrangePi[j];
#ifdef CONFIG_ORANGEPI_RK3399
	i += 1000;
#endif

#endif
	
    sprintf (fName, "/sys/class/gpio/gpio%d/direction", i) ;
    if ((fd = open (fName, O_RDONLY)) == -1)
    	continue ;

    if (first == 0)
    {
      ++first ;
      printf ("GPIO Pins exported:\n") ;
    }

    printf ("%4d(%d): ", j, i) ;

    if ((l = read (fd, buf, 16)) == 0)
      sprintf (buf, "%s", "?") ;
 
    buf [l] = 0 ;
    if ((buf [strlen (buf) - 1]) == '\n')
      buf [strlen (buf) - 1] = 0 ;

    printf ("%-3s", buf) ;

    close (fd) ;

// Try to Read the value

    sprintf (fName, "/sys/class/gpio/gpio%d/value", i) ;
    if ((fd = open (fName, O_RDONLY)) == -1)
    {
      printf ("No Value file (huh?)\n") ;
      continue ;
    }

    if ((l = read (fd, buf, 16)) == 0)
      sprintf (buf, "%s", "?") ;

    buf [l] = 0 ;
    if ((buf [strlen (buf) - 1]) == '\n')
      buf [strlen (buf) - 1] = 0 ;

    printf ("  %s", buf) ;

// Read any edge trigger file

    sprintf (fName, "/sys/class/gpio/gpio%d/edge", i) ;
    if ((fd = open (fName, O_RDONLY)) == -1)
    {
      printf ("\n") ;
      continue ;
    }

    if ((l = read (fd, buf, 16)) == 0)
      sprintf (buf, "%s", "?") ;

    buf [l] = 0 ;
    if ((buf [strlen (buf) - 1]) == '\n')
      buf [strlen (buf) - 1] = 0 ;

    printf ("  %-8s\n", buf) ;

    close (fd) ;
  }
}


/*
 * doExport:
 *	gpio export pin mode
 *	This uses the /sys/class/gpio device interface.
 *********************************************************************************
 */

void doExport (int argc, char *argv [])
{
  FILE *fd ;
  int pin ;
  char *mode ;
  char fName [128] ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s export pin mode\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;
#ifdef CONFIG_ORANGEPI
  pin = pinToGpioOrangePi[pin];
#ifdef CONFIG_ORANGEPI_RK3399
  pin += 1000;
#endif

#endif

  mode = argv [3] ;

  if ((fd = fopen ("/sys/class/gpio/export", "w")) == NULL)
  {
    fprintf (stderr, "%s: Unable to open GPIO export interface: %s\n", argv [0], strerror (errno)) ;
    exit (1) ;
  }

  fprintf (fd, "%d\n", pin) ;
  fclose (fd) ;

  sprintf (fName, "/sys/class/gpio/gpio%d/direction", pin) ;
  if ((fd = fopen (fName, "w")) == NULL)
  {
    fprintf (stderr, "%s: Unable to open GPIO direction interface for pin %d: %s\n", argv [0], pin, strerror (errno)) ;
    exit (1) ;
  }

  /**/ if ((strcasecmp (mode, "in")   == 0) || (strcasecmp (mode, "input")  == 0))
    fprintf (fd, "in\n") ;
  else if ((strcasecmp (mode, "out")  == 0) || (strcasecmp (mode, "output") == 0))
    fprintf (fd, "out\n") ;
  else if ((strcasecmp (mode, "high") == 0) || (strcasecmp (mode, "up")     == 0))
    fprintf (fd, "high\n") ;
  else if ((strcasecmp (mode, "low")  == 0) || (strcasecmp (mode, "down")   == 0))
    fprintf (fd, "low\n") ;
  else
  {
    fprintf (stderr, "%s: Invalid mode: %s. Should be in, out, high or low\n", argv [1], mode) ;
    exit (1) ;
  }

  fclose (fd) ;

// Change ownership so the current user can actually use it

  sprintf (fName, "/sys/class/gpio/gpio%d/value", pin) ;
  changeOwner (argv [0], fName) ;

  sprintf (fName, "/sys/class/gpio/gpio%d/edge", pin) ;
  changeOwner (argv [0], fName) ;

}


/*
 * doWfi:
 *	gpio wfi pin mode
 *	Wait for Interrupt on a given pin.
 *	Slight cheat here - it's easier to actually use ISR now (which calls
 *	gpio to set the pin modes!) then we simply sleep, and expect the thread
 *	to exit the program. Crude but effective.
 *********************************************************************************
 */

static void wfi (void)
  { exit (0) ; }

void doWfi (int argc, char *argv [])
{
  int pin, mode ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s wfi pin mode\n", argv [0]) ;
    exit (1) ;
  }

  pin  = atoi (argv [2]) ;

  /**/ if (strcasecmp (argv [3], "rising")  == 0) mode = INT_EDGE_RISING ;
  else if (strcasecmp (argv [3], "falling") == 0) mode = INT_EDGE_FALLING ;
  else if (strcasecmp (argv [3], "both")    == 0) mode = INT_EDGE_BOTH ;
  else
  {
    fprintf (stderr, "%s: wfi: Invalid mode: %s. Should be rising, falling or both\n", argv [1], argv [3]) ;
    exit (1) ;
  }

  if (wiringPiISR (pin, mode, &wfi) < 0)
  {
    fprintf (stderr, "%s: wfi: Unable to setup ISR: %s\n", argv [1], strerror (errno)) ;
    exit (1) ;
  }

  for (;;)
    delay (9999) ;
}



/*
 * doEdge:
 *	gpio edge pin mode
 *	Easy access to changing the edge trigger on a GPIO pin
 *	This uses the /sys/class/gpio device interface.
 *********************************************************************************
 */

void doEdge (int argc, char *argv [])
{
  FILE *fd ;
  int pin ;
  char *mode ;
  char fName [128] ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s edge pin mode\n", argv [0]) ;
    exit (1) ;
  }

  pin  = atoi (argv [2]) ;
#ifdef CONFIG_ORANGEPI
  pin = pinToGpioOrangePi[pin];
#ifdef CONFIG_ORANGEPI_RK3399
	pin += 1000;
#endif

#endif
  mode = argv [3] ;

// Export the pin and set direction to input

  if ((fd = fopen ("/sys/class/gpio/export", "w")) == NULL)
  {
    fprintf (stderr, "%s: Unable to open GPIO export interface: %s\n", argv [0], strerror (errno)) ;
    exit (1) ;
  }

  fprintf (fd, "%d\n", pin) ;
  fclose (fd) ;

  sprintf (fName, "/sys/class/gpio/gpio%d/direction", pin) ;
  if ((fd = fopen (fName, "w")) == NULL)
  {
    fprintf (stderr, "%s: Unable to open GPIO direction interface for pin %d: %s\n", argv [0], pin, strerror (errno)) ;
    exit (1) ;
  }

  fprintf (fd, "in\n") ;
  fclose (fd) ;

  sprintf (fName, "/sys/class/gpio/gpio%d/edge", pin) ;
  if ((fd = fopen (fName, "w")) == NULL)
  {
    fprintf (stderr, "%s: Unable to open GPIO edge interface for pin %d: %s\n", argv [0], pin, strerror (errno)) ;
    exit (1) ;
  }

  /**/ if (strcasecmp (mode, "none")    == 0) fprintf (fd, "none\n") ;
  else if (strcasecmp (mode, "rising")  == 0) fprintf (fd, "rising\n") ;
  else if (strcasecmp (mode, "falling") == 0) fprintf (fd, "falling\n") ;
  else if (strcasecmp (mode, "both")    == 0) fprintf (fd, "both\n") ;
  else
  {
    fprintf (stderr, "%s: Invalid mode: %s. Should be none, rising, falling or both\n", argv [1], mode) ;
    exit (1) ;
  }

// Change ownership of the value and edge files, so the current user can actually use it!

  sprintf (fName, "/sys/class/gpio/gpio%d/value", pin) ;
  changeOwner (argv [0], fName) ;

  sprintf (fName, "/sys/class/gpio/gpio%d/edge", pin) ;
  changeOwner (argv [0], fName) ;

  fclose (fd) ;
}


/*
 * doUnexport:
 *	gpio unexport pin
 *	This uses the /sys/class/gpio device interface.
 *********************************************************************************
 */

void doUnexport (int argc, char *argv [])
{
  FILE *fd ;
  int pin ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s unexport pin\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;
#ifdef CONFIG_ORANGEPI
	pin = pinToGpioOrangePi[pin];
#ifdef CONFIG_ORANGEPI_RK3399
   pin += 1000;
#endif

#endif

  if ((fd = fopen ("/sys/class/gpio/unexport", "w")) == NULL)
  {
    fprintf (stderr, "%s: Unable to open GPIO export interface\n", argv [0]) ;
    exit (1) ;
  }

  fprintf (fd, "%d\n", pin) ;
  fclose (fd) ;
}


/*
 * doUnexportAll:
 *	gpio unexportall
 *	Un-Export all the GPIO pins.
 *	This uses the /sys/class/gpio device interface.
 *********************************************************************************
 */

void doUnexportall (char *progName)
{
  FILE *fd ;
  int pin ;
  int i = 0;

  for (pin = 0 ; pin < 63 ; ++pin)
  {
#ifdef CONFIG_ORANGEPI
  	i = pinToGpioOrangePi[pin];
#ifdef CONFIG_ORANGEPI_RK3399
	i += 1000;
#endif

#endif
    if ((fd = fopen ("/sys/class/gpio/unexport", "w")) == NULL)
    {
      fprintf (stderr, "%s: Unable to open GPIO export interface\n", progName) ;
      exit (1) ;
    }
    fprintf (fd, "%d\n", i) ;
    fclose (fd) ;
  }
}


/*
 * doReset:
 *	Reset the GPIO pins - as much as we can do
 *********************************************************************************
 */

static void doReset (UNU char *progName)
{
  printf ("GPIO Reset is dangerous and has been removed from the gpio command.\n") ;
  printf (" - Please write a shell-script to reset the GPIO pins into the state\n") ;
  printf ("   that you need them in for your applications.\n") ;
}


/*
 * doMode:
 *	gpio mode pin mode ...
 *********************************************************************************
 */

void doMode (int argc, char *argv [])
{
  int pin ;
  char *mode ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s mode pin mode\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;

  mode = argv [3] ;

  /**/ if (strcasecmp (mode, "in")      == 0) pinMode         (pin, INPUT) ;
  else if (strcasecmp (mode, "input")   == 0) pinMode         (pin, INPUT) ;
  else if (strcasecmp (mode, "out")     == 0) pinMode         (pin, OUTPUT) ;
  else if (strcasecmp (mode, "output")  == 0) pinMode         (pin, OUTPUT) ;
  else if (strcasecmp (mode, "pwm")     == 0) pinMode         (pin, PWM_OUTPUT) ;
  else if (strcasecmp (mode, "pwmTone") == 0) pinMode         (pin, PWM_TONE_OUTPUT) ;
  else if (strcasecmp (mode, "clock")   == 0) pinMode         (pin, GPIO_CLOCK) ;
  else if (strcasecmp (mode, "up")      == 0) pullUpDnControl (pin, PUD_UP) ;
  else if (strcasecmp (mode, "down")    == 0) pullUpDnControl (pin, PUD_DOWN) ;
  else if (strcasecmp (mode, "tri")     == 0) pullUpDnControl (pin, PUD_OFF) ;
  else if (strcasecmp (mode, "off")     == 0) pullUpDnControl (pin, PUD_OFF) ;
  else if (strcasecmp (mode, "alt2")    == 0) pinModeAlt (pin, 0b010) ;
  else if (strcasecmp (mode, "alt3")    == 0) pinModeAlt (pin, 0b011) ;
  else if (strcasecmp (mode, "alt4")    == 0) pinModeAlt (pin, 0b100) ;
  else if (strcasecmp (mode, "alt5")    == 0) pinModeAlt (pin, 0b101) ;
  else if (strcasecmp (mode, "alt6")    == 0) pinModeAlt (pin, 0b110) ;
  else if (strcasecmp (mode, "alt7")    == 0) pinModeAlt (pin, 0b111) ;
  else
  {
    fprintf (stderr, "%s: Invalid mode: %s. Should be in/out/pwm/clock/up/down/tri\n", argv [1], mode) ;
    exit (1) ;
  }
}


/*
 * doPadDrive:
 *	gpio drive group value
 *********************************************************************************
 */

static void doPadDrive (int argc, char *argv [])
{
  int group, val ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s drive group value\n", argv [0]) ;
    exit (1) ;
  }

  group = atoi (argv [2]) ;
  val   = atoi (argv [3]) ;

  if ((group < 0) || (group > 2))
  {
    fprintf (stderr, "%s: drive group not 0, 1 or 2: %d\n", argv [0], group) ;
    exit (1) ;
  }

  if ((val < 0) || (val > 7))
  {
    fprintf (stderr, "%s: drive value not 0-7: %d\n", argv [0], val) ;
    exit (1) ;
  }

  setPadDrive (group, val) ;
}


/*
 * doUsbP:
 *	Control USB Power - High (1.2A) or Low (600mA)
 *	gpio usbp high/low
 *********************************************************************************
 */

static void doUsbP (int argc, char *argv [])
{
	return;
}


/*
 * doGbw:
 *	gpio gbw channel value
 *	Gertboard Write - To the Analog output
 *********************************************************************************
 */

static void doGbw (int argc, char *argv [])
{
  int channel, value ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s gbw <channel> <value>\n", argv [0]) ;
    exit (1) ;
  }

  channel = atoi (argv [2]) ;
  value   = atoi (argv [3]) ;

  if ((channel < 0) || (channel > 1))
  {
    fprintf (stderr, "%s: gbw: Channel number must be 0 or 1\n", argv [0]) ;
    exit (1) ;
  }

  if ((value < 0) || (value > 255))
  {
    fprintf (stderr, "%s: gbw: Value must be from 0 to 255\n", argv [0]) ;
    exit (1) ;
  }

  if (gertboardAnalogSetup (64) < 0)
  {
    fprintf (stderr, "Unable to initialise the Gertboard SPI interface: %s\n", strerror (errno)) ;
    exit (1) ;
  }

  analogWrite (64 + channel, value) ;
}


/*
 * doGbr:
 *	gpio gbr channel
 *	From the analog input
 *********************************************************************************
 */

static void doGbr (int argc, char *argv [])
{
  int channel ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s gbr <channel>\n", argv [0]) ;
    exit (1) ;
  }

  channel = atoi (argv [2]) ;

  if ((channel < 0) || (channel > 1))
  {
    fprintf (stderr, "%s: gbr: Channel number must be 0 or 1\n", argv [0]) ;
    exit (1) ;
  }

  if (gertboardAnalogSetup (64) < 0)
  {
    fprintf (stderr, "Unable to initialise the Gertboard SPI interface: %s\n", strerror (errno)) ;
    exit (1) ;
  }

  printf ("%d\n", analogRead (64 + channel)) ;
}


/*
 * doWrite:
 *	gpio write pin value
 *********************************************************************************
 */

static void doWrite (int argc, char *argv [])
{
	int pin, val ;

	if (argc != 4)
	{
		fprintf (stderr, "Usage: %s write pin value\n", argv [0]) ;
		exit (1) ;
	}

	pin = atoi (argv [2]) ;

	if ((strcasecmp (argv [3], "up") == 0) || (strcasecmp (argv [3], "on") == 0))
		val = 1 ;
	else if ((strcasecmp (argv [3], "down") == 0) || (strcasecmp (argv [3], "off") == 0))
		val = 0 ;
	else
		val = atoi (argv [3]) ;

	if (val == 0)
		digitalWrite (pin, LOW) ;
	else
		digitalWrite (pin, HIGH) ;
}


/*
 * doAwriterite:
 *	gpio awrite pin value
 *********************************************************************************
 */

static void doAwrite (int argc, char *argv [])
{
  int pin, val ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s awrite pin value\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;

  val = atoi (argv [3]) ;

  analogWrite (pin, val) ;
}


/*
 * doWriteByte:
 *	gpio wb value
 *********************************************************************************
 */

static void doWriteByte (int argc, char *argv [])
{
  int val ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s wb value\n", argv [0]) ;
    exit (1) ;
  }

  val = (int)strtol (argv [2], NULL, 0) ;

  digitalWriteByte (val) ;
}


/*
 * doReadByte:
 *	gpio rbx|rbd value
 *********************************************************************************
 */

static void doReadByte (int argc, char *argv [], int printHex)
{
  int val ;

  if (argc != 2)
  {
    fprintf (stderr, "Usage: %s rbx|rbd\n", argv [0]) ;
    exit (1) ;
  }

  val = digitalReadByte () ;
  if (printHex)
    printf ("%02X\n", val) ;
  else
    printf ("%d\n", val) ;
}


/*
 * doRead:
 *	Read a pin and return the value
 *********************************************************************************
 */

void doRead (int argc, char *argv []) 
{
  int pin, val ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s read pin\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;
  val = digitalRead (pin) ;

  printf ("%s\n", val == 0 ? "0" : "1") ;
}


/*
 * doAread:
 *	Read an analog pin and return the value
 *********************************************************************************
 */

void doAread (int argc, char *argv []) 
{
  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s aread pin\n", argv [0]) ;
    exit (1) ;
  }

  printf ("%d\n", analogRead (atoi (argv [2]))) ;
}


/*
 * doToggle:
 *	Toggle an IO pin
 *********************************************************************************
 */

void doToggle (int argc, char *argv [])
{
  int pin ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s toggle pin\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;

  digitalWrite (pin, !digitalRead (pin)) ;
}


/*
 * doBlink:
 *	Blink an IO pin
 *********************************************************************************
 */

void doBlink (int argc, char *argv [])
{
  int pin ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s blink pin\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;

  pinMode (pin, OUTPUT) ;
  for (;;)
  {
    digitalWrite (pin, !digitalRead (pin)) ;
    delay (500) ;
  }

}


/*
 * doPwmTone:
 *	Output a tone in a PWM pin
 *********************************************************************************
 */

void doPwmTone (int argc, char *argv [])
{
  int pin, freq ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s pwmTone <pin> <freq>\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;
  freq = atoi (argv [3]) ;

  pwmToneWrite (pin, freq) ;
}


/*
 * doClock:
 *	Output a clock on a pin
 *********************************************************************************
 */

void doClock (int argc, char *argv [])
{
  int pin, freq ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s clock <pin> <freq>\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;

  freq = atoi (argv [3]) ;

  gpioClockSet (pin, freq) ;
}


/*
 * doPwm:
 *	Output a PWM value on a pin
 *********************************************************************************
 */

/*
 * doPwm:
 *	Output a PWM value on a pin
 *********************************************************************************
 */

void doPwm (int argc, char *argv [])
{
  int pin, val ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s pwm <pin> <value>\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;
  val = atoi (argv [3]) ;

  pinMode (pin, PWM_OUTPUT);
  pwmWrite (pin, val) ;
}


/*
 * doPwmMode: doPwmRange: doPwmClock:
 *	Change the PWM mode, range and clock divider values
 *********************************************************************************
 */

static void doPwmMode (int mode)
{
  pwmSetMode (mode) ;
}

static void doPwmRange (int argc, char *argv [])
{
  unsigned int range ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s pwmr <range>\n", argv [0]) ;
    exit (1) ;
  }

  range = (unsigned int)strtoul (argv [2], NULL, 10) ;

  if (range == 0)
  {
    fprintf (stderr, "%s: range must be > 0\n", argv [0]) ;
    exit (1) ;
  }

  pwmSetRange (range) ;
}

static void doPwmClock (int argc, char *argv [])
{
  unsigned int clock ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s pwmc <clock>\n", argv [0]) ;
    exit (1) ;
  }

  clock = (unsigned int)strtoul (argv [2], NULL, 10) ;

  if ((clock < 1) || (clock > 4095))
  {
    fprintf (stderr, "%s: clock must be between 0 and 4096\n", argv [0]) ;
    exit (1) ;
  }

  pwmSetClock (clock) ;
}



/*
 * doVersion:
 *	Handle the ever more complicated version command and print out
 *	some usefull information.
 *********************************************************************************
 */

static void doVersion (char *argv [])
{
  struct stat statBuf ;
  char name [80] ;
  FILE *fd ;

  int vMaj, vMin ;

  wiringPiVersion (&vMaj, &vMin) ;
  printf ("gpio version: %d.%d\n", vMaj, vMin) ;
  printf ("Copyright (c) 2012-2018 Gordon Henderson\n") ;
  printf ("This is free software with ABSOLUTELY NO WARRANTY.\n") ;
  printf ("For details type: %s -warranty\n", argv [0]) ;
  printf ("\n") ;
//  piBoardId (&model, &rev, &mem, &maker, &warranty) ;

//  printf ("Raspberry Pi Details:\n") ;
//  printf ("  Type: %s, Revision: %s, Memory: %dMB, Maker: %s %s\n", 
//      piModelNames [model], piRevisionNames [rev], piMemorySize [mem], piMakerNames [maker], warranty ? "[Out of Warranty]" : "") ;

// Check for device tree

  if (stat ("/proc/device-tree", &statBuf) == 0)	// We're on a devtree system ...
    printf ("  * Device tree is enabled.\n") ;

  if (stat ("/proc/device-tree/model", &statBuf) == 0)	// Output Kernel idea of board type
  {
    if ((fd = fopen ("/proc/device-tree/model", "r")) != NULL)
    {
      if ( fgets (name, 80, fd) ){;}
      fclose (fd) ;
      printf ("  *--> %s\n", name) ;
    }
  }

//  if (stat ("/dev/gpiomem", &statBuf) == 0)		// User level GPIO is GO
//    printf ("  * This Raspberry Pi supports user-level GPIO access.\n") ;
//  else
//    printf ("  * Root or sudo required for GPIO access.\n") ;
}


/*
 * main:
 *	Start here
 *********************************************************************************
 */
int main (int argc, char *argv [])
{
  int i ;

  if (getenv ("WIRINGPI_DEBUG") != NULL)
  {
    printf ("gpio: wiringPi debug mode enabled\n") ;
    wiringPiDebug = TRUE ;
  }

  if (argc == 1)
  {
    fprintf (stderr,
"%s: At your service!\n"
"  Type: gpio -h for full details and\n"
"        gpio readall for a quick printout of your connector details\n", argv [0]) ;
    exit (EXIT_FAILURE) ;
  }

// Help

  if (strcasecmp (argv [1], "-h") == 0)
  {
    printf ("%s: %s\n", argv [0], usage) ;
    exit (EXIT_SUCCESS) ;
  }

// Version & Warranty
//	Wish I could remember why I have both -R and -V ...

  if ((strcmp (argv [1], "-R") == 0) || (strcmp (argv [1], "-V") == 0))
  {
    printf ("wiringPi for Orange Pi\n") ;
    exit (EXIT_SUCCESS) ;
  }

// Version and information

  if (strcmp (argv [1], "-v") == 0)
  {
    doVersion (argv) ;
    exit (EXIT_SUCCESS) ;
  }

  if (strcasecmp (argv [1], "-warranty") == 0)
  {
    printf ("gpio version: %s\n", VERSION) ;
    printf ("Copyright (c) 2012-2018 Gordon Henderson\n") ;
    printf ("\n") ;
    printf ("    This program is free software; you can redistribute it and/or modify\n") ;
    printf ("    it under the terms of the GNU Leser General Public License as published\n") ;
    printf ("    by the Free Software Foundation, either version 3 of the License, or\n") ;
    printf ("    (at your option) any later version.\n") ;
    printf ("\n") ;
    printf ("    This program is distributed in the hope that it will be useful,\n") ;
    printf ("    but WITHOUT ANY WARRANTY; without even the implied warranty of\n") ;
    printf ("    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n") ;
    printf ("    GNU Lesser General Public License for more details.\n") ;
    printf ("\n") ;
    printf ("    You should have received a copy of the GNU Lesser General Public License\n") ;
    printf ("    along with this program. If not, see <http://www.gnu.org/licenses/>.\n") ;
    printf ("\n") ;
    exit (EXIT_SUCCESS) ;
  }

  if (geteuid () != 0)
  {
    fprintf (stderr, "%s: Must be root to run. Program should be suid root. This is an error.\n", argv [0]) ;
    exit (EXIT_FAILURE) ;
  }

// Initial test for /sys/class/gpio operations:

  /**/ if (strcasecmp (argv [1], "exports"    ) == 0)	{ doExports     (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [1], "export"     ) == 0)	{ doExport      (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [1], "edge"       ) == 0)	{ doEdge        (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [1], "unexport"   ) == 0)	{ doUnexport    (argc, argv) ;	return 0 ; }
  else if (strcasecmp (argv [1], "unexportall") == 0)	{ doUnexportall (argv [0]) ;	return 0 ; }

// Check for load command:

  if (strcasecmp (argv [1], "load"   ) == 0)	{ doLoad   (argc, argv) ; return 0 ; }
  if (strcasecmp (argv [1], "unload" ) == 0)	{ doUnLoad (argc, argv) ; return 0 ; }

// Check for usb power command

  if (strcasecmp (argv [1], "usbp"   ) == 0)	{ doUsbP   (argc, argv) ; return 0 ; }

// Gertboard commands

  if (strcasecmp (argv [1], "gbr" ) == 0)	{ doGbr (argc, argv) ; return 0 ; }
  if (strcasecmp (argv [1], "gbw" ) == 0)	{ doGbw (argc, argv) ; return 0 ; }

// Check for allreadall command, force Gpio mode

  if (strcasecmp (argv [1], "allreadall") == 0)
  {
    wiringPiSetupGpio () ;
    doAllReadall      () ;
    return 0 ;
  }

// Check for -g argument

  /**/ if (strcasecmp (argv [1], "-g") == 0)
  {
    wiringPiSetupGpio () ;

    for (i = 2 ; i < argc ; ++i)
      argv [i - 1] = argv [i] ;
    --argc ;
    wpMode = WPI_MODE_GPIO ;
  }

// Check for -1 argument

  else if (strcasecmp (argv [1], "-1") == 0)
  {
    wiringPiSetupPhys () ;

    for (i = 2 ; i < argc ; ++i)
      argv [i - 1] = argv [i] ;
    --argc ;
    wpMode = WPI_MODE_PHYS ;
  }

// Check for -p argument for PiFace

  else if (strcasecmp (argv [1], "-p") == 0)
  {
    piFaceSetup (200) ;

    for (i = 2 ; i < argc ; ++i)
      argv [i - 1] = argv [i] ;
    --argc ;
    wpMode = WPI_MODE_PIFACE ;
  }

// Check for -z argument so we don't actually initialise wiringPi

  else if (strcasecmp (argv [1], "-z") == 0)
  {
    for (i = 2 ; i < argc ; ++i)
      argv [i - 1] = argv [i] ;
    --argc ;
    wpMode = WPI_MODE_UNINITIALISED ;
  }

// Default to wiringPi mode

  else
  {
    wiringPiSetup () ;
    wpMode = WPI_MODE_PINS ;
  }

// Check for -x argument to load in a new extension
//	-x extension:base:args
//	Can load many modules, but unless daemon mode we can only send one
//	command at a time.

  while (strcasecmp (argv [1], "-x") == 0)
  {
    if (argc < 3)
    {
      fprintf (stderr, "%s: -x missing extension command.\n", argv [0]) ;
      exit (EXIT_FAILURE) ;
    }

    if (!loadWPiExtension (argv [0], argv [2], TRUE))
    {
      fprintf (stderr, "%s: Extension load failed: %s\n", argv [0], strerror (errno)) ;
      exit (EXIT_FAILURE) ;
    }

// Shift args down by 2

    for (i = 3 ; i < argc ; ++i)
      argv [i - 2] = argv [i] ;
    argc -= 2 ;
  }

  if (argc <= 1)
  {
    fprintf (stderr, "%s: no command given\n", argv [0]) ;
    exit (EXIT_FAILURE) ;
  }

// Core wiringPi functions

  /**/ if (strcasecmp (argv [1], "mode"   ) == 0) doMode      (argc, argv) ;
  else if (strcasecmp (argv [1], "read"   ) == 0) doRead      (argc, argv) ;
  else if (strcasecmp (argv [1], "write"  ) == 0) doWrite     (argc, argv) ;
  else if (strcasecmp (argv [1], "pwm"    ) == 0) doPwm       (argc, argv) ;
  else if (strcasecmp (argv [1], "awrite" ) == 0) doAwrite    (argc, argv) ;
  else if (strcasecmp (argv [1], "aread"  ) == 0) doAread     (argc, argv) ;

// GPIO Nicies

  else if (strcasecmp (argv [1], "toggle" ) == 0) doToggle    (argc, argv) ;
  else if (strcasecmp (argv [1], "blink"  ) == 0) doBlink     (argc, argv) ;

// Pi Specifics

  else if (strcasecmp (argv [1], "pwm-bal"  ) == 0) doPwmMode    (PWM_MODE_BAL) ;
  else if (strcasecmp (argv [1], "pwm-ms"   ) == 0) doPwmMode    (PWM_MODE_MS) ;
  else if (strcasecmp (argv [1], "pwmr"     ) == 0) doPwmRange   (argc, argv) ;
  else if (strcasecmp (argv [1], "pwmc"     ) == 0) doPwmClock   (argc, argv) ;
  else if (strcasecmp (argv [1], "pwmTone"  ) == 0) doPwmTone    (argc, argv) ;
  else if (strcasecmp (argv [1], "drive"    ) == 0) doPadDrive   (argc, argv) ;
  else if (strcasecmp (argv [1], "readall"  ) == 0) doReadall    () ;
  else if (strcasecmp (argv [1], "nreadall" ) == 0) doReadall    () ;
  else if (strcasecmp (argv [1], "pins"     ) == 0) doReadall    () ;
  else if (strcasecmp (argv [1], "qmode"    ) == 0) doQmode      (argc, argv) ;
  else if (strcasecmp (argv [1], "i2cdetect") == 0) doI2Cdetect  (argc, argv) ;
  else if (strcasecmp (argv [1], "i2cd"     ) == 0) doI2Cdetect  (argc, argv) ;
  else if (strcasecmp (argv [1], "serial"     ) == 0) doSerialTest  (argc, argv) ;
  else if (strcasecmp (argv [1], "reset"    ) == 0) doReset      (argv [0]) ;
  else if (strcasecmp (argv [1], "wb"       ) == 0) doWriteByte  (argc, argv) ;
  else if (strcasecmp (argv [1], "rbx"      ) == 0) doReadByte   (argc, argv, TRUE) ;
  else if (strcasecmp (argv [1], "rbd"      ) == 0) doReadByte   (argc, argv, FALSE) ;
  else if (strcasecmp (argv [1], "clock"    ) == 0) doClock      (argc, argv) ;
  else if (strcasecmp (argv [1], "wfi"      ) == 0) doWfi        (argc, argv) ;
  else
  {
    fprintf (stderr, "%s: Unknown command: %s.\n", argv [0], argv [1]) ;
    exit (EXIT_FAILURE) ;
  }

  return 0 ;
}
