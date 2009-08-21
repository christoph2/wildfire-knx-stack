#include "knx_disp.h"

/*
**
**  Hinweis:    dieses Modul stellt eigentlich ein allgemeingültiges Pattern dar!!!
**  todo:       Konnex-Abhängigkeiten entfernen!!!
**
*/

PMSG_Buffer MSG_ScratchBuffer;   /* MSG_ScratchBuffer */


void KNXDispDispatchLayer(const uint8 LayerID,const KNXLayerServicesType *ServiceTable)
{
    uint8 entry;

    do {
        MSG_ScratchBuffer=MSG_Get(LayerID);

        if (MSG_ScratchBuffer!=(PMSG_Buffer)NULL) {
            entry=MSG_ScratchBuffer->service-ServiceTable->LayerOffset;
            if (entry<ServiceTable->NumServices) {
                /* todo: _ASSERT() Function-Pointer!=NULL !!! */
                ServiceTable->Functions[entry]();
            } else {
                (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);     /* Release invalid Message - todo: Error-Handling!? */
            }
        }
    } while (MSG_ScratchBuffer!=(PMSG_Buffer)NULL);
}
