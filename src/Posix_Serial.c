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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <unistd.h>

#if defined(HAVE_POLL_H)

#endif /* HAVE_POLL_H */

#define FLUSH_TRANSMITTER   1

#if (defined(__CYGWIN__) && !defined(_WIN32)) || defined(__linux__)
    // Cygwin POSIX under Microsoft Windows.and Linux.
    #define DEVICE_NAME "/dev/ttyS%u"
#endif

#include "knx_et.h"
#include "port/port_serial.h"
#include "port/port_timer.h"

/*
__unix__
__linux__
__FreeBSD__
__NetBSD__
__OpenBSD__
__APPLE__
__MACH__

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/param.h>
#if defined(BSD)
    // BSD (DragonFly BSD, FreeBSD, OpenBSD, NetBSD)

#endif
#endif

#if defined(__sun) && defined(__SVR4)
    //  Solaris.

#endif

#if defined(__CYGWIN__) && !defined(_WIN32)
    // Cygwin POSIX under Microsoft Windows. --------------------

#endif

#if defined(_WIN64)
    // Microsoft Windows (64-bit). ------------------------------

#elif defined(_WIN32)
    // Microsoft Windows (32-bit). ------------------------------

#endif

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
    // UNIX-style OS. -------------------------------------------
#include <unistd.h>
#if defined(_POSIX_VERSION)
    //  POSIX compliant

#endif

sys.platform = %r
os.name = %r
serialposix.py version = %s

also add the device name of the serial port and where the
counting starts for the first serial port.
e.g. 'first serial port: /dev/ttyS0'
and with a bit luck you can get this module running...
""" % (sys.platform, os.name, VERSION))
    # no exception, just continue with a brave attempt to build a device name
    # even if the device name is not correct for the platform it has chances
    # to work using a string with the real device name as port parameter.
    def device(portum):
        return '/dev/ttyS%d' % portnum
    def set_special_baudrate(port, baudrate):
        raise SerialException("sorry don't know how to handle non standard baud rate on this platform")
    baudrate_constants = {}
    #~ raise Exception, "this module does not run on this platform, sorry."

*/


#if 0
#define POLLIN  1       /* Set if data to read. */
#define POLLPRI 2       /* Set if urgent data to read. */
#define POLLOUT 4       /* Set if writing data wouldn't block. */
#define POLLERR   8     /* An error occured. */
#define POLLHUP  16     /* Shutdown or close happened. */
#define POLLNVAL 32     /* Invalid file descriptor. */
#endif

/*
Typepunning
-----------
C99 6.7.5
*/

void KnxTlc_OnConnectionTimeoutTimer(void)
{

}

#include "link-layer/uart_bif.h"

#define PORT (1)

int main(void)
{
    uint16_t events;
    uint16_t counter;
    uint16_t count;
    uint16_t errors;
    Port_Serial_PollingResultType pollingResult;
    uint8_t buffer[128];
    uint8_t idx;

    Port_Timer_Setup();

    if (Port_Serial_Init(PORT)) {
        Port_Serial_Flush();
        KnxLL_Init();
        U_Reset_req();

        while (counter < 8) {

                pollingResult = Port_Serial_Poll(FALSE, &events);
                //printf("pollingResult: %u\n", pollingResult);

                if (pollingResult ==  POLLING_ERROR) {
                    KnxEt_Error("read", errno);
                } else if (pollingResult == POLLING_OK) {
                    //printf("Polling events: %04X\n", events);
                    count = Port_Serial_BytesWaiting(&errors);
                    printf("Bytes waiting: %u\n", count);
                    if (!Port_Serial_Read(buffer, count)) {
                        KnxEt_Error("read", errno);
                    } else {
                        KnxEt_DumpHex(buffer, count);
                        for (idx = 0; idx < count; ++idx) {
                            KnxLL_FeedReceiver(buffer[idx]);
                        }
                    }
                } else if (pollingResult == POLLING_TIMEOUT) {
                    printf("Timeout.\n");
                } else {
                }

                counter++;
                usleep(5 * 100);
        }

        usleep(3 * 1000);
        Port_Serial_Deinit();
    } else {
        printf("Could not open serial port.\n");
    }
    //Port_Serial_Write();
/*
    if (Serial_OpenPort(&port, B19200, PARENB, CS8, 1)) {
        Serial_Write(&port,buffer, 1);

        //for (counter = 0; counter < 8; counter++) {
        while (counter < 8) {

                pollingResult = Serial_Poll(&port, FALSE, &events);

                if (pollingResult ==  POLLING_ERROR) {
                    KnxEt_Error("read", errno);
                } else if (pollingResult == POLLING_OK) {
                    counter++;
                    printf("Polling events: %04X\n", events);
                    count = Serial_BytesWaiting(&port, &errors);
                    printf("Bytes waiting: %u\n", count);
                    result = read(port.fd, buffer, count);
                    printf("Read-Result: %02x\n", result);
                    if (result == -1) {
                        KnxEt_Error("read", errno);
                    } else {
                        KnxEt_DumpHex(buffer, count);
                        for (idx = 0; idx < count; ++idx) {
                            KnxLL_FeedReceiver(buffer[idx]);
                        }
                    }
                } else if (pollingResult == POLLING_TIMEOUT) {
                    printf("Timeout.\n");
                } else {
                }
                usleep(5 * 1000);
        }

        Serial_ClosePort(&port);
    }
*/

    return 0;
}

