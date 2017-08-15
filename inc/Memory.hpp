/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__MEMORY_HPP)
#define __MEMORY_HPP

/* #include "target.hpp" */
#include "knx_types.hpp"

namespace knx {

enum class MemoryType { 
    mtROM, 
    mtZPRAM, 
    mtRAM, 
    mtEEPROM 
};

struct MemoryControlBlock {
    uint16_t OriginalAdress;
    void * MappedAdress;
    uint16_t len;
    MemoryType mt;
    uint8_t access; /* todo: enum. */

};

void MM_ClearMCBs();

uint16_t MM_SetByte(uint16_t Address, uint8_t value);
uint16_t MM_GetByte(uint16_t Address, uint8_t * value);
uint16_t MM_MapAddressToTarget(uint16_t Address);
uint16_t MM_MapAddressFromTarget(uint16_t Address);

void CopyMem(void * d, void * s, uint16_t len);
void FillMem(void * p, uint8_t b, uint16_t len);
void ZeroMem(void * p, uint16_t len);
bool CompMem(void * p1, void * p2, uint16_t len);

void CopyRAM(void * d, void * s, uint16_t len);
void FillRAM(void * p, uint8_t b, uint16_t len);
void ZeroRAM(void * p, uint16_t len);


#define CompRAM CompMem
#define CopyMem CopyRAM

} // namespace knx

#endif  /* __MEMORY_HPP */
