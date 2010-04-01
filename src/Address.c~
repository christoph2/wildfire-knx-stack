/*!
 *
 *  (Adress-)Tabellen-Managment, Programmier-Modus etc.
 *
 */

/*
**  check: wie wärs mit 'IMP_' für Implementations-abhängige Sachen.
*/

/*
**  Hinweis: Im KNX-Handbuch werden Adressen und Tabellen als Resources bezeichnet.
**           KNX-RF verwendet zusätzlich eine Tabelle mit Seriennummern (je 6 Bytes),
**           ausserdem eine erweiterte Assoziations-Tabelle die die Seriennummer-Nummer enthält.
*/

#include "Address.h"

/*
static const uint8 KNX_OBJTYPESIZE[16]={ // Hinweis: auch als 'KNX_OBJ_LEN_TAB' definiert!?
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

/*
**  todo: User- bzw. Project-Supplied, da Hardwareabhängig!!!
*/
boolean ADR_InProgrammingMode(void)    /* todo: als Makro-Equivalent zu 'IMP_InProgrammingMode'!!! */
{
    return TRUE;
}

/*
**
*/

/*
 *
 * todo: automatischer Reggressions-Test:
 *       Adress-Tabelle zufälliger Länge mit zufälligen Adressen füllen,
 *       jeweils den gesamten Adressraum prüfen, Binärsuche mit Linear-
 *       Suche abgleichen!!!
 *
 */
boolean ADR_IsAddressed(Knx_AddressType searched_addr,uint8 *tsap)  /* todo: Broadcasts ('0/0/0') sind immer 'TRUE'!!! */
                                                                    /*       außerdem muss der Ladezustand der Adresstabelle berücksichtigt werden!!! */
                                                                    /* (wobei Broadcast auch ohne Adress-Tabelle funktionieren!!!) */
                                                                    /* todo: adaptive Binärsuche!!! */
{
    /* check: In welcher Byte-Order arbeiten??? */
    
    uint8 len;
    uint16 mid;
    sint16 left,right;
    boolean ack;
    uint16 ca,*ap;

    ack=FALSE;
    *tsap=KNX_INVALID_TSAP;  /* Hinweis: für 'NOTFOUND' ist eigentlich '0xfe' (KNX_UNUSED_TSAP) vorgesehen. */

    if (ADR_IsBroadcastAddress(searched_addr)) {
        return TRUE;
    }
    
    len=ADR_GrATLength();
    
    if (len>(uint8)1) {
        ap=ADR_GrATBasePtr();

        left=0;
        right=len-1;
        do {
            mid=(uint16)(left+right)>>1;
            ca=btohs(*(ap+mid));
            if (searched_addr==ca) {
                *tsap=mid+1;
                ack=TRUE;
                break;
            } else if (searched_addr<ca) {
                right=mid-1;
            } else {
                left=mid+1;
            }
        } while (left<=right);
    } else if (len==(uint8)0) {
        ack=TRUE;   /* passthru of every Frame. */
    } else {        /* len==1. */
        ack=FALSE;
    }
        
    return ack;
}

Knx_AddressType ADR_GetPhysAddr(void)
{
    return btohs(*(uint16*)&APP_AddressTable[1]); /* todo: als Makro!!! */
}

void ADR_SetPhysAddr(Knx_AddressType addr) 
{
    *(uint16*)APP_AddressTable[1]=htobs(addr);    /* todo: Memory-Server verwenden!!! */
}

void ADR_GetSerialNumber(Knx_SerialNumberType serial_number) /* Hinweis: Implementations-Abhängig!!! */
                                                            /* kommt darauf an, kann beim Start ins EEPROM kopiert werden, dann nicht. */
{
/*CopyMem((uint8*)serial_number,(uint8*)DEV_SERIAL_NUM,sizeof(KNX_SerialNumberType)); */
}

boolean ADR_IsOwnPhysicalAddr(Knx_AddressType addr)     /* todo: als Makro!!! */
{
    return (ADR_GetPhysAddr()==addr);
}


/*

todo:   mit MAKEWORD() Words generieren und subtrahieren.

int CompareAddresses(Knx_AddressType a1,Knx_AddressType a2)   // addrcmp
{
    // a1==a2 ==> return 0  - a1<a2 ==> return -1   -   a1>a2   ==> return 1
}
*/
