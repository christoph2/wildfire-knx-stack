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
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/

#if !defined(__KNX_ET_H)
#define __KNX_ET_H

#include "knx_defs.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


typedef void (*KnxEt_CallbackType)(uint8_t ModuleId, uint8_t ApiId, uint8_t ErrorCode);

typedef struct tagKnxEt_ErrorConditionType {
    uint8_t ModuleId;
    uint8_t ApiId;
    uint8_t ErrorCode;
} KnxEt_ErrorConditionType;


void KnxEt_Init(void);
void KnxEt_ReportError(uint8_t ModuleId, uint8_t ApiId, uint8_t ErrorCode);
void KnxEt_Start(void);

void KnxEt_SetCallback(KnxEt_CallbackType callback);
void KnxEt_GetErrorCondition(KnxEt_ErrorConditionType * condition);

void KnxEt_Error(char * function, uint32_t err);
void KnxEt_DumpHex(uint8_t const * frame, uint16_t length);


#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_ET_H */

