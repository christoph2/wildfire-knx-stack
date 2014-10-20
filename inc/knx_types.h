/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
*                                       cpu12.gems@googlemail.com>
*
*   All Rights Reserved
*
*  This program is free softwKNXe; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free SoftwKNXe Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#if !defined(__KNX_TYPES_H)
#define __KNX_TYPES_H

#include <stdint.h>
#include "knx_imi.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
 *
 * Basic Types.
 *
 */
#if __STDC_VERSION__ >= 199901L
typedef _Bool boolean;
#else
typedef unsigned char boolean;
#endif

typedef float float32;
typedef double float64;

#if !defined(NULL)
    #define NULL    ((void *) 0)
#endif

#if !defined(TRUE)
    #define TRUE    (1u)
#endif

#if !defined(FALSE)
    #define FALSE   (0u)
#endif

#define STD_ON TRUE
#define STD_OFF FALSE

/*
** Global types.
*/
typedef uint16_t  Knx_AddressType;
typedef uint8_t   Knx_SerialNumberType[6];

typedef enum tagKnx_StatusType {
    KNX_E_OK,
    KNX_E_NOT_OK
} Knx_StatusType;

typedef struct tagKnx_PollGroupSettingType {
    uint16_t  group_addr;
    uint8_t   slot_no;
} Knx_PollGroupSettingType;

typedef struct tagKnx_CommObjDescriptorType {
    uint8_t   DataPtr;
    uint8_t   Config;
    uint8_t   Type;
} Knx_CommObjDescriptorType;

typedef enum tagKnx_DafType {
    atINDIVIDUAL   = ((uint8_t) 0x00),
    atMULTICAST    = ((uint8_t) 0x80)
} Knx_DafType;

typedef enum tagKnx_FrameTypeType {
    ftExtended = ((uint8_t) 0x00),
    ftStandard = ((uint8_t) 0x80),
    ftPolling  = ((uint8_t) 0xC0)
} Knx_FrameTypeType;

typedef enum tagKnx_ObjectType {
    otUINT1, otUINT2, otUINT3, otUINT4, otUINT5, otUINT6, otUINT7,
    otUINT8, otUINT16, otBYTE3, otFLOAT, otDATA6, otDOUBLE, otDATA10, otMAXDATA, otVARDATA
} Knx_ObjectType;    /* Type-Byte / Object-Descriptor. */

typedef uint8_t Knx_PriorityType;

/*
**  START: LAYER-Dispatching-Functions.
*/
typedef void (*Knx_LayerServiceFunctionType)(void);

typedef struct tagKnx_ServiceEntryType {
    Knx_ServiceTypeType             Service;
    Knx_LayerServiceFunctionType    Function;
} Knx_ServiceEntryType;

typedef struct tagKnx_LayerServicesType {
    uint8_t                                   LayerOffset;
    uint8_t                                   NumServices;
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

typedef uint8_t Knx_HopCountType;

/*
**  Global Types.
*/
typedef enum tagKnxModule_StateType {
    KNX_MODULE_UNINIT,
    KNX_MODULE_READY
} KnxModule_StateType;

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TYPES_H */

