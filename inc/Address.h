
/*
**  todo: 'END_USER_EEPROM' (dananch folgen auf dem Bus 'unsichbare' Bereiche: Seriennummer,Load-/Run-Controls,Pollgroup etc.).
**                                          Hinweis: Seriennummer ist eine 'IMP_'!!! bzw. 'USR_'-Funktion!!!
*/

/*  todo: 'Vernünftigere' Namen für die Tabellen finden!!! */
/*  Hinweis: der Tabellen-'Kram' gehört nach 'Application'!!! */

#if !defined(__ADDRESS_H)
#define __ADDRESS_H

#include "KNXDefs.h"
#include "KNXConv.h"
#include "Memory.h"

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


/* check: oder besser 'PHYS_(GROUP_)ADDR_TO_WORD' ??? */
#define MAKE_PHYS_ADDR(a,b,c)   ((((a) & 0x0f) << 12) | (((b) & 0x0f) << 8) | ((c) & 0xff))
#define MAKE_GROUP_ADDR(a,b,c)  ((((a) & 0x1f) << 11) | (((b) & 0x07) << 8) | ((c) & 0xff))

boolean ADR_InProgrammingMode(void);

#define ADR_GrATLength()                ((uint8)APP_AddressTable[0])
#define ADR_GrOATLength()               ((uint8)APP_AssociationTable[0])

/* 
** check: zwischen 'Base' und 'Entry' Pointer unterscheiden (Base um die Tabellen 'static' zu machen) ???
*/
#define ADR_GrATBasePtr()               (uint16*)((uint8*)&APP_AddressTable+3)
#define ADR_GrOATBasePtr()              (uint16*)((uint8*)&APP_AssociationTable+1)

#define ADR_GetGroupAddress(n)          (btohs(*(uint16*)(ADR_GrATBasePtr()+((n)-1))))  /* check: -1 ??? */
#define ADR_GetAssoc(n)                 (btohs(*(uint16*)(ADR_GrOATBasePtr()+(n))))  /* todo: Sinnvoller Name!!! */

boolean ADR_IsAddressed(Knx_AddressType searched_addr,uint8 *tsap);
boolean ADR_IsOwnPhysicalAddr(Knx_AddressType addr);
#define ADR_IsBroadcastAddress(addr)    ((addr)==(Knx_AddressType)0x0000)

Knx_AddressType ADR_GetPhysAddr(void);
void ADR_SetPhysAddr(Knx_AddressType addr);
void ADR_GetSerialNumber(Knx_SerialNumberType serial_number);

#endif  /* __ADDRESS_H */
