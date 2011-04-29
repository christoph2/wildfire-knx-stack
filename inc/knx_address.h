/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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

#if !defined(__KNX_ADDRESS_H)
#define __KNX_ADDRESS_H

#include "KNXDefs.h"
#include "KNXConv.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

extern const uint8  APP_AddressTable[];
extern const uint8  APP_AssociationTable[];

#define START_USER_EEPROM() /* Dummy, s.o. */
#define END_USER_EEPROM()   /* Dummy, s.o. */

/* *INDENT-OFF* */
#define START_ADDRESS_TABLE(n)      \
const uint8 APP_AddressTable[]={    \
(uint8)((n)+1),

#define END_ADDRESS_TABLE()\
};
/* *INDENT-ON*  */

#define IMPLEMENT_ADDRESS_TABLE(n)      \
    const uint8 APP_AddressTable[] = {        \
        (uint8)((n) + 1),                         \
        IMPLEMENT_ADDRESSES()                   \
    }

#define IMPLEMENT_PHYS_ADDR(p)  HIBYTE((p)), LOBYTE((p)),
#define IMPLEMENT_GROUP_ADDR(g) HIBYTE((g)), LOBYTE((g)),

/* *INDENT-OFF* */
#define START_ASSOCIATION_TABLE(n)      \
const uint8 APP_AssociationTable[]={    \
    (uint8)((n)),

#define END_ASSOCIATION_TABLE() };
/* *INDENT-ON*  */

#define IMPLEMENT_ASSOCIATION(a) HIBYTE((a)), LOBYTE((a)),

/* *INDENT-OFF* */
#define START_COMMOBJ_TABLE(n,r)    \
const uint8 APP_CommObjTab[]={      \
        (uint8)((n)),(uint8)((r)),

#define END_COMMOBJ_TABLE()  };
/* *INDENT-ON*  */

#define IMPLEMENT_COMMOBJ(dp, config, type) (dp), (config), (type),

#define MAKE_PHYS_ADDR(a, b, c)             ((((a) & (uint8)0x0f) << 12) | (((b) & (uint8)0x0f) << 8) | ((c) & (uint8)0xff))
#define MAKE_GROUP_ADDR(a, b, c)            ((((a) & (uint8)0x1f) << 11) | (((b) & (uint8)0x07) << 8) | ((c) & (uint8)0xff))

boolean KnxADR_InProgrammingMode(void);

#define KnxADR_GrATLength()                 ((uint8)APP_AddressTable[0])
#define KnxADR_GrOATLength()                ((uint8)APP_AssociationTable[0])

#define KnxADR_GrATBasePtr()                (uint16 *)((uint8 *)&APP_AddressTable + 3)
#define KnxADR_GrOATBasePtr()               (uint16 *)((uint8 *)&APP_AssociationTable + 1)

#define KnxADR_GetGroupAddress(n)           (btohs(*(uint16 *)(KnxADR_GrATBasePtr() + ((n) - 1))))
#define KnxADR_GetAssoc(n)                  (btohs(*(uint16 *)(KnxADR_GrOATBasePtr() + (n))))

boolean KnxADR_IsAddressed(Knx_AddressType searched_addr, uint8 * tsap);
boolean KnxADR_IsOwnPhysicalAddr(Knx_AddressType addr);

#define KnxADR_IsBroadcastAddress(addr)     ((addr) == (Knx_AddressType)0x0000)

Knx_AddressType KnxADR_GetPhysAddr(void);
void            KnxADR_SetPhysAddr(Knx_AddressType addr);
void            KnxADR_GetSerialNumber(Knx_SerialNumberType serial_number);

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_ADDRESS_H */
