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

#include "knx_address.h"

/*
   static const uint8_t KNX_OBJTYPESIZE[16]={ // Hinweis: auch als 'KNX_OBJ_LEN_TAB' definiert!?
    1,   // UINT1
    1,   // UINT2
    1,   // UINT3
    1,   // UINT4
    1,   // UINT5
    1,   // UINT6
    1,   // UINT7
    1,   // UINT8
    2,   // UINT16
    3,   // BYTE3
    4,   // FLOAT
    6,   // DATA6
    8,   // DOUBLE
    10,  // DATA10
    14,  // MAXDATA
    14   // VARDATA     - wichtiger Hinweis: im KNX-Handbuch: Interface Object Reference (4-14 octets).
   };
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
/*
**	Global functions.
*/

/*
**  todo: this Fkt. is hardware-dependent !!!
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxADR_InProgrammingMode(void)
#else
boolean KnxADR_InProgrammingMode(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return TRUE;
}


/*
**
*/

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE)  KnxADR_IsAddressed(Knx_AddressType searched_addr,
                                               P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) tsap
                                               )
#else
boolean KnxADR_IsAddressed(Knx_AddressType searched_addr, uint8_t * tsap)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   len;
    uint16_t  mid;
    sint16  left, right;
    boolean ack;
    uint16_t  ca, * ap;

    ack    = FALSE;
    *tsap  = KNX_INVALID_TSAP;

    if (KnxADR_IsBroadcastAddress(searched_addr)) {
        return TRUE;
    }

    len = KnxADR_GrATLength();

    if (len > (uint8_t)1) {
        ap = KnxADR_GrATBasePtr();

        left   = (sint16)0;
        right  = len - (sint16)1;

        do {
            mid    = (uint16_t)(left + right) >> 1;
            ca     = btohs(*(ap + mid));

            if (searched_addr == ca) {
                *tsap  = mid + (uint16_t)1;
                ack    = TRUE;
                break;  /*!MISRA 1998 Rule 58 ('non-switch break used') Exception: the alternative would be a 'goto' statement ;-) */
            } else if (searched_addr < ca) {
                right = (sint16)(mid - (uint16_t)1);
            } else {
                left = (sint16)(mid + (uint16_t)1);
            }
        } while (left <= right);
    } else if (len == (uint8_t)0) {
        ack = TRUE; /* passthru of every Frame. */
    } else {        /* len==1. */
        ack = FALSE;
    }

    return ack;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_AddressType, KSTACK_CODE) KnxADR_GetPhysAddr(void)
#else
Knx_AddressType KnxADR_GetPhysAddr(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return btohs(*(uint16_t *)&APP_AddressTable[1]);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxADR_SetPhysAddr(Knx_AddressType addr)
#else
void KnxADR_SetPhysAddr(Knx_AddressType addr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    *(uint16_t *)APP_AddressTable[1] = htobs(addr);    /* todo: use Memory-Server!!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxADR_GetSerialNumber(Knx_SerialNumberType serial_number)
#else
void KnxADR_GetSerialNumber(Knx_SerialNumberType serial_number)
#endif /* KSTACK_MEMORY_MAPPING */
{
/*CopyMem((uint8_t*)serial_number,(uint8_t*)DEV_SERIAL_NUM,sizeof(KNX_SerialNumberType)); */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxADR_IsOwnPhysicalAddr(Knx_AddressType addr)   /* todo: Macro!!! */
#else
boolean KnxADR_IsOwnPhysicalAddr(Knx_AddressType addr)                      /* todo: Macro!!! */
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxADR_GetPhysAddr() == addr;
}


/*
   int CompareAddresses(Knx_AddressType a1,Knx_AddressType a2)   // addrcmp
   {
    // a1==a2 ==> return 0  - a1<a2 ==> return -1   -   a1>a2   ==> return 1
   }
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
