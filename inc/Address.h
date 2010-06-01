/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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

#if !defined(__ADDRESS_H)
#define __ADDRESS_H

#include "KNXDefs.h"
#include "KNXConv.h"

extern const uint8 APP_AddressTable[];
extern const uint8 APP_AssociationTable[];

#define START_USER_EEPROM()     /* Dummy, s.o. */
#define END_USER_EEPROM()       /* Dummy, s.o. */

#define START_ADDRESS_TABLE(n)      \
const uint8 APP_AddressTable[]={    \
(uint8)((n)+1),

#define END_ADDRESS_TABLE()\
};

#define IMPLEMENT_ADDRESS_TABLE(n)      \
const uint8 APP_AddressTable[]={        \
(uint8)((n)+1),                         \
IMPLEMENT_ADDRESSES()                   \
}


#define IMPLEMENT_PHYS_ADDR(p)      HIBYTE((p)),LOBYTE((p)),
#define IMPLEMENT_GROUP_ADDR(g)     HIBYTE((g)),LOBYTE((g)),


#define START_ASSOCIATION_TABLE(n)      \
const uint8 APP_AssociationTable[]={    \
    (uint8)((n)),

#define END_ASSOCIATION_TABLE() };  

#define IMPLEMENT_ASSOCIATION(a)     HIBYTE((a)),LOBYTE((a)),

#define START_COMMOBJ_TABLE(n,r)    \
const uint8 APP_CommObjTab[]={      \
        (uint8)((n)),(uint8)((r)),

#define END_COMMOBJ_TABLE()  };

#define IMPLEMENT_COMMOBJ(dp,config,type)    (dp),(config),(type),


#define MAKE_PHYS_ADDR(a,b,c)   ((((a) & (uint8)0x0f) << 12) | (((b) & (uint8)0x0f) << 8) | ((c) & (uint8)0xff))
#define MAKE_GROUP_ADDR(a,b,c)  ((((a) & (uint8)0x1f) << 11) | (((b) & (uint8)0x07) << 8) | ((c) & (uint8)0xff))

boolean ADR_InProgrammingMode(void);

#define ADR_GrATLength()                ((uint8)APP_AddressTable[0])
#define ADR_GrOATLength()               ((uint8)APP_AssociationTable[0])


#define ADR_GrATBasePtr()               (uint16*)((uint8*)&APP_AddressTable+3)
#define ADR_GrOATBasePtr()              (uint16*)((uint8*)&APP_AssociationTable+1)

#define ADR_GetGroupAddress(n)          (btohs(*(uint16*)(ADR_GrATBasePtr()+((n)-1))))
#define ADR_GetAssoc(n)                 (btohs(*(uint16*)(ADR_GrOATBasePtr()+(n))))

boolean ADR_IsAddressed(Knx_AddressType searched_addr,uint8 *tsap);
boolean ADR_IsOwnPhysicalAddr(Knx_AddressType addr);
#define ADR_IsBroadcastAddress(addr)    ((addr)==(Knx_AddressType)0x0000)

Knx_AddressType ADR_GetPhysAddr(void);
void ADR_SetPhysAddr(Knx_AddressType addr);
void ADR_GetSerialNumber(Knx_SerialNumberType serial_number);

#endif  /* __ADDRESS_H */
