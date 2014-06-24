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

#include "Memory.h"

/*
   Memory-Types (Absolute Code/Data Allocation Record)
   ---------------------------------------------------
   1   Zero page RAM
   2   RAM
   3   EEPROM
 */

#define NUM_MCBs 1  /* TODO: config. */

/*
** Global variables.
*/
uint8_t UserLowRAM[0x23];

MemoryControlBlock MCBs[NUM_MCBs] = {
    {(uint16_t)0x00BDU, &UserLowRAM, (uint8_t)0x23, mtRAM, (uint8_t)0x00},
};

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) MM_ClearMCBs(void)
#else
void MM_ClearMCBs(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    ZeroRAM(MCBs, sizeof(MemoryControlBlock) * NUM_MCBs);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint16_t, KSTACK_CODE) MM_MapAddressToTarget(uint16_t Address)
#else
uint16_t MM_MapAddressToTarget(uint16_t Address)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return (uint16_t)0xffffU;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint16_t, KSTACK_CODE) MM_MapAddressFromTarget(uint16_t Address)
#else
uint16_t MM_MapAddressFromTarget(uint16_t Address)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return (uint16_t)0xffffU;
}


/*
   int MM_SetByte(uint16_t Address,uint8_t value)
   {
        return 0;
   }

   int MM_GetByte(uint16_t Address,uint8_t *value)
   {
        return 1;
   }
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) CompMem(P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) p1, P2VAR(void, AUTOMATIC,
                                                                                      KSTACK_APPL_DATA) p2, uint16_t len)
#else
boolean CompMem(void * p1, void * p2, uint16_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t * bp1    = (uint8_t *)p1;
    uint8_t * bp2    = (uint8_t *)p2;

    while (len--) {
        if ((*bp1++) != (*bp2++)) {
            return FALSE;
        }
    }

    return TRUE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) FillRAM(P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) p, uint8_t b, uint16_t len)
#else
void FillRAM(void * p, uint8_t b, uint16_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t * bp = (uint8_t *)p;

    while (len--) {
        *bp++ = b;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) CopyRAM(P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) d, P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) s, uint16_t len)
#else
void CopyRAM(void * d, void * s, uint16_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t * bd = (uint8_t *)d;
    uint8_t * bs = (uint8_t *)s;

    while (len--) {
        *bd++ = *bs++;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) ZeroRAM(P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) p, uint16_t len)
#else
void ZeroRAM(void * p, uint16_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t * bp = (uint8_t *)p;

    while (len--) {
        *bp++ = (uint8_t)0;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
