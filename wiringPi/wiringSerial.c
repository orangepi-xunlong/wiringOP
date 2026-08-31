/*
 * wiringSerial.c:
 *	Handle a serial port
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
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <poll.h>
#include <sys/file.h>

#ifdef __linux__
# include <linux/serial.h>
# include <linux/tty_flags.h>
# define termios linux_uapi_termios
# include <asm/termbits.h>
# undef termios
# include <asm/ioctls.h>
#endif

#include "wiringSerial.h"

/*
 * serialOpen:
 *	Open and initialise the serial port, setting all the right
 *	port parameters - or as many as are required - hopefully!
 *********************************************************************************
 */

int serialOpen (const char *device, const int baud)
{
  struct termios options ;
  speed_t myBaud ;
  int     status, fd ;

  switch (baud)
  {
    case      50:	myBaud =      B50 ; break ;
    case      75:	myBaud =      B75 ; break ;
    case     110:	myBaud =     B110 ; break ;
    case     134:	myBaud =     B134 ; break ;
    case     150:	myBaud =     B150 ; break ;
    case     200:	myBaud =     B200 ; break ;
    case     300:	myBaud =     B300 ; break ;
    case     600:	myBaud =     B600 ; break ;
    case    1200:	myBaud =    B1200 ; break ;
    case    1800:	myBaud =    B1800 ; break ;
    case    2400:	myBaud =    B2400 ; break ;
    case    4800:	myBaud =    B4800 ; break ;
    case    9600:	myBaud =    B9600 ; break ;
    case   19200:	myBaud =   B19200 ; break ;
    case   38400:	myBaud =   B38400 ; break ;
    case   57600:	myBaud =   B57600 ; break ;
    case  115200:	myBaud =  B115200 ; break ;
    case  230400:	myBaud =  B230400 ; break ;
    case  460800:	myBaud =  B460800 ; break ;
    case  500000:	myBaud =  B500000 ; break ;
    case  576000:	myBaud =  B576000 ; break ;
    case  921600:	myBaud =  B921600 ; break ;
    case 1000000:	myBaud = B1000000 ; break ;
    case 1152000:	myBaud = B1152000 ; break ;
    case 1500000:	myBaud = B1500000 ; break ;
    case 2000000:	myBaud = B2000000 ; break ;
    case 2500000:	myBaud = B2500000 ; break ;
    case 3000000:	myBaud = B3000000 ; break ;
    case 3500000:	myBaud = B3500000 ; break ;
    case 4000000:	myBaud = B4000000 ; break ;

    default:
      return -2 ;
  }

  if ((fd = open (device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
    return -1 ;

  fcntl (fd, F_SETFL, O_RDWR) ;

// Get and modify current options:

  tcgetattr (fd, &options) ;

    cfmakeraw   (&options) ;
    cfsetispeed (&options, myBaud) ;
    cfsetospeed (&options, myBaud) ;

    options.c_cflag |= (CLOCAL | CREAD) ;
    options.c_cflag &= ~PARENB ;
    options.c_cflag &= ~CSTOPB ;
    options.c_cflag &= ~CSIZE ;
    options.c_cflag |= CS8 ;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
    options.c_oflag &= ~OPOST ;

    options.c_cc [VMIN]  =   0 ;
    options.c_cc [VTIME] = 100 ;	// Ten seconds (100 deciseconds)

  tcsetattr (fd, TCSANOW, &options) ;

  ioctl (fd, TIOCMGET, &status);

  status |= TIOCM_DTR ;
  status |= TIOCM_RTS ;

  ioctl (fd, TIOCMSET, &status);

  usleep (10000) ;	// 10mS

  return fd ;
}


/*
 * serialFlush:
 *	Flush the serial buffers (both tx & rx)
 *********************************************************************************
 */

void serialFlush (const int fd)
{
  tcflush (fd, TCIOFLUSH) ;
}


/*
 * serialClose:
 *	Release the serial port
 *********************************************************************************
 */

void serialClose (const int fd)
{
  close (fd) ;
}


/*
 * serialPutchar:
 *	Send a single character to the serial port
 *********************************************************************************
 */

void serialPutchar (const int fd, const unsigned char c)
{
  int ret;
  ret = write (fd, &c, 1) ;
  if (ret < 0)
  	 printf("Serial Putchar Error\n");
}


/*
 * serialPuts:
 *	Send a string to the serial port
 *********************************************************************************
 */

void serialPuts (const int fd, const char *s)
{ 
	int ret;
	ret = write (fd, s, strlen (s));
	if (ret < 0)
		printf("Serial Puts Error\n");
}

/*
 * serialPrintf:
 *	Printf over Serial
 *********************************************************************************
 */

void serialPrintf (const int fd, const char *message, ...)
{
  va_list argp ;
  char buffer [1024] ;

  va_start (argp, message) ;
    vsnprintf (buffer, 1023, message, argp) ;
  va_end (argp) ;

  serialPuts (fd, buffer) ;
}


/*
 * serialDataAvail:
 *	Return the number of bytes of data avalable to be read in the serial port
 *********************************************************************************
 */

int serialDataAvail (const int fd)
{
  int result ;

  if (ioctl (fd, FIONREAD, &result) == -1)
    return -1 ;

  return result ;
}


/*
 * serialGetchar:
 *	Get a single character from the serial device.
 *	Note: Zero is a valid character and this function will time-out after
 *	10 seconds.
 *********************************************************************************
 */

int serialGetchar (const int fd)
{
  uint8_t x ;

  if (read (fd, &x, 1) != 1)
    return -1 ;

  return ((int)x) & 0xFF ;
}

/* ------------------------------------------------------------------------- */
/* Extended serial configuration and high-throughput I/O                     */
/* ------------------------------------------------------------------------- */

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif
#ifndef CMSPAR
# define CMSPAR 0
#endif
#ifndef CRTSCTS
# ifdef CNEW_RTSCTS
#  define CRTSCTS CNEW_RTSCTS
# else
#  define CRTSCTS 0
# endif
#endif

struct serialContext
{
  int fd ;
  int ownFd ;
  int cancelRead [2] ;
  int cancelWrite [2] ;
} ;

static int serialBadArgument (void)
{
  errno = EINVAL ;
  return -1 ;
}

static speed_t serialBaudConstant (const unsigned int baud)
{
  switch (baud)
  {
    case       0: return B0 ;
    case      50: return B50 ;
    case      75: return B75 ;
    case     110: return B110 ;
    case     134: return B134 ;
    case     150: return B150 ;
    case     200: return B200 ;
    case     300: return B300 ;
    case     600: return B600 ;
    case    1200: return B1200 ;
    case    1800: return B1800 ;
    case    2400: return B2400 ;
    case    4800: return B4800 ;
    case    9600: return B9600 ;
    case   19200: return B19200 ;
    case   38400: return B38400 ;
#ifdef B57600
    case   57600: return B57600 ;
#endif
#ifdef B115200
    case  115200: return B115200 ;
#endif
#ifdef B230400
    case  230400: return B230400 ;
#endif
#ifdef B460800
    case  460800: return B460800 ;
#endif
#ifdef B500000
    case  500000: return B500000 ;
#endif
#ifdef B576000
    case  576000: return B576000 ;
#endif
#ifdef B921600
    case  921600: return B921600 ;
#endif
#ifdef B1000000
    case 1000000: return B1000000 ;
#endif
#ifdef B1152000
    case 1152000: return B1152000 ;
#endif
#ifdef B1500000
    case 1500000: return B1500000 ;
#endif
#ifdef B2000000
    case 2000000: return B2000000 ;
#endif
#ifdef B2500000
    case 2500000: return B2500000 ;
#endif
#ifdef B3000000
    case 3000000: return B3000000 ;
#endif
#ifdef B3500000
    case 3500000: return B3500000 ;
#endif
#ifdef B4000000
    case 4000000: return B4000000 ;
#endif
    default: return (speed_t) 0 ;
  }
}

static unsigned int serialBaudNumber (const speed_t baud)
{
  switch (baud)
  {
    case B0: return 0 ;
    case B50: return 50 ;
    case B75: return 75 ;
    case B110: return 110 ;
    case B134: return 134 ;
    case B150: return 150 ;
    case B200: return 200 ;
    case B300: return 300 ;
    case B600: return 600 ;
    case B1200: return 1200 ;
    case B1800: return 1800 ;
    case B2400: return 2400 ;
    case B4800: return 4800 ;
    case B9600: return 9600 ;
    case B19200: return 19200 ;
    case B38400: return 38400 ;
#ifdef B57600
    case B57600: return 57600 ;
#endif
#ifdef B115200
    case B115200: return 115200 ;
#endif
#ifdef B230400
    case B230400: return 230400 ;
#endif
#ifdef B460800
    case B460800: return 460800 ;
#endif
#ifdef B500000
    case B500000: return 500000 ;
#endif
#ifdef B576000
    case B576000: return 576000 ;
#endif
#ifdef B921600
    case B921600: return 921600 ;
#endif
#ifdef B1000000
    case B1000000: return 1000000 ;
#endif
#ifdef B1152000
    case B1152000: return 1152000 ;
#endif
#ifdef B1500000
    case B1500000: return 1500000 ;
#endif
#ifdef B2000000
    case B2000000: return 2000000 ;
#endif
#ifdef B2500000
    case B2500000: return 2500000 ;
#endif
#ifdef B3000000
    case B3000000: return 3000000 ;
#endif
#ifdef B3500000
    case B3500000: return 3500000 ;
#endif
#ifdef B4000000
    case B4000000: return 4000000 ;
#endif
    default: return 0 ;
  }
}

static int64_t serialMonotonicMs (void)
{
  struct timespec ts ;
  if (clock_gettime (CLOCK_MONOTONIC, &ts) < 0)
    return -1 ;
  return ((int64_t) ts.tv_sec * 1000) + (ts.tv_nsec / 1000000) ;
}

static int serialRemainingMs (const int timeoutMs, const int64_t deadline)
{
  int64_t now, remaining ;
  if (timeoutMs < 0)
    return -1 ;
  now = serialMonotonicMs () ;
  if (now < 0)
    return 0 ;
  remaining = deadline - now ;
  if (remaining <= 0)
    return 0 ;
  if (remaining > INT_MAX)
    return INT_MAX ;
  return (int) remaining ;
}

static int serialPollOne (const int fd, const short events, const int timeoutMs,
                          const int64_t deadline)
{
  struct pollfd pfd ;
  int result, waitMs ;
  pfd.fd = fd ; pfd.events = events ; pfd.revents = 0 ;
  for (;;)
  {
    waitMs = timeoutMs < 0 ? -1 : serialRemainingMs (timeoutMs, deadline) ;
    result = poll (&pfd, 1, waitMs) ;
    if (result < 0 && errno == EINTR)
      continue ;
    if (result <= 0)
      return result ;
    if (pfd.revents & POLLNVAL) { errno = EBADF ; return -1 ; }
    if (pfd.revents & POLLERR) { errno = EIO ; return -1 ; }
    if (pfd.revents & (events | POLLHUP)) return 1 ;
  }
}

static int serialSetNonBlocking (const int fd, const int enabled, int *oldFlags)
{
  int flags, requested ;
  flags = fcntl (fd, F_GETFL) ;
  if (flags < 0) return -1 ;
  if (oldFlags != NULL) *oldFlags = flags ;
  requested = enabled ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK) ;
  if (requested == flags) return 0 ;
  return fcntl (fd, F_SETFL, requested) ;
}

static void serialRestoreFlags (const int fd, const int oldFlags, const int savedErrno)
{
  (void) fcntl (fd, F_SETFL, oldFlags) ;
  errno = savedErrno ;
}

static int serialTtyName (const int fd, char *name, const size_t nameSize)
{
  char path [PATH_MAX] ;
  const char *base ;
  int result ;
  if (name == NULL || nameSize == 0) return serialBadArgument () ;
  result = ttyname_r (fd, path, sizeof (path)) ;
  if (result != 0) { errno = result ; return -1 ; }
  base = strrchr (path, '/') ; base = base == NULL ? path : base + 1 ;
  if (*base == '\0' || strlen (base) + 1 > nameSize) { errno = ENAMETOOLONG ; return -1 ; }
  strcpy (name, base) ;
  return 0 ;
}

static int serialSysfsPath (const int fd, const char *suffix, char *path, const size_t pathSize)
{
  char name [128] ;
  int count ;
  if (suffix == NULL || path == NULL || pathSize == 0) return serialBadArgument () ;
  if (serialTtyName (fd, name, sizeof (name)) < 0) return -1 ;
  count = snprintf (path, pathSize, "/sys/class/tty/%s/%s", name, suffix) ;
  if (count < 0 || (size_t) count >= pathSize) { errno = ENAMETOOLONG ; return -1 ; }
  return 0 ;
}

static int serialReadTextFile (const char *path, char *buffer, const size_t size)
{
  int fd, savedErrno ; ssize_t count ;
  if (path == NULL || buffer == NULL || size < 2) return serialBadArgument () ;
  fd = open (path, O_RDONLY | O_CLOEXEC) ; if (fd < 0) return -1 ;
  do count = read (fd, buffer, size - 1) ; while (count < 0 && errno == EINTR) ;
  savedErrno = errno ; close (fd) ; errno = savedErrno ;
  if (count < 0) return -1 ;
  buffer[count] = '\0' ; return (int) count ;
}

static int serialWriteTextFile (const char *path, const char *text)
{
  int fd, savedErrno ; size_t total = 0, size ; ssize_t count ;
  if (path == NULL || text == NULL) return serialBadArgument () ;
  fd = open (path, O_WRONLY | O_CLOEXEC) ; if (fd < 0) return -1 ;
  size = strlen (text) ;
  while (total < size)
  {
    count = write (fd, text + total, size - total) ;
    if (count < 0 && errno == EINTR) continue ;
    if (count <= 0) { if (count == 0) errno = EIO ; savedErrno = errno ; close(fd); errno=savedErrno; return -1; }
    total += (size_t) count ;
  }
  savedErrno = errno ; if (close(fd) < 0) return -1 ; errno = savedErrno ; return 0 ;
}

static int serialReadUnsignedSysfs (const int fd, const char *suffix, unsigned int *value)
{
  char path[PATH_MAX], text[128], *end ; unsigned long parsed ;
  if (value == NULL) return serialBadArgument () ;
  if (serialSysfsPath(fd,suffix,path,sizeof(path)) < 0 || serialReadTextFile(path,text,sizeof(text)) < 0) return -1 ;
  errno=0 ; parsed=strtoul(text,&end,0) ;
  if (errno != 0 || end == text || parsed > UINT_MAX) { errno=EIO ; return -1 ; }
  *value=(unsigned int)parsed ; return 0 ;
}

static int serialOfCompatibleContains (const int fd, const char *needle)
{
  char path[PATH_MAX], buffer[1024] ; int count, offset=0 ;
  if (needle == NULL || serialSysfsPath(fd,"device/of_node/compatible",path,sizeof(path)) < 0) return 0 ;
  count=serialReadTextFile(path,buffer,sizeof(buffer)); if(count<=0)return 0;
  while(offset<count){ size_t n=strnlen(buffer+offset,(size_t)(count-offset)); if(strcmp(buffer+offset,needle)==0)return 1; offset+=(int)n+1; }
  return 0 ;
}

static int serialSysfsExists (const int fd, const char *suffix, const int writeAccess)
{
  char path[PATH_MAX] ;
  if (serialSysfsPath(fd,suffix,path,sizeof(path)) < 0) return 0 ;
  return access(path,writeAccess ? W_OK : R_OK) == 0 ;
}

void serialConfigInit (serialConfig *config)
{
  if (config == NULL) return ;
  memset(config,0,sizeof(*config)); config->structSize=(uint32_t)sizeof(*config); config->version=SERIAL_CONFIG_VERSION;
  config->baud=9600; config->dataBits=8; config->parity=SERIAL_PARITY_NONE; config->stopBits=SERIAL_STOP_BITS_ONE;
  config->flowControl=SERIAL_FLOW_NONE; config->xonChar=0x11; config->xoffChar=0x13; config->vmin=0; config->vtime=0;
}

static int serialValidateConfig (const serialConfig *config)
{
  if (config == NULL || config->structSize < sizeof(*config) || config->version != SERIAL_CONFIG_VERSION) return serialBadArgument();
  if (config->dataBits < 5 || config->dataBits > 8 || config->parity > SERIAL_PARITY_SPACE) return serialBadArgument();
  if (config->stopBits < SERIAL_STOP_BITS_ONE || config->stopBits > SERIAL_STOP_BITS_TWO) return serialBadArgument();
  if (config->stopBits == SERIAL_STOP_BITS_ONE_POINT_FIVE && config->dataBits != 5) return serialBadArgument();
  if (config->flowControl & ~(SERIAL_FLOW_XON_XOFF|SERIAL_FLOW_RTS_CTS)) return serialBadArgument();
  return 0;
}

int serialSetBaud (const int fd, const unsigned int baud)
{
  struct termios options ; speed_t speed ;
#ifdef __linux__
  {
    serialHardwareInfo hardware ;
    if (baud > 4000000U && serialGetHardwareInfo(fd,&hardware)==0 && hardware.hardwareType==SERIAL_HARDWARE_RK3588_UART)
      { errno=ERANGE; return -1; }
  }
#endif
  speed=serialBaudConstant(baud);
  if (baud==0 || speed!=0)
  {
    if(tcgetattr(fd,&options)<0)return -1;
    if(cfsetispeed(&options,speed)<0 || cfsetospeed(&options,speed)<0)return -1;
    return tcsetattr(fd,TCSANOW,&options);
  }
#ifdef __linux__
  {
    struct termios2 options2 ;
    if(ioctl(fd,TCGETS2,&options2)<0)return -1;
    options2.c_cflag &= ~CBAUD; options2.c_cflag |= BOTHER; options2.c_ispeed=baud; options2.c_ospeed=baud;
    return ioctl(fd,TCSETS2,&options2);
  }
#else
  errno=EINVAL; return -1;
#endif
}

int serialGetBaud (const int fd, unsigned int *baud)
{
  struct termios options ;
  if(baud==NULL)return serialBadArgument();
#ifdef __linux__
  { struct termios2 options2; if(ioctl(fd,TCGETS2,&options2)==0 && options2.c_ospeed!=0){*baud=options2.c_ospeed;return 0;} }
#endif
  if(tcgetattr(fd,&options)<0)return -1;
  *baud=serialBaudNumber(cfgetospeed(&options));
  if(*baud==0 && cfgetospeed(&options)!=B0){errno=EIO;return -1;}
  return 0;
}

int serialSetConfig (const int fd, const serialConfig *config)
{
  struct termios options ; speed_t speed ; int customBaud ;
  if(serialValidateConfig(config)<0 || tcgetattr(fd,&options)<0)return -1;
#ifdef __linux__
  if(config->parity==SERIAL_PARITY_MARK || config->parity==SERIAL_PARITY_SPACE)
  { serialHardwareInfo hardware; if(serialGetHardwareInfo(fd,&hardware)==0 && hardware.hardwareType==SERIAL_HARDWARE_RK3588_UART){errno=ENOTSUP;return -1;} }
#endif
  cfmakeraw(&options); options.c_cflag |= CLOCAL|CREAD; options.c_cflag &= ~(CSIZE|PARENB|PARODD|CSTOPB);
#if CMSPAR != 0
  options.c_cflag &= ~CMSPAR;
#endif
#if CRTSCTS != 0
  options.c_cflag &= ~CRTSCTS;
#endif
  switch(config->dataBits){case 5:options.c_cflag|=CS5;break;case 6:options.c_cflag|=CS6;break;case 7:options.c_cflag|=CS7;break;case 8:options.c_cflag|=CS8;break;default:return serialBadArgument();}
  switch(config->parity){
    case SERIAL_PARITY_NONE:break; case SERIAL_PARITY_ODD:options.c_cflag|=PARENB|PARODD;break; case SERIAL_PARITY_EVEN:options.c_cflag|=PARENB;break;
    case SERIAL_PARITY_MARK:
#if CMSPAR != 0
      options.c_cflag|=PARENB|PARODD|CMSPAR;break;
#else
      errno=ENOTSUP;return -1;
#endif
    case SERIAL_PARITY_SPACE:
#if CMSPAR != 0
      options.c_cflag|=PARENB|CMSPAR;break;
#else
      errno=ENOTSUP;return -1;
#endif
    default:return serialBadArgument();
  }
  if(config->stopBits!=SERIAL_STOP_BITS_ONE)options.c_cflag|=CSTOPB;
  if(config->flowControl&SERIAL_FLOW_XON_XOFF)options.c_iflag|=IXON|IXOFF;else{options.c_iflag&=~(IXON|IXOFF);
#ifdef IXANY
    options.c_iflag&=~IXANY;
#endif
  }
  if(config->flowControl&SERIAL_FLOW_RTS_CTS){
#if CRTSCTS != 0
    options.c_cflag|=CRTSCTS;
#else
    errno=ENOTSUP;return -1;
#endif
  }
  options.c_cc[VSTART]=config->xonChar; options.c_cc[VSTOP]=config->xoffChar; options.c_cc[VMIN]=config->vmin; options.c_cc[VTIME]=config->vtime;
  speed=serialBaudConstant(config->baud); customBaud=(config->baud!=0 && speed==0); if(customBaud)speed=B38400;
  if(cfsetispeed(&options,speed)<0 || cfsetospeed(&options,speed)<0 || tcsetattr(fd,TCSANOW,&options)<0)return -1;
  return customBaud ? serialSetBaud(fd,config->baud) : 0;
}

int serialGetConfig (const int fd, serialConfig *config)
{
  struct termios options ; unsigned int baud ;
  if(config==NULL)return serialBadArgument();
  if(tcgetattr(fd,&options)<0 || serialGetBaud(fd,&baud)<0)return -1;
  serialConfigInit(config); config->baud=baud;
  switch(options.c_cflag&CSIZE){case CS5:config->dataBits=5;break;case CS6:config->dataBits=6;break;case CS7:config->dataBits=7;break;case CS8:config->dataBits=8;break;default:config->dataBits=8;break;}
  if(!(options.c_cflag&PARENB))config->parity=SERIAL_PARITY_NONE;
#if CMSPAR != 0
  else if(options.c_cflag&CMSPAR)config->parity=(options.c_cflag&PARODD)?SERIAL_PARITY_MARK:SERIAL_PARITY_SPACE;
#endif
  else config->parity=(options.c_cflag&PARODD)?SERIAL_PARITY_ODD:SERIAL_PARITY_EVEN;
  if(!(options.c_cflag&CSTOPB))config->stopBits=SERIAL_STOP_BITS_ONE;else if(config->dataBits==5)config->stopBits=SERIAL_STOP_BITS_ONE_POINT_FIVE;else config->stopBits=SERIAL_STOP_BITS_TWO;
  config->flowControl=SERIAL_FLOW_NONE; if((options.c_iflag&(IXON|IXOFF))!=0)config->flowControl|=SERIAL_FLOW_XON_XOFF;
#if CRTSCTS != 0
  if(options.c_cflag&CRTSCTS)config->flowControl|=SERIAL_FLOW_RTS_CTS;
#endif
  config->xonChar=options.c_cc[VSTART];config->xoffChar=options.c_cc[VSTOP];config->vmin=options.c_cc[VMIN];config->vtime=options.c_cc[VTIME];return 0;
}

int serialOpenConfig (const char *device, const serialConfig *config)
{
  int fd,flags,savedErrno,modem;
  if(device==NULL || serialValidateConfig(config)<0)return -1;
  fd=open(device,O_RDWR|O_NOCTTY|O_NONBLOCK|O_CLOEXEC);if(fd<0)return -1;
  if(serialSetConfig(fd,config)<0)goto fail;
  flags=fcntl(fd,F_GETFL);if(flags<0 || fcntl(fd,F_SETFL,flags&~O_NONBLOCK)<0)goto fail;
  if(!(config->flowControl&SERIAL_FLOW_RTS_CTS) && ioctl(fd,TIOCMGET,&modem)==0){modem|=TIOCM_RTS|TIOCM_DTR;(void)ioctl(fd,TIOCMSET,&modem);}
  return fd;
fail:savedErrno=errno;close(fd);errno=savedErrno;return -1;
}

ssize_t serialRead (const int fd, void *buffer, const size_t count)
{ if(buffer==NULL && count!=0){errno=EINVAL;return -1;} return read(fd,buffer,count); }
ssize_t serialWrite (const int fd, const void *buffer, const size_t count)
{ if(buffer==NULL && count!=0){errno=EINVAL;return -1;} return write(fd,buffer,count); }

ssize_t serialReadTimeout (const int fd, void *buffer, const size_t count, const int timeoutMs, const int interByteTimeoutMs)
{
  unsigned char *out=(unsigned char*)buffer; size_t total=0,request; int oldFlags,changed,ready,available,waitMs,savedErrno; int64_t deadline,interDeadline=0; ssize_t got;
  if((buffer==NULL&&count!=0)||timeoutMs< -1||interByteTimeoutMs< -1){errno=EINVAL;return -1;} if(count==0)return 0;
  deadline=timeoutMs<0?0:serialMonotonicMs()+timeoutMs; if(serialSetNonBlocking(fd,1,&oldFlags)<0)return -1; changed=!(oldFlags&O_NONBLOCK);
  for(;;){
    if (total >= count) break;
    waitMs = timeoutMs < 0 ? -1 : serialRemainingMs(timeoutMs, deadline);
    if(total>0&&interByteTimeoutMs>=0){int r=serialRemainingMs(interByteTimeoutMs,interDeadline);if(waitMs<0||r<waitMs)waitMs=r;}
    {int64_t pd=waitMs<0?0:serialMonotonicMs()+waitMs;ready=serialPollOne(fd,POLLIN,waitMs,pd);} if(ready<0)goto fail;if(ready==0)break;
    available=0;if(ioctl(fd,FIONREAD,&available)<0||available<=0)request=count-total;else{request=(size_t)available;if(request>count-total)request=count-total;}
    got=read(fd,out+total,request);if(got<0){if(errno==EINTR||errno==EAGAIN||errno==EWOULDBLOCK)continue;goto fail;}if(got==0)break;
    total+=(size_t)got;if(interByteTimeoutMs>=0)interDeadline=serialMonotonicMs()+interByteTimeoutMs;
  }
  savedErrno=errno;if(changed)serialRestoreFlags(fd,oldFlags,savedErrno);return(ssize_t)total;
fail:savedErrno=errno;if(changed)serialRestoreFlags(fd,oldFlags,savedErrno);return -1;
}

ssize_t serialWriteTimeout (const int fd, const void *buffer, const size_t count, const int timeoutMs)
{
  const unsigned char *data=(const unsigned char*)buffer;size_t total=0;int oldFlags,changed,ready,savedErrno;int64_t deadline;ssize_t written;
  if((buffer==NULL&&count!=0)||timeoutMs< -1){errno=EINVAL;return -1;}if(count==0)return 0;deadline=timeoutMs<0?0:serialMonotonicMs()+timeoutMs;
  if (serialSetNonBlocking(fd, 1, &oldFlags) < 0) return -1;
  changed = !(oldFlags & O_NONBLOCK);
  while(total<count){written=write(fd,data+total,count-total);if(written>0){total+=(size_t)written;continue;}if(written<0&&errno!=EINTR&&errno!=EAGAIN&&errno!=EWOULDBLOCK)goto fail;if(timeoutMs>=0&&serialRemainingMs(timeoutMs,deadline)==0)break;ready=serialPollOne(fd,POLLOUT,timeoutMs,deadline);if(ready<0)goto fail;if(ready==0)break;}
  savedErrno=errno;if(changed)serialRestoreFlags(fd,oldFlags,savedErrno);return(ssize_t)total;
fail:savedErrno=errno;if(changed)serialRestoreFlags(fd,oldFlags,savedErrno);return -1;
}

int serialDrain(const int fd){return tcdrain(fd);} int serialFlushInput(const int fd){return tcflush(fd,TCIFLUSH);} int serialFlushOutput(const int fd){return tcflush(fd,TCOFLUSH);}
int serialInputWaiting(const int fd){int result;if(ioctl(fd,FIONREAD,&result)<0)return -1;return result;}
int serialOutputWaiting(const int fd){
#ifdef TIOCOUTQ
  int result;if(ioctl(fd,TIOCOUTQ,&result)<0)return -1;return result;
#else
  (void)fd;errno=ENOTSUP;return -1;
#endif
}

int serialSetBreak(const int fd,const int enabled){
#if defined(TIOCSBRK) && defined(TIOCCBRK)
  return ioctl(fd,enabled?TIOCSBRK:TIOCCBRK);
#else
  if(enabled)return tcsendbreak(fd,0);errno=ENOTSUP;return -1;
#endif
}
int serialSendBreak(const int fd,const unsigned int durationMs){
#if defined(TIOCSBRK) && defined(TIOCCBRK)
  struct timespec d,r;if(serialSetBreak(fd,1)<0)return -1;d.tv_sec=durationMs/1000;d.tv_nsec=(long)(durationMs%1000)*1000000L;while(nanosleep(&d,&r)<0&&errno==EINTR)d=r;return serialSetBreak(fd,0);
#else
  (void)durationMs;return tcsendbreak(fd,0);
#endif
}

static unsigned int serialModemFromNative(const int native){unsigned int r=0;
#ifdef TIOCM_RTS
if(native&TIOCM_RTS)r|=SERIAL_MODEM_RTS;
#endif
#ifdef TIOCM_DTR
if(native&TIOCM_DTR)r|=SERIAL_MODEM_DTR;
#endif
#ifdef TIOCM_CTS
if(native&TIOCM_CTS)r|=SERIAL_MODEM_CTS;
#endif
#ifdef TIOCM_DSR
if(native&TIOCM_DSR)r|=SERIAL_MODEM_DSR;
#endif
#ifdef TIOCM_RI
if(native&TIOCM_RI)r|=SERIAL_MODEM_RI;
#endif
#ifdef TIOCM_CD
if(native&TIOCM_CD)r|=SERIAL_MODEM_CD;
#elif defined(TIOCM_CAR)
if(native&TIOCM_CAR)r|=SERIAL_MODEM_CD;
#endif
#ifdef TIOCM_LOOP
if(native&TIOCM_LOOP)r|=SERIAL_MODEM_LOOP;
#endif
return r;}
static int serialModemToNative(const unsigned int lines){int r=0;
#ifdef TIOCM_RTS
if(lines&SERIAL_MODEM_RTS)r|=TIOCM_RTS;
#endif
#ifdef TIOCM_DTR
if(lines&SERIAL_MODEM_DTR)r|=TIOCM_DTR;
#endif
#ifdef TIOCM_LOOP
if(lines&SERIAL_MODEM_LOOP)r|=TIOCM_LOOP;
#endif
return r;}
int serialGetModemLines(const int fd,unsigned int *lines){int native;if(lines==NULL)return serialBadArgument();if(ioctl(fd,TIOCMGET,&native)<0)return -1;*lines=serialModemFromNative(native);return 0;}
int serialSetModemLines(const int fd,const unsigned int setMask,const unsigned int clearMask){const unsigned int w=SERIAL_MODEM_RTS|SERIAL_MODEM_DTR|SERIAL_MODEM_LOOP;int n;if((setMask&clearMask)||((setMask|clearMask)&~w))return serialBadArgument();n=serialModemToNative(setMask);if(n&&ioctl(fd,TIOCMBIS,&n)<0)return -1;n=serialModemToNative(clearMask);if(n&&ioctl(fd,TIOCMBIC,&n)<0)return -1;return 0;}
int serialSetRTS(const int fd,const int enabled){return serialSetModemLines(fd,enabled?SERIAL_MODEM_RTS:0,enabled?0:SERIAL_MODEM_RTS);} int serialSetDTR(const int fd,const int enabled){return serialSetModemLines(fd,enabled?SERIAL_MODEM_DTR:0,enabled?0:SERIAL_MODEM_DTR);}
static int serialGetModemBit(const int fd,const unsigned int bit){unsigned int lines;if(serialGetModemLines(fd,&lines)<0)return -1;return(lines&bit)?1:0;}
int serialGetCTS(const int fd){return serialGetModemBit(fd,SERIAL_MODEM_CTS);} int serialGetDSR(const int fd){return serialGetModemBit(fd,SERIAL_MODEM_DSR);} int serialGetRI(const int fd){return serialGetModemBit(fd,SERIAL_MODEM_RI);} int serialGetCD(const int fd){return serialGetModemBit(fd,SERIAL_MODEM_CD);}
int serialSetInputFlow(const int fd,const int enabled){return tcflow(fd,enabled?TCION:TCIOFF);} int serialSetOutputFlow(const int fd,const int enabled){return tcflow(fd,enabled?TCOON:TCOOFF);} int serialSetExclusive(const int fd,const int enabled){return flock(fd,enabled?(LOCK_EX|LOCK_NB):LOCK_UN);}

int serialGetLowLatency(const int fd){
#ifdef __linux__
struct serial_struct info;if(ioctl(fd,TIOCGSERIAL,&info)<0)return -1;return(info.flags&ASYNC_LOW_LATENCY)?1:0;
#else
(void)fd;errno=ENOTSUP;return -1;
#endif
}
int serialSetLowLatency(const int fd,const int enabled){
#ifdef __linux__
struct serial_struct info;if(ioctl(fd,TIOCGSERIAL,&info)<0)return -1;if(enabled)info.flags|=ASYNC_LOW_LATENCY;else info.flags&=~ASYNC_LOW_LATENCY;return ioctl(fd,TIOCSSERIAL,&info);
#else
(void)fd;(void)enabled;errno=ENOTSUP;return -1;
#endif
}
int serialGetCounters(const int fd,serialCounters *counters){
#ifdef __linux__
struct serial_icounter_struct n;if(counters==NULL)return serialBadArgument();memset(&n,0,sizeof(n));if(ioctl(fd,TIOCGICOUNT,&n)<0)return -1;memset(counters,0,sizeof(*counters));counters->structSize=sizeof(*counters);counters->cts=n.cts;counters->dsr=n.dsr;counters->rng=n.rng;counters->dcd=n.dcd;counters->rx=n.rx;counters->tx=n.tx;counters->frame=n.frame;counters->overrun=n.overrun;counters->parity=n.parity;counters->brk=n.brk;counters->bufOverrun=n.buf_overrun;return 0;
#else
(void)fd;(void)counters;errno=ENOTSUP;return -1;
#endif
}
int serialTxEmpty(const int fd){
#ifdef TIOCSERGETLSR
int status;if(ioctl(fd,TIOCSERGETLSR,&status)<0)return -1;
#ifdef TIOCSER_TEMT
return(status&TIOCSER_TEMT)?1:0;
#else
return status?1:0;
#endif
#else
(void)fd;errno=ENOTSUP;return -1;
#endif
}

static void serialRS485FromNative(serialRS485Config *config,
#ifdef __linux__
const struct serial_rs485 *native
#else
const void *native
#endif
){memset(config,0,sizeof(*config));config->structSize=sizeof(*config);
#ifdef __linux__
if (native->flags & SER_RS485_ENABLED) config->flags |= SERIAL_RS485_ENABLED;
if (native->flags & SER_RS485_RTS_ON_SEND) config->flags |= SERIAL_RS485_RTS_ON_SEND;
if (native->flags & SER_RS485_RTS_AFTER_SEND) config->flags |= SERIAL_RS485_RTS_AFTER_SEND;
if (native->flags & SER_RS485_RX_DURING_TX) config->flags |= SERIAL_RS485_RX_DURING_TX;
#ifdef SER_RS485_TERMINATE_BUS
if(native->flags&SER_RS485_TERMINATE_BUS)config->flags|=SERIAL_RS485_TERMINATE_BUS;
#endif
#ifdef SER_RS485_ADDRB
if(native->flags&SER_RS485_ADDRB)config->flags|=SERIAL_RS485_ADDRESS_MODE;
#endif
#ifdef SER_RS485_ADDR_RECV
if(native->flags&SER_RS485_ADDR_RECV)config->flags|=SERIAL_RS485_RX_ADDRESS;
#endif
#ifdef SER_RS485_ADDR_DEST
if(native->flags&SER_RS485_ADDR_DEST)config->flags|=SERIAL_RS485_DEST_ADDRESS;
#endif
#ifdef SER_RS485_MODE_RS422
if(native->flags&SER_RS485_MODE_RS422)config->flags|=SERIAL_RS485_MODE_RS422;
#endif
config->delayBeforeSendMs=native->delay_rts_before_send;config->delayAfterSendMs=native->delay_rts_after_send;
#if defined(SER_RS485_ADDR_RECV) || defined(SER_RS485_ADDR_DEST)
config->receiveAddress=native->addr_recv;config->destinationAddress=native->addr_dest;
#endif
#else
(void)native;
#endif
}
int serialGetRS485(const int fd,serialRS485Config *config){
#ifdef __linux__
struct serial_rs485 n;if(config==NULL)return serialBadArgument();memset(&n,0,sizeof(n));if(ioctl(fd,TIOCGRS485,&n)<0)return -1;serialRS485FromNative(config,&n);return 0;
#else
(void)fd;(void)config;errno=ENOTSUP;return -1;
#endif
}
int serialSetRS485(const int fd,const serialRS485Config *config){
#ifdef __linux__
struct serial_rs485 n;const uint32_t allowed=SERIAL_RS485_ENABLED|SERIAL_RS485_RTS_ON_SEND|SERIAL_RS485_RTS_AFTER_SEND|SERIAL_RS485_RX_DURING_TX|SERIAL_RS485_TERMINATE_BUS|SERIAL_RS485_ADDRESS_MODE|SERIAL_RS485_RX_ADDRESS|SERIAL_RS485_DEST_ADDRESS|SERIAL_RS485_MODE_RS422;
if (config == NULL || config->structSize < sizeof(*config) || (config->flags & ~allowed)) return serialBadArgument();
if ((config->flags & (SERIAL_RS485_RX_ADDRESS | SERIAL_RS485_DEST_ADDRESS)) && !(config->flags & SERIAL_RS485_ADDRESS_MODE)) return serialBadArgument();
memset(&n, 0, sizeof(n));
if (config->flags & SERIAL_RS485_ENABLED) n.flags |= SER_RS485_ENABLED;
if (config->flags & SERIAL_RS485_RTS_ON_SEND) n.flags |= SER_RS485_RTS_ON_SEND;
if (config->flags & SERIAL_RS485_RTS_AFTER_SEND) n.flags |= SER_RS485_RTS_AFTER_SEND;
if (config->flags & SERIAL_RS485_RX_DURING_TX) n.flags |= SER_RS485_RX_DURING_TX;
#ifdef SER_RS485_TERMINATE_BUS
if(config->flags&SERIAL_RS485_TERMINATE_BUS)n.flags|=SER_RS485_TERMINATE_BUS;
#else
if(config->flags&SERIAL_RS485_TERMINATE_BUS){errno=ENOTSUP;return -1;}
#endif
#ifdef SER_RS485_ADDRB
if(config->flags&SERIAL_RS485_ADDRESS_MODE)n.flags|=SER_RS485_ADDRB;
#else
if(config->flags&(SERIAL_RS485_ADDRESS_MODE|SERIAL_RS485_RX_ADDRESS|SERIAL_RS485_DEST_ADDRESS)){errno=ENOTSUP;return -1;}
#endif
#ifdef SER_RS485_ADDR_RECV
if(config->flags&SERIAL_RS485_RX_ADDRESS)n.flags|=SER_RS485_ADDR_RECV;
#endif
#ifdef SER_RS485_ADDR_DEST
if(config->flags&SERIAL_RS485_DEST_ADDRESS)n.flags|=SER_RS485_ADDR_DEST;
#endif
#ifdef SER_RS485_MODE_RS422
if(config->flags&SERIAL_RS485_MODE_RS422)n.flags|=SER_RS485_MODE_RS422;
#else
if(config->flags&SERIAL_RS485_MODE_RS422){errno=ENOTSUP;return -1;}
#endif
n.delay_rts_before_send=config->delayBeforeSendMs;n.delay_rts_after_send=config->delayAfterSendMs;
#if defined(SER_RS485_ADDR_RECV) || defined(SER_RS485_ADDR_DEST)
n.addr_recv=config->receiveAddress;n.addr_dest=config->destinationAddress;
#endif
return ioctl(fd,TIOCSRS485,&n);
#else
(void)fd;(void)config;errno=ENOTSUP;return -1;
#endif
}

int serialGetRxTrigger(const int fd){
#ifdef __linux__
unsigned int v;if(serialReadUnsignedSysfs(fd,"rx_trig_bytes",&v)<0)return -1;if(v>INT_MAX){errno=ERANGE;return -1;}return(int)v;
#else
(void)fd;errno=ENOTSUP;return -1;
#endif
}
int serialSetRxTrigger(const int fd,const unsigned int bytes){
#ifdef __linux__
char path[PATH_MAX],value[32];if(bytes>255)return serialBadArgument();if(serialSysfsPath(fd,"rx_trig_bytes",path,sizeof(path))<0)return -1;snprintf(value,sizeof(value),"%u\n",bytes);return serialWriteTextFile(path,value);
#else
(void)fd;(void)bytes;errno=ENOTSUP;return -1;
#endif
}
int serialGetWakeup(const int fd){
#ifdef __linux__
char path[PATH_MAX],value[32];if(serialSysfsPath(fd,"device/power/wakeup",path,sizeof(path))<0||serialReadTextFile(path,value,sizeof(value))<0)return -1;if(strncmp(value,"enabled",7)==0)return 1;if(strncmp(value,"disabled",8)==0)return 0;errno=EIO;return -1;
#else
(void)fd;errno=ENOTSUP;return -1;
#endif
}
int serialSetWakeup(const int fd,const int enabled){
#ifdef __linux__
char path[PATH_MAX];if(serialSysfsPath(fd,"device/power/wakeup",path,sizeof(path))<0)return -1;return serialWriteTextFile(path,enabled?"enabled\n":"disabled\n");
#else
(void)fd;(void)enabled;errno=ENOTSUP;return -1;
#endif
}

int serialGetHardwareInfo(const int fd,serialHardwareInfo *info){uint64_t capabilities=0;if(info==NULL)return serialBadArgument();memset(info,0,sizeof(*info));info->structSize=sizeof(*info);info->hardwareType=SERIAL_HARDWARE_UNKNOWN;
#ifdef __linux__
{struct serial_struct n;if(ioctl(fd,TIOCGSERIAL,&n)==0){info->hardwareType=SERIAL_HARDWARE_8250;info->fifoSize=n.xmit_fifo_size;info->baudBase=n.baud_base;info->portType=n.type;info->line=n.line;capabilities|=SERIAL_CAP_8250;if(n.xmit_fifo_size>1)capabilities|=SERIAL_CAP_FIFO;}}
if(serialOfCompatibleContains(fd,"snps,dw-apb-uart")){info->hardwareType=SERIAL_HARDWARE_DW_APB_UART;capabilities|=SERIAL_CAP_DW_APB_UART;}
if(serialOfCompatibleContains(fd,"rockchip,rk3588-uart")){info->hardwareType=SERIAL_HARDWARE_RK3588_UART;capabilities|=SERIAL_CAP_DW_APB_UART|SERIAL_CAP_RK3588_UART|SERIAL_CAP_AUTO_RTS_CTS;if(info->fifoSize==0)info->fifoSize=64;}
if(serialSysfsExists(fd,"device/of_node/dmas",0))capabilities|=SERIAL_CAP_DMA_DESCRIBED;
#endif
info->capabilities=capabilities;return 0;}

int serialGetCapabilities(const int fd,uint64_t *capabilities){uint64_t flags=SERIAL_CAP_XON_XOFF|SERIAL_CAP_EXCLUSIVE|SERIAL_CAP_CANCEL_IO;serialHardwareInfo info;if(capabilities==NULL)return serialBadArgument();
#if CRTSCTS != 0
flags|=SERIAL_CAP_RTS_CTS;
#endif
#if CMSPAR != 0
flags|=SERIAL_CAP_MARK_SPACE_PARITY;
#endif
#ifdef TIOCMGET
{int modem;if(ioctl(fd,TIOCMGET,&modem)==0)flags|=SERIAL_CAP_MODEM_CONTROL|SERIAL_CAP_MODEM_STATUS;}
#endif
#ifdef __linux__
{struct termios2 t;struct serial_struct s;struct serial_rs485 r;struct serial_icounter_struct c;if(ioctl(fd,TCGETS2,&t)==0)flags|=SERIAL_CAP_CUSTOM_BAUD;if(ioctl(fd,TIOCGSERIAL,&s)==0)flags|=SERIAL_CAP_LOW_LATENCY|SERIAL_CAP_8250|(s.xmit_fifo_size>1?SERIAL_CAP_FIFO:0);memset(&r,0,sizeof(r));if(ioctl(fd,TIOCGRS485,&r)==0)flags|=SERIAL_CAP_RS485;memset(&c,0,sizeof(c));if(ioctl(fd,TIOCGICOUNT,&c)==0)flags|=SERIAL_CAP_ICOUNT;
#ifdef TIOCSERGETLSR
{int lsr;if(ioctl(fd,TIOCSERGETLSR,&lsr)==0)flags|=SERIAL_CAP_TX_EMPTY;}
#endif
if (serialSysfsExists(fd, "rx_trig_bytes", 0)) flags |= SERIAL_CAP_RX_TRIGGER;
if (serialSysfsExists(fd, "device/power/wakeup", 0)) flags |= SERIAL_CAP_WAKEUP;
}
#endif
if(serialGetHardwareInfo(fd,&info)==0){flags|=info.capabilities;if(info.hardwareType==SERIAL_HARDWARE_RK3588_UART)flags&=~SERIAL_CAP_MARK_SPACE_PARITY;}*capabilities=flags;return 0;}

/* ------------------------------------------------------------------------- */
/* Cancellable context API                                                    */
/* ------------------------------------------------------------------------- */

static int serialPipeCreate (int pipefd[2])
{
#ifdef __linux__
  if (pipe2 (pipefd, O_CLOEXEC | O_NONBLOCK) == 0) return 0 ;
  if (errno != ENOSYS) return -1 ;
#endif
  if (pipe (pipefd) < 0) return -1 ;
  if (fcntl(pipefd[0],F_SETFD,FD_CLOEXEC)<0 || fcntl(pipefd[1],F_SETFD,FD_CLOEXEC)<0 ||
      fcntl(pipefd[0],F_SETFL,fcntl(pipefd[0],F_GETFL)|O_NONBLOCK)<0 ||
      fcntl(pipefd[1],F_SETFL,fcntl(pipefd[1],F_GETFL)|O_NONBLOCK)<0)
  { int e=errno; close(pipefd[0]);close(pipefd[1]);errno=e;return -1; }
  return 0 ;
}

static void serialPipeDrain (const int fd)
{
  unsigned char b[64] ;
  while (read(fd,b,sizeof(b)) > 0) { }
}

static int serialPipeSignal (const int fd)
{
  unsigned char byte=1 ; ssize_t n ;
  do n=write(fd,&byte,1); while(n<0 && errno==EINTR);
  if(n==1 || (n<0 && (errno==EAGAIN||errno==EWOULDBLOCK))) return 0;
  return -1;
}

static serialContext *serialContextCreateOwnedFd (const int fd)
{
  serialContext *ctx ;
  int flags, savedErrno ;

  if (fd < 0) { errno = EBADF ; return NULL ; }
  flags = fcntl (fd, F_GETFL) ;
  if (flags < 0 || fcntl (fd, F_SETFL, flags | O_NONBLOCK) < 0)
  {
    savedErrno = errno ;
    close (fd) ;
    errno = savedErrno ;
    return NULL ;
  }

  ctx = (serialContext *) calloc (1, sizeof (*ctx)) ;
  if (ctx == NULL)
  {
    savedErrno = errno ;
    close (fd) ;
    errno = savedErrno ;
    return NULL ;
  }

  ctx->fd = fd ;
  ctx->ownFd = 1 ;
  ctx->cancelRead[0] = ctx->cancelRead[1] = -1 ;
  ctx->cancelWrite[0] = ctx->cancelWrite[1] = -1 ;
  if (serialPipeCreate (ctx->cancelRead) < 0 || serialPipeCreate (ctx->cancelWrite) < 0)
  {
    savedErrno = errno ;
    serialContextClose (ctx) ;
    errno = savedErrno ;
    return NULL ;
  }
  return ctx ;
}

serialContext *serialContextFromFd (const int fd, const int takeOwnership)
{
  char path [PATH_MAX] ;
  int privateFd ;
  serialContext *context ;

  if (fd < 0) { errno = EBADF ; return NULL ; }
  if (ttyname_r (fd, path, sizeof (path)) != 0)
  {
    errno = ENOTTY ;
    return NULL ;
  }
  privateFd = open (path, O_RDWR | O_NOCTTY | O_NONBLOCK | O_CLOEXEC) ;
  if (privateFd < 0)
    return NULL ;
  context = serialContextCreateOwnedFd (privateFd) ;
  if (context == NULL)
    return NULL ;
  /* Transfer caller ownership only after the new context is fully valid. */
  if (takeOwnership)
    close (fd) ;
  return context ;
}

serialContext *serialContextOpen (const char *device, const serialConfig *config)
{
  int fd = serialOpenConfig (device, config) ;
  if (fd < 0) return NULL ;
  return serialContextCreateOwnedFd (fd) ;
}

void serialContextClose (serialContext *context)
{
  if(context==NULL)return;
  if (context->cancelRead[0] >= 0) close(context->cancelRead[0]);
  if (context->cancelRead[1] >= 0) close(context->cancelRead[1]);
  if (context->cancelWrite[0] >= 0) close(context->cancelWrite[0]);
  if (context->cancelWrite[1] >= 0) close(context->cancelWrite[1]);
  if (context->ownFd && context->fd >= 0) close(context->fd);
  free(context);
}

int serialContextGetFd (const serialContext *context)
{ if(context==NULL){errno=EINVAL;return -1;} return context->fd; }
int serialContextCancelRead (serialContext *context)
{ if(context==NULL){errno=EINVAL;return -1;} return serialPipeSignal(context->cancelRead[1]); }
int serialContextCancelWrite (serialContext *context)
{ if(context==NULL){errno=EINVAL;return -1;} return serialPipeSignal(context->cancelWrite[1]); }

static int serialContextPoll (serialContext *context, const int cancelFd, const short event,
                              const int timeoutMs, const int64_t deadline)
{
  struct pollfd p[2]; int r,waitMs;
  p[0].fd=context->fd;p[0].events=event;p[0].revents=0;p[1].fd=cancelFd;p[1].events=POLLIN;p[1].revents=0;
  for(;;){waitMs=timeoutMs<0?-1:serialRemainingMs(timeoutMs,deadline);r=poll(p,2,waitMs);if(r<0&&errno==EINTR)continue;if(r<=0)return r;if(p[1].revents&POLLIN)return 2;if(p[0].revents&POLLNVAL){errno=EBADF;return -1;}if(p[0].revents&POLLERR){errno=EIO;return -1;}if(p[0].revents&(event|POLLHUP))return 1;}
}

ssize_t serialContextRead (serialContext *context, void *buffer, const size_t count,
                           const int timeoutMs, const int interByteTimeoutMs)
{
  unsigned char *out=(unsigned char*)buffer;size_t total=0;int r,waitMs;int64_t deadline,interDeadline=0;ssize_t n;
  if(context==NULL||(buffer==NULL&&count!=0)||timeoutMs< -1||interByteTimeoutMs< -1){errno=EINVAL;return -1;}if(count==0)return 0;
  serialPipeDrain(context->cancelRead[0]);deadline=timeoutMs<0?0:serialMonotonicMs()+timeoutMs;
  while(total<count){waitMs=timeoutMs<0?-1:serialRemainingMs(timeoutMs,deadline);if(total&&interByteTimeoutMs>=0){int q=serialRemainingMs(interByteTimeoutMs,interDeadline);if(waitMs<0||q<waitMs)waitMs=q;}
    {int64_t pd=waitMs<0?0:serialMonotonicMs()+waitMs;r=serialContextPoll(context,context->cancelRead[0],POLLIN,waitMs,pd);} if(r<0)return total?(ssize_t)total:-1;if(r==0)return(ssize_t)total;if(r==2){serialPipeDrain(context->cancelRead[0]);return(ssize_t)total;}
    n=read(context->fd,out+total,count-total);if(n>0){total+=(size_t)n;if(interByteTimeoutMs>=0)interDeadline=serialMonotonicMs()+interByteTimeoutMs;continue;}if(n==0)return(ssize_t)total;if(errno==EINTR||errno==EAGAIN||errno==EWOULDBLOCK)continue;return total?(ssize_t)total:-1;
  }return(ssize_t)total;
}

ssize_t serialContextWrite (serialContext *context, const void *buffer, const size_t count,
                            const int timeoutMs)
{
  const unsigned char *data=(const unsigned char*)buffer;size_t total=0;int r;int64_t deadline;ssize_t n;
  if(context==NULL||(buffer==NULL&&count!=0)||timeoutMs< -1){errno=EINVAL;return -1;}if(count==0)return 0;serialPipeDrain(context->cancelWrite[0]);deadline=timeoutMs<0?0:serialMonotonicMs()+timeoutMs;
  while(total<count){n=write(context->fd,data+total,count-total);if(n>0){total+=(size_t)n;continue;}if(n<0&&errno!=EINTR&&errno!=EAGAIN&&errno!=EWOULDBLOCK)return total?(ssize_t)total:-1;
    r=serialContextPoll(context,context->cancelWrite[0],POLLOUT,timeoutMs,deadline);if(r<0)return total?(ssize_t)total:-1;if(r==0)return(ssize_t)total;if(r==2){serialPipeDrain(context->cancelWrite[0]);return(ssize_t)total;}
  }return(ssize_t)total;
}
