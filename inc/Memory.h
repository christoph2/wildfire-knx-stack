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
#if !defined(__MEMORY_H)
#define __MEMORY_H

/* #include "target.h" */
#include <Std_Types.h>

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

typedef enum tagMemoryType { mtROM, mtZPRAM, mtRAM, mtEEPROM } MemoryType;

typedef struct tagMemoryControlBlock {
    uint16      OriginalAdress;
    void *      MappedAdress;
    uint16      len;
    MemoryType  mt;
    uint8       access; /* todo: enum. */

} MemoryControlBlock;

void MM_ClearMCBs(void);

uint16  MM_SetByte(uint16 Address, uint8 value);
uint16  MM_GetByte(uint16 Address, uint8 * value);
uint16  MM_MapAddressToTarget(uint16 Address);
uint16  MM_MapAddressFromTarget(uint16 Address);

void    CopyMem(void * d, void * s, uint16 len);
void    FillMem(void * p, uint8 b, uint16 len);
void    ZeroMem(void * p, uint16 len);
boolean CompMem(void * p1, void * p2, uint16 len);

void    CopyRAM(void * d, void * s, uint16 len);
void    FillRAM(void * p, uint8 b, uint16 len);
void    ZeroRAM(void * p, uint16 len);

#define CompRAM CompMem
#define CopyMem CopyRAM

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __MEMORY_H */

