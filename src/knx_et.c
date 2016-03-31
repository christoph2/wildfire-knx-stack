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

#include <stdio.h>
#include <string.h>

#include "knx_et.h"
#include "knx_ffi.h"

/** @file KNX Error Tracker
 *
 *  The Error Tracker ships as an example and should adjusted to your needs.
 *
 */

void KnxEt_Init(void)
{

}

void KnxEt_ReportError(uint8_t ModuleId, uint8_t ApiId, uint8_t ErrorCode)
{
    KNX_API_ERROR(ModuleId, ApiId, ErrorCode);

    printf("KnxEt_ReportError -- module: %u api: %u error: %u\n", ModuleId, ApiId, ErrorCode);
}

void KnxEt_Start(void)
{

}

void KnxEt_DumpHex(uint8_t const * frame, uint16_t length)
{
    uint8_t idx;

    for (idx = 0; idx < length; ++idx) {
        printf("%02x ", frame[idx]);
    }
    printf("\n");
}

void KnxEt_Error(char * function, uint32_t err)
{
    printf("%s failed with error %u: %s\n", function, err, strerror(err));
}
