/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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
#include "knx_conversion.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
**  Global variables.
*/
extern const uint8_t  APP_AddressTable[];
extern const uint8_t  APP_AssociationTable[];

/*
**  Global function-like macros.
*/
#define START_USER_EEPROM() /* Dummy, s.o. */
#define END_USER_EEPROM()   /* Dummy, s.o. */

/* *INDENT-OFF* */
#define START_ADDRESS_TABLE(n)      \
const uint8_t APP_AddressTable[]={    \
(uint8_t)((n)+1),

#define END_ADDRESS_TABLE()\
};
/* *INDENT-ON*  */

#define IMPLEMENT_ADDRESS_TABLE(n)     \
    const uint8_t APP_AddressTable[] = { \
        (uint8_t)((n) + 1),              \
        IMPLEMENT_ADDRESSES()          \
    }


#define IMPLEMENT_PHYS_ADDR(p)  HIBYTE((p)), LOBYTE((p)),
#define IMPLEMENT_GROUP_ADDR(g) HIBYTE((g)), LOBYTE((g)),

/* *INDENT-OFF* */
#define START_ASSOCIATION_TABLE(n)      \
const uint8_t APP_AssociationTable[]={    \
    (uint8_t)((n)),

#define END_ASSOCIATION_TABLE() };
/* *INDENT-ON*  */

#define IMPLEMENT_ASSOCIATION(a) HIBYTE((a)), LOBYTE((a)),

/* *INDENT-OFF* */
#define START_COMMOBJ_TABLE(n,r)    \
const uint8_t APP_CommObjTab[]={      \
        (uint8_t)((n)),(uint8_t)((r)),

#define END_COMMOBJ_TABLE()  };
/* *INDENT-ON*  */

#define IMPLEMENT_COMMOBJ(dp, config, type) (dp), (config), (type),

#define MAKE_PHYS_ADDR(a, b, c)             ((((a) & (uint8_t)0x0f) << 12) | (((b) & (uint8_t)0x0f) << 8) | ((c) & (uint8_t)0xff))
#define MAKE_GROUP_ADDR(a, b, c)            ((((a) & (uint8_t)0x1f) << 11) | (((b) & (uint8_t)0x07) << 8) | ((c) & (uint8_t)0xff))

#define KnxADR_GrATLength()                 ((uint8_t)APP_AddressTable[0])
#define KnxADR_GrOATLength()                ((uint8_t)APP_AssociationTable[0])

#define KnxADR_GrATBasePtr()                (uint16_t *)((uint8_t *)&APP_AddressTable + 3)
#define KnxADR_GrOATBasePtr()               (uint16_t *)((uint8_t *)&APP_AssociationTable + 1)

#define KnxADR_GetGroupAddress(n)           (btohs(*(uint16_t *)(KnxADR_GrATBasePtr() + ((n) - 1))))
#define KnxADR_GetAssoc(n)                  (btohs(*(uint16_t *)(KnxADR_GrOATBasePtr() + (n))))

#define KnxADR_IsBroadcastAddress(addr)     ((addr) == (Knx_AddressType)0x0000)

/*
**  Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE)      KnxADR_InProgrammingMode(void);
FUNC(boolean, KSTACK_CODE)      KnxADR_IsAddressed(Knx_AddressType searched_addr,
                                                   P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) tsap
                                                   );
FUNC(boolean, KSTACK_CODE)      KnxADR_IsOwnPhysicalAddr(Knx_AddressType addr);
FUNC(Knx_AddressType, KSTACK_CODE)  KnxADR_GetPhysAddr(void);
FUNC(void, KSTACK_CODE)         KnxADR_SetPhysAddr(Knx_AddressType addr);
FUNC(void, KSTACK_CODE)         KnxADR_GetSerialNumber(Knx_SerialNumberType serial_number);
#else
boolean         KnxADR_InProgrammingMode(void);
boolean         KnxADR_IsAddressed(Knx_AddressType searched_addr, uint8_t * tsap);
boolean         KnxADR_IsOwnPhysicalAddr(Knx_AddressType addr);
Knx_AddressType KnxADR_GetPhysAddr(void);
void            KnxADR_SetPhysAddr(Knx_AddressType addr);
void            KnxADR_GetSerialNumber(Knx_SerialNumberType serial_number);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_ADDRESS_H */
