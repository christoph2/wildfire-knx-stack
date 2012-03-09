/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__KNX_TYPES_H)
#define __KNX_TYPES_H

#include "Std_Types.h"
#include "knx_imi.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


/*
** Global types.
*/
typedef uint16  Knx_AddressType;
typedef uint8   Knx_SerialNumberType[6];

typedef struct tagKnx_PollGroupSettingType {
    uint16  group_addr;
    uint8   slot_no;
} Knx_PollGroupSettingType;

typedef struct tagKnx_CommObjDescriptorType {
    uint8   DataPtr;
    uint8   Config;
    uint8   Type;
} Knx_CommObjDescriptorType;

typedef enum tagKnx_DafType {
    atINDIVIDUAL   = ((uint8) 0x00),
    atMULTICAST    = ((uint8) 0x80)
} Knx_DafType;

typedef enum tagKnx_FrameTypeType {
    ftExtended = ((uint8) 0x00),
    ftStandard = ((uint8) 0x80),
    ftPolling  = ((uint8) 0xC0)
} Knx_FrameTypeType;

typedef enum tagKnx_ObjectType {
    otUINT1, otUINT2, otUINT3, otUINT4, otUINT5, otUINT6, otUINT7,
    otUINT8, otUINT16, otBYTE3, otFLOAT, otDATA6, otDOUBLE, otDATA10, otMAXDATA, otVARDATA
} Knx_ObjectType;    /* Type-Byte / Object-Descriptor. */

typedef uint8 Knx_PriorityType;

/*
**  START: LAYER-Dispatching-Functions.
*/
typedef void (*Knx_LayerServiceFunctionType)(void);

typedef struct tagKnx_ServiceEntryType {
    Knx_ServiceTypeType             Service;
    Knx_LayerServiceFunctionType    Function;
} Knx_ServiceEntryType;

typedef struct tagKnx_LayerServicesType {
    uint8                                   LayerOffset;
    uint8                                   NumServices;
    const Knx_LayerServiceFunctionType *    Functions;
} Knx_LayerServicesType;
/*
**  END: LAYER-Dispatching-Functions.
*/

/*
   typedef enum tagKNX_PriorityType {
    prSystem,prNormal,
    prUrgent,prLow
   } KNX_PriorityType;
 */

typedef uint8 Knx_HopCountType;

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TYPES_H */

