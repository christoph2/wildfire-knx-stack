/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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
#include "knx_disp.hpp"

using namespace knx;

/*
** Global variables.
*/
KnxMsg_Buffer * KnxMsg_ScratchBufferPtr = nullptr;

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

/*
** Global functions.
*/
void KnxDisp_DispatchLayer(uint8_t layerID, const Knx_LayerServicesType & serviceTable)
{
    uint8_t entry;

    do {
        knx::KnxMsg_ScratchBufferPtr = KnxMsg_Get(layerID);

        if (knx::KnxMsg_ScratchBufferPtr != nullptr) {
            entry = static_cast<const uint8_t>(knx::KnxMsg_ScratchBufferPtr->service) - serviceTable.LayerOffset;

            if (entry < serviceTable.NumServices) {
                /* todo: _ASSERT() Function-Pointer!=nullptr !!! */
                serviceTable.Functions[entry]();
            } else {
                (void)KnxMsg_ReleaseBuffer(knx::KnxMsg_ScratchBufferPtr);
            }
        }
    } while (knx::KnxMsg_ScratchBufferPtr != nullptr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

