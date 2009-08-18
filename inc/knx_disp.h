#if !defined(__KNX_DISP_H)
#define __KNX_DISP_H

#include "KNXDefs.h"
#include "Messaging.h"

extern PMSG_Buffer MSG_ScratchBuffer;    /* todo: make 'static' !!! */

void KNXDispDispatchLayer(const uint8 LayerID,const KNXLayerServicesType *ServiceTable);

#endif /* __KNX_DISP_H */
