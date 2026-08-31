/*
 * wiringSerial.h:
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

#ifndef __WIRING_SERIAL_H__
#define __WIRING_SERIAL_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

/*
 * The original descriptor-based API below is intentionally preserved.
 * The extended API is additive and also uses file descriptors so existing
 * applications can mix old and new calls without wrapper objects or copies.
 */

#define SERIAL_CONFIG_VERSION              1U
#define SERIAL_CONFIG_STRUCT_SIZE          ((uint32_t) sizeof (serialConfig))

#define SERIAL_PARITY_NONE                 0U
#define SERIAL_PARITY_ODD                  1U
#define SERIAL_PARITY_EVEN                 2U
#define SERIAL_PARITY_MARK                 3U
#define SERIAL_PARITY_SPACE                4U

#define SERIAL_STOP_BITS_ONE               1U
#define SERIAL_STOP_BITS_ONE_POINT_FIVE    2U
#define SERIAL_STOP_BITS_TWO               3U

#define SERIAL_FLOW_NONE                   0U
#define SERIAL_FLOW_XON_XOFF               (1U << 0)
#define SERIAL_FLOW_RTS_CTS                (1U << 1)

#define SERIAL_MODEM_RTS                   (1U << 0)
#define SERIAL_MODEM_DTR                   (1U << 1)
#define SERIAL_MODEM_CTS                   (1U << 2)
#define SERIAL_MODEM_DSR                   (1U << 3)
#define SERIAL_MODEM_RI                    (1U << 4)
#define SERIAL_MODEM_CD                    (1U << 5)
#define SERIAL_MODEM_LOOP                  (1U << 6)

#define SERIAL_RS485_ENABLED               (1U << 0)
#define SERIAL_RS485_RTS_ON_SEND           (1U << 1)
#define SERIAL_RS485_RTS_AFTER_SEND        (1U << 2)
#define SERIAL_RS485_RX_DURING_TX          (1U << 3)
#define SERIAL_RS485_TERMINATE_BUS         (1U << 4)
#define SERIAL_RS485_ADDRESS_MODE          (1U << 5)
#define SERIAL_RS485_RX_ADDRESS            (1U << 6)
#define SERIAL_RS485_DEST_ADDRESS          (1U << 7)
#define SERIAL_RS485_MODE_RS422            (1U << 8)

#define SERIAL_CAP_CUSTOM_BAUD             (1ULL << 0)
#define SERIAL_CAP_MARK_SPACE_PARITY       (1ULL << 1)
#define SERIAL_CAP_XON_XOFF                (1ULL << 2)
#define SERIAL_CAP_RTS_CTS                 (1ULL << 3)
#define SERIAL_CAP_MODEM_CONTROL           (1ULL << 4)
#define SERIAL_CAP_MODEM_STATUS            (1ULL << 5)
#define SERIAL_CAP_EXCLUSIVE               (1ULL << 6)
#define SERIAL_CAP_LOW_LATENCY             (1ULL << 7)
#define SERIAL_CAP_RS485                   (1ULL << 8)
#define SERIAL_CAP_ICOUNT                  (1ULL << 9)
#define SERIAL_CAP_TX_EMPTY                (1ULL << 10)
#define SERIAL_CAP_RX_TRIGGER              (1ULL << 11)
#define SERIAL_CAP_WAKEUP                  (1ULL << 12)
#define SERIAL_CAP_8250                    (1ULL << 13)
#define SERIAL_CAP_DW_APB_UART             (1ULL << 14)
#define SERIAL_CAP_RK3588_UART             (1ULL << 15)
#define SERIAL_CAP_DMA_DESCRIBED           (1ULL << 16)
#define SERIAL_CAP_AUTO_RTS_CTS            (1ULL << 17)
#define SERIAL_CAP_FIFO                    (1ULL << 18)
#define SERIAL_CAP_CANCEL_IO               (1ULL << 19)

#define SERIAL_HARDWARE_UNKNOWN            0U
#define SERIAL_HARDWARE_8250               1U
#define SERIAL_HARDWARE_DW_APB_UART        2U
#define SERIAL_HARDWARE_RK3588_UART        3U

typedef struct
{
  uint32_t structSize ;
  uint32_t version ;
  uint32_t baud ;
  uint8_t  dataBits ;
  uint8_t  parity ;
  uint8_t  stopBits ;
  uint8_t  flowControl ;
  uint8_t  xonChar ;
  uint8_t  xoffChar ;
  uint8_t  vmin ;
  uint8_t  vtime ;             /* Native termios deciseconds. */
  uint32_t reserved [4] ;
} serialConfig ;

typedef struct
{
  uint32_t structSize ;
  uint32_t flags ;
  uint32_t delayBeforeSendMs ;
  uint32_t delayAfterSendMs ;
  uint8_t  receiveAddress ;
  uint8_t  destinationAddress ;
  uint8_t  reserved8 [2] ;
  uint32_t reserved [4] ;
} serialRS485Config ;

typedef struct
{
  uint32_t structSize ;
  uint32_t cts ;
  uint32_t dsr ;
  uint32_t rng ;
  uint32_t dcd ;
  uint32_t rx ;
  uint32_t tx ;
  uint32_t frame ;
  uint32_t overrun ;
  uint32_t parity ;
  uint32_t brk ;
  uint32_t bufOverrun ;
  uint32_t reserved [4] ;
} serialCounters ;

typedef struct
{
  uint32_t structSize ;
  uint32_t hardwareType ;
  uint32_t fifoSize ;
  uint32_t baudBase ;
  uint32_t portType ;
  uint32_t line ;
  uint64_t capabilities ;
  uint32_t reserved [6] ;
} serialHardwareInfo ;

/* Opaque cancellable I/O context. Hot-path buffers remain caller-owned. */
typedef struct serialContext serialContext ;

#ifdef __cplusplus
extern "C" {
#endif

/* Legacy wiringPi/wiringOP API. */
extern int   serialOpen      (const char *device, const int baud) ;
extern void  serialClose     (const int fd) ;
extern void  serialFlush     (const int fd) ;
extern void  serialPutchar   (const int fd, const unsigned char c) ;
extern void  serialPuts      (const int fd, const char *s) ;
extern void  serialPrintf    (const int fd, const char *message, ...) ;
extern int   serialDataAvail (const int fd) ;
extern int   serialGetchar   (const int fd) ;

/* Extended, allocation-free descriptor API. */
extern void    serialConfigInit         (serialConfig *config) ;
extern int     serialOpenConfig         (const char *device, const serialConfig *config) ;
extern int     serialGetConfig          (const int fd, serialConfig *config) ;
extern int     serialSetConfig          (const int fd, const serialConfig *config) ;
extern int     serialGetBaud            (const int fd, unsigned int *baud) ;
extern int     serialSetBaud            (const int fd, const unsigned int baud) ;

extern ssize_t serialRead               (const int fd, void *buffer, const size_t count) ;
extern ssize_t serialWrite              (const int fd, const void *buffer, const size_t count) ;
extern ssize_t serialReadTimeout        (const int fd, void *buffer, const size_t count,
                                         const int timeoutMs, const int interByteTimeoutMs) ;
extern ssize_t serialWriteTimeout       (const int fd, const void *buffer, const size_t count,
                                         const int timeoutMs) ;

extern int     serialDrain              (const int fd) ;
extern int     serialFlushInput         (const int fd) ;
extern int     serialFlushOutput        (const int fd) ;
extern int     serialInputWaiting       (const int fd) ;
extern int     serialOutputWaiting      (const int fd) ;

extern int     serialSendBreak          (const int fd, const unsigned int durationMs) ;
extern int     serialSetBreak           (const int fd, const int enabled) ;

extern int     serialGetModemLines      (const int fd, unsigned int *lines) ;
extern int     serialSetModemLines      (const int fd, const unsigned int setMask,
                                         const unsigned int clearMask) ;
extern int     serialSetRTS             (const int fd, const int enabled) ;
extern int     serialSetDTR             (const int fd, const int enabled) ;
extern int     serialGetCTS             (const int fd) ;
extern int     serialGetDSR             (const int fd) ;
extern int     serialGetRI              (const int fd) ;
extern int     serialGetCD              (const int fd) ;

extern int     serialSetInputFlow       (const int fd, const int enabled) ;
extern int     serialSetOutputFlow      (const int fd, const int enabled) ;
extern int     serialSetExclusive       (const int fd, const int enabled) ;

extern int     serialGetLowLatency      (const int fd) ;
extern int     serialSetLowLatency      (const int fd, const int enabled) ;
extern int     serialGetCounters        (const int fd, serialCounters *counters) ;
extern int     serialTxEmpty            (const int fd) ;

extern int     serialGetRS485           (const int fd, serialRS485Config *config) ;
extern int     serialSetRS485           (const int fd, const serialRS485Config *config) ;

/* Linux 8250/DW-APB controls exposed by stable kernel interfaces/sysfs. */
extern int     serialGetRxTrigger       (const int fd) ;
extern int     serialSetRxTrigger       (const int fd, const unsigned int bytes) ;
extern int     serialGetWakeup          (const int fd) ;
extern int     serialSetWakeup          (const int fd, const int enabled) ;
extern int     serialGetHardwareInfo    (const int fd, serialHardwareInfo *info) ;
extern int     serialGetCapabilities    (const int fd, uint64_t *capabilities) ;

/*
 * Optional cancellable context API. It adds one allocation at open time only;
 * read/write buffers are never copied. serialContextCancel* is thread-safe and
 * wakes a blocking serialContextRead/Write without closing the tty descriptor.
 */
extern serialContext *serialContextOpen       (const char *device, const serialConfig *config) ;
extern serialContext *serialContextFromFd     (const int fd, const int takeOwnership) ;
extern void           serialContextClose      (serialContext *context) ;
extern int            serialContextGetFd      (const serialContext *context) ;
extern ssize_t        serialContextRead       (serialContext *context, void *buffer, const size_t count,
                                               const int timeoutMs, const int interByteTimeoutMs) ;
extern ssize_t        serialContextWrite      (serialContext *context, const void *buffer, const size_t count,
                                               const int timeoutMs) ;
extern int            serialContextCancelRead (serialContext *context) ;
extern int            serialContextCancelWrite(serialContext *context) ;

#ifdef __cplusplus
}
#endif

#endif
