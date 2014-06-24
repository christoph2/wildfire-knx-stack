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
#include "knx_disp.h"

/*
** Global variables.
*/
KnxMSG_BufferPtr KnxMSG_ScratchBufferPtr;

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
        KnxMSG_ScratchBufferPtr = KnxMSG_Get(LayerID);

        if (KnxMSG_ScratchBufferPtr != (KnxMSG_BufferPtr)NULL) {
            entry = KnxMSG_ScratchBufferPtr->service - ServiceTable->LayerOffset;

            if (entry < ServiceTable->NumServices) {
                /* todo: _ASSERT() Function-Pointer!=NULL !!! */
                ServiceTable->Functions[entry]();
            } else {
                KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
            }
        }
    } while (KnxMSG_ScratchBufferPtr != (KnxMSG_BufferPtr)NULL);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
