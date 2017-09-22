/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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

#ifndef __PORT_SERIAL_H
#define __PORT_SERIAL_H

#include <stdint.h>

#include "Wildfire_Config.hpp"

#if KNX_TARGET_TYPE == KNX_TARGET_POSIX && KNX_MOCK_DRIVERS == STD_OFF
#include <termios.h>
#endif

#include "knx_et.hpp"
#include "knx_defs.hpp"

/*
**  Service-IDs.
*/
#define AR_SERVICE_SERIAL_INIT                     ((uint8_t)0x00)
#define AR_SERVICE_SERIAL_DEINIT                   ((uint8_t)0x01)

#if 0
#define AR_SERVICE_SERIAL_     ((uint8_t)0x)
#define AR_SERVICE_SERIAL_     ((uint8_t)0x)
#define AR_SERVICE_SERIAL_     ((uint8_t)0x)
#define AR_SERVICE_SERIAL_     ((uint8_t)0x)
#define AR_SERVICE_SERIAL_     ((uint8_t)0x)
#endif

/*
**  Module-Errors.
*/
#define MSG_E_UNINIT                            ((uint8_t)0x01)
#define MSG_E_NULL_PTR                          ((uint8_t)0x02)


typedef struct tagComPort_t {
    uint8_t portNumber;

#if KNX_TARGET_TYPE == KNX_TARGET_POSIX && KNX_MOCK_DRIVERS == STD_OFF
    int fd;
    struct termios savedFlags;
#endif
} Port_Serial_ComPortType;

typedef enum tagPort_Serial_PollingResultType {
    POLLING_OK,
    POLLING_TIMEOUT,
    POLLING_INTERRUPTED,
    POLLING_ERROR
} Port_Serial_PollingResultType;

bool Port_Serial_Init(uint8_t portNumber);
void Port_Serial_Deinit(void);
bool Port_Serial_Write(uint8_t const * buffer, uint32_t byteCount);
void Port_Serial_Flush(void);
void Port_Serial_FlushTransmitter(void);
void Port_Serial_FlushReceiver(void);
Port_Serial_PollingResultType Port_Serial_Poll(bool writing, uint16_t * events);
uint16_t Port_Serial_BytesWaiting(uint32_t * errors);
bool Port_Serial_Read(uint8_t * buffer, uint16_t byteCount);
void Port_Serial_Task(void);

#endif /* __PORT_SERIAL_H*/

