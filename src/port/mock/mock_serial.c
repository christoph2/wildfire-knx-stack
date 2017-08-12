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

#include "knx_types.h"
#include <stdint.h>

typedef struct tagPort_Serial_PollingResultType {
  int dummy;
} Port_Serial_PollingResultType;

typedef void (*WriterCalloutType)(uint8_t const * const frame, uint32_t length);

static WriterCalloutType Serial_WriterCallout = 0UL;

/*
**
** Global Functions.
**
*/

void Port_Serial_SetCallback(WriterCalloutType * cb)
{
    Serial_WriterCallout = *cb;
}

_Bool Port_Serial_Init(uint8_t portNumber)
{
}

void Port_Serial_Deinit(void)
{
}

_Bool Port_Serial_Write(uint8_t const * buffer, uint32_t byteCount)
{
    if (Serial_WriterCallout) {
        Serial_WriterCallout(buffer, byteCount);
        return FALSE;
    }
    return TRUE;
}

void Port_Serial_Flush(void)
{
}

void Port_Serial_FlushTransmitter(void)
{
}

void Port_Serial_FlushReceiver(void)
{
}


Port_Serial_PollingResultType Port_Serial_Poll(_Bool writing, uint16_t * events)
{

}

_Bool Port_Serial_Read(uint8_t * buffer, uint16_t byteCount)
{
}

uint16_t Port_Serial_BytesWaiting(uint32_t * errors)
{
}

void Port_Serial_Task(void)
{
}

