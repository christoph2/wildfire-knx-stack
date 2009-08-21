/*
**  todo: Benutzer-Callbacks f. Memory-Read()/-Write(), um bspw. BCU-RAM emulieren zu können.
**      Hinweis:        'MEM_'-Prefixe sind besser!!!
**                              Die RAM-Routinen sind im Gegensatz zu den EEPROM-Routinen Plattform-unabhängig!!!
**
**  todo: aus Effizienz-Gründen 'SetByte' implementieren.
*/

#include "Memory.h"

/*
Memory-Types (Absolute Code/Data Allocation Record)
---------------------------------------------------
1   Zero page RAM
2   RAM
3   EEPROM
*/


#define NUM_MCBs        1

uint8 UserLowRAM[0x23];

MemoryControlBlock MCBs[NUM_MCBs]=
{
        {0x00BD,&UserLowRAM,0x23,mtRAM,0x00},
};

void MM_ClearMCBs(void)
{
        ZeroRAM(MCBs,sizeof(MemoryControlBlock)*NUM_MCBs);
}

/*
** EIB-Adresse in Target-Adresse übersetzen.
**
**      check: Fehler-Behandlung bei den folgenden beiden Funktionen!!!
*/
uint16 MM_MapAddressToTarget(uint16 Address)
{
        return  0xffff;
}

/*
** Target-Adresse in EIB-Adresse übersetzen.
*/
uint16 MM_MapAddressFromTarget(uint16 Address)
{
        return  0xffff;
}

/* check: Überflüssig?! */
/*
int MM_SetByte(uint16 Address,uint8 value)
{
        return 0;
}

int MM_GetByte(uint16 Address,uint8 *value)
{
        return 1;
}
*/

/*
**      todo: die folgenden Funktionen sollten in ???RAM umbenannt werden.
**                      ausserdem Funktionen implementieren, die RAM und EEPROM transparent handeln,
**                      wobei 'Comp' lediglich ein Synonym darstellt.
*/

/*
void FillMem(void *p,uint8 b,uint16 len)
{
    uint8 *bp=(uint8*)p;
    
    while (len--) {
        *bp++=b;
    }
}

void CopyMem(void *d,void *s,uint16 len)
{
    uint8 *bd=(uint8*)d;
    uint8 *bs=(uint8*)s;
    
    while (len--) {
        *bd++=*bs++;
    }
}


void ZeroMem(void *p,uint16 len)
{
    uint8 *bp=(uint8*)p;
    
    while (len--) {
        *bp++=(uint8)0;
    }
}
*/

boolean CompMem(void *p1,void *p2,uint16 len)
{
    uint8 *bp1=(uint8*)p1;
    uint8 *bp2=(uint8*)p2;

    while (len--) {
        if ((*bp1++)!=(*bp2++)) {
            return FALSE;
        }
    }
    return TRUE;
}

void FillRAM(void *p,uint8 b,uint16 len)
{
    uint8 *bp=(uint8*)p;
    
    while (len--) {
        *bp++=b;
    }
}

void CopyRAM(void *d,void *s,uint16 len)
{
    uint8 *bd=(uint8*)d;
    uint8 *bs=(uint8*)s;
    
    while (len--) {
        *bd++=*bs++;
    }
}


void ZeroRAM(void *p,uint16 len)
{
    uint8 *bp=(uint8*)p;
    
    while (len--) {
        *bp++=(uint8)0;
    }
}
