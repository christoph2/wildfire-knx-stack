/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
*                                       cpu12.gems@googlemail.com>
*
*   All Rights Reserved
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "knx_et.h"
#include "port/port_serial.h"

#if defined(HAVE_POLL_H)

#endif /* HAVE_POLL_H */

#define FLUSH_TRANSMITTER   1

#if (defined(__CYGWIN__) && !defined(_WIN32)) || defined(__linux__)
    #define DEVICE_NAME "/dev/ttyS%u"
#endif

/* Function Prototypes. */
static boolean Serial_OpenPort(Port_Serial_ComPortType * port, uint16_t nBaudRate,uint8_t nParity, uint8_t nDataBits, uint8_t nStopBits);
static void Serial_ClosePort(Port_Serial_ComPortType const * port);
static uint16_t Serial_BytesWaiting(Port_Serial_ComPortType const * port, uint32_t * errors);
static boolean Serial_Write(Port_Serial_ComPortType * port, uint8_t const * buffer, uint32_t byteCount);
static boolean Serial_WriteByte(Port_Serial_ComPortType * port, uint8_t byteToWrite);
static Port_Serial_PollingResultType Serial_Poll(Port_Serial_ComPortType * port, boolean writing, uint16_t * events);
static void Serial_Flush(Port_Serial_ComPortType const * port);
static void Serial_FlushTransmitter(Port_Serial_ComPortType const * port);
static void Serial_FlushReceiver(Port_Serial_ComPortType const * port);

static Port_Serial_ComPortType ComPort;


static Port_Serial_PollingResultType Serial_Poll(Port_Serial_ComPortType * port, boolean writing, uint16_t * events)
{
    struct pollfd fds[1];
    int result;

    fds[0].fd = port->fd;
    fds[0].events = POLLERR| POLLHUP| POLLNVAL | (writing ? POLLOUT : POLLIN);

    //do {
        result = poll(fds, 1, 500);
    //} while (result == -1 && errno == EINTR);

    if (result == -1) {
        if (errno == EINTR) {
            //printf("<<POLL INTERRUPTED [%u]>>\n");
            return POLLING_INTERRUPTED;
        } else {
            KnxEt_Error("poll", errno);
            return POLLING_ERROR;
        }
    } else if (result == 0) {
        return POLLING_TIMEOUT;
    } else {
        *events = fds[0].revents;
    }
    return POLLING_OK;
}

static uint16_t Serial_BytesWaiting(Port_Serial_ComPortType const * port, uint32_t * errors)
{
    int count;

    *errors = ioctl(port->fd, TIOCINQ, &count);

    return count;
}


static boolean Serial_Write(Port_Serial_ComPortType * port, uint8_t const * buffer, uint32_t byteCount)
{
    int result;
    Port_Serial_PollingResultType pollingResult;
    uint16_t events;

    result = write(port->fd, buffer, byteCount);
#if defined(FLUSH_TRANSMITTER) && FLUSH_TRANSMITTER == 1
    tcdrain(port->fd);
#endif

    pollingResult = Serial_Poll(port, TRUE, &events);
    if (pollingResult == POLLING_OK) {
        //printf("Poll-Events: %02x\n", events);
    }

    return result != -1;
}


static boolean Serial_OpenPort(Port_Serial_ComPortType * port, uint16_t nBaudRate,uint8_t nParity, uint8_t nDataBits, uint8_t nStopBits)
{
    //int fd;
    //int cflag, lflag, iflag, oflag;
    struct termios flags;
    char deviceName[128];

    sprintf(deviceName, DEVICE_NAME, port->portNumber);

    // O_NDELAY
    port->fd = open(deviceName, O_RDWR | O_NOCTTY | O_NONBLOCK);    /* O_NDELAY | */
    if( port->fd == -1) {
        KnxEt_Error("open", errno);
        return FALSE;
    }

    if(!isatty(port->fd)) {
        KnxEt_Error("isatty", errno);
        return FALSE;
    }

    if (tcgetattr(port->fd, &flags) < 0) {
        KnxEt_Error("tcgetattr", errno);
        return FALSE;
    }

    flags.c_lflag &= ~(CSTOPB | PARODD | CSIZE);
    flags.c_cflag |=  (CLOCAL| CREAD | CS8 | PARENB);
    flags.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG | IEXTEN | ECHOCTL | ECHOKE);
    flags.c_oflag &= ~(OCRNL | ONLCR | ONLRET | ONOCR | OFILL | OLCUC | OPOST);
    flags.c_iflag &= ~(INLCR | IGNCR | ICRNL | IGNBRK | IUCLC | PARMRK| BRKINT | IXON | IXOFF | IXANY);
    flags.c_iflag |= (INPCK | ISTRIP);
#if 0
- 8N1
flags.c_cflag &= ~PARENB
flags.c_cflag &= ~CSTOPB
flags.c_cflag &= ~CSIZE
flags.c_cflag |= CS8

- 7E1
flags.c_cflag |= PARENB
flags.c_cflag &= ~PARODD
flags.c_cflag &= ~CSTOPB
flags.c_cflag &= ~CSIZE
flags.c_cflag |= CS7

- 7O1
flags.c_cflag |= PARENB
flags.c_cflag |= PARODD
flags.c_cflag &= ~CSTOPB
flags.c_cflag &= ~CSIZE
flags.c_cflag |= CS7

-7 S1
flags.c_cflag &= ~ PARENB
flags.c_cflag &= ~CSTOPB
flags.c_cflag &= ~CSIZE
flags.c_cflag |= CS8

#endif
    cfsetispeed(&flags, nBaudRate);
    cfsetospeed(&flags, nBaudRate);

    tcflush(port->fd, TCIOFLUSH);

    if (tcsetattr(port->fd, TCSANOW, &flags) < 0) {
        KnxEt_Error("tcsetattr", errno);
        return FALSE;
    }
#if 0
BRKINT      Signal interrupt on break.
ICRNL       Map CR to NL on input.
IGNBRK      Ignore break condition.
IGNCR       Ignore CR
IGNPAR      Ignore characters with parity errors.
INLCR       Map NL to CR on input.
INPCK       Enable input parity check.
ISTRIP      Strip character
IUCLC       Map upper-case to lower-case on input (LEGACY).
IXANY       Enable any character to restart output.
IXOFF       Enable start/stop input control.
IXON        Enable start/stop output control.
PARMRK      Mark parity errors.
#endif

    return TRUE;
}

static void Serial_FlushTransmitter(Port_Serial_ComPortType const * port)
{
    if (tcflush(port->fd, TCOFLUSH) == -1) {
        KnxEt_Error("tcflush", errno);
    }
}

static void Serial_FlushReceiver(Port_Serial_ComPortType const * port)
{
    if (tcflush(port->fd, TCIFLUSH) == -1) {
        KnxEt_Error("tcflush", errno);
    }
}

static void Serial_Flush(Port_Serial_ComPortType const * port)
{
    if (tcflush(port->fd, TCIOFLUSH) == -1) {
        KnxEt_Error("tcflush", errno);
    }
}

static void Serial_ClosePort(Port_Serial_ComPortType const * port)
{
    close(port->fd);
}

/*
**
** Global Functions.
**
*/

boolean Port_Serial_Init(uint8_t portNumber)
{
    ComPort.portNumber = portNumber;
    return Serial_OpenPort(&ComPort, B19200, PARENB, CS8, 1);
}

void Port_Serial_Deinit(void)
{
    Serial_ClosePort(&ComPort);
}

boolean Port_Serial_Write(uint8_t const * buffer, uint32_t byteCount)
{
    return Serial_Write(&ComPort, buffer, byteCount);
}

void Port_Serial_Flush(void)
{
    Serial_Flush(&ComPort);
}

void Port_Serial_FlushTransmitter(void)
{
    Serial_FlushTransmitter(&ComPort);
}

void Port_Serial_FlushReceiver(void)
{
    Serial_FlushReceiver(&ComPort);
}


Port_Serial_PollingResultType Port_Serial_Poll(boolean writing, uint16_t * events)
{

    return Serial_Poll(&ComPort, writing, events);
}

boolean Port_Serial_Read(uint8_t * buffer, uint16_t byteCount)
{
    return read(ComPort.fd, buffer, byteCount) != -1;
}

uint16_t Port_Serial_BytesWaiting(uint32_t * errors)
{
    return Serial_BytesWaiting(&ComPort, errors);
}

void Port_Serial_Task(void)
{
    uint8_t buffer[128];
    Port_Serial_PollingResultType pollingResult;
    uint16_t events;
    uint32_t errors;
    int byteCount;
    int idx;

    pollingResult = Port_Serial_Poll(FALSE, &events);

    if (pollingResult ==  POLLING_ERROR) {
        KnxEt_Error("Port_Serial_Poll", errno);
    } else if (pollingResult == POLLING_OK) {
        printf("Polling events: %04X\n", events);
        byteCount = Port_Serial_BytesWaiting(&errors);
        printf("Bytes waiting: %u\n", byteCount);
        if (!Port_Serial_Read(buffer, byteCount)) {
            KnxEt_Error("read", errno);
        } else {
            //KnxEt_DumpHex(buffer, byteCount);
            for (idx = 0; idx < byteCount; ++idx) {
                printf(" %02x", buffer[idx]);
                KnxLL_FeedReceiver(buffer[idx]);
            }
        }
    } else if (pollingResult == POLLING_TIMEOUT) {
        printf("Timeout.\n");
    } else {
    }
}
