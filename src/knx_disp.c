/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
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
#include "knx_disp.h"

/*
** Global variables.
*/
KnxMsg_Buffer * KnxMsg_ScratchBufferPtr;

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxDisp_DispatchLayer(const uint8_t LayerID,
    CONSTP2CONST(Knx_LayerServicesType, AUTOMATIC, KSTACK_APPL_DATA) ServiceTable
);
#else
void KnxDisp_DispatchLayer(const uint8_t LayerID, const Knx_LayerServicesType * ServiceTable)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t entry;

    do {
        KnxMsg_ScratchBufferPtr = KnxMsg_Get(LayerID);

        if (KnxMsg_ScratchBufferPtr != (KnxMsg_Buffer *)NULL) {
            entry = KnxMsg_ScratchBufferPtr->service - ServiceTable->LayerOffset;

            if (entry < ServiceTable->NumServices) {
                /* todo: _ASSERT() Function-Pointer!=NULL !!! */
                ServiceTable->Functions[entry]();
            } else {
                (void)KnxMsg_ReleaseBuffer(KnxMsg_ScratchBufferPtr);
            }
        }
    } while (KnxMsg_ScratchBufferPtr != (KnxMsg_Buffer *)NULL);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

