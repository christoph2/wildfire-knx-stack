/*
*   KONNEX/EIB-Protocol-Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__KNX_WIN_NV_MEM_H)
#define __KNX_WIN_NV_MEM_H

#include "knx_defs.h"
#include "knx_platform.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
**  Global Types.
*/
typedef struct tagNVMem_PersistentArrayType {
    void * fileHandle;
    void * mappingHandle;
    void * mappingAddress;
} NVMem_PersistentArrayType;

typedef struct tagNVMem_Traits {
    wchar_t          name[32];
    unsigned __int32 memSize;
    unsigned __int8  writeSize;
    unsigned __int16 sectorSize;
    unsigned __int16 pageSize;
    unsigned __int8 erasedValue;
    unsigned __int32 cycles;
    NVMem_PersistentArrayType * persistentArray;
} NVMem_Traits;


/*
**  Global Functions.
*/
void NVMem_Create(NVMem_Traits * traits);
void NVMem_Close(NVMem_Traits * traits);
void NVMem_SetData8(NVMem_Traits * traits, unsigned __int32 address, unsigned __int8 data);
void NVMem_GetData8(NVMem_Traits * traits, unsigned __int32 address, unsigned __int8 * data);
void NVMem_SetData16(NVMem_Traits * traits, unsigned __int32 address, unsigned __int16 data);
void NVMem_GetData16(NVMem_Traits * traits, unsigned __int32 address, unsigned __int16 * data);
void NVMem_SetData32(NVMem_Traits * traits, unsigned __int32 address, unsigned __int32 data);
void NVMem_GetData32(NVMem_Traits * traits, unsigned __int32 address, unsigned __int32 * data);
void NvSim_EraseBlock(NVMem_Traits * traits, unsigned __int32 address);


#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_WIN_NV_MEM_H */
