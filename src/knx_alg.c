/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
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

/*
**  Application-Layer / Group-Oriented.
*/

#include "knx_alg.h"

/*
**	Local function prototypes.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, KSTACK_CODE) Disp_A_DataGroupReq(void), Disp_A_PollDataReq(void), Disp_T_PollDataCon(void);
STATIC  FUNC(void, KSTACK_CODE) Disp_T_DataGroupInd(void), Disp_T_DataGroupCon(void);


#else
void    Disp_A_DataGroupReq(void), Disp_A_PollDataReq(void), Disp_T_PollDataCon(void);
void    Disp_T_DataGroupInd(void), Disp_T_DataGroupCon(void);


#endif /* KSTACK_MEMORY_MAPPING */

STATIC const Knx_LayerServiceFunctionType ALG_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      T_DATA_GROUP_IND        */ Disp_T_DataGroupInd,
/*      T_DATA_GROUP_CON        */ Disp_T_DataGroupCon,
/*      T_POLL_DATA_CON         */ Disp_T_PollDataCon,
/*      A_DATA_GROUP_REQ        */ Disp_A_DataGroupReq,
/*      A_POLL_DATA_REQ         */ Disp_A_PollDataReq,
/*      ====================================================*/
};

STATIC const Knx_LayerServicesType ALG_ServiceTable[] = {
    {KNX_ALG_SERVICES, 5, ALG_Services}
};

#define SAP_CB_OK       ((uint8_t)0)      /* Ok (Return messagebuffer to system). */
#define SAP_CB_BREAK    ((uint8_t)1)      /* Break and free buffer */
#define SAP_CB_GETBUF   ((uint8_t)2)      /* Hold buffer and break */
#define SAP_CB_SEND     ((uint8_t)3)      /* Send message and break */

const uint8_t KNX_AL_SHORT_DATA_MASK[] = {
    (uint8_t)(1 << 1) - 1, (uint8_t)(1 << 2) - 1, (uint8_t)(1 << 3) - 1, (uint8_t)(1 << 4) - 1,
    (uint8_t)(1 << 5) - 1, (uint8_t)(1 << 6) - 1, (uint8_t)(1 << 7) - 1, (uint8_t)(1 << 8) - 1
};

const uint8_t KNX_OBJ_LEN_TAB[16] = {
    (uint8_t)1, (uint8_t)1,  (uint8_t)1,  (uint8_t)1,  (uint8_t)1,  (uint8_t)1,
    (uint8_t)1, (uint8_t)1,  (uint8_t)2,  (uint8_t)3,  (uint8_t)4,  (uint8_t)6,
    (uint8_t)8, (uint8_t)10, (uint8_t)14, (uint8_t)0
};  /* 'Interface Object Reference' not supported yet. */

/* *** OUTGOING *** */
/*
   AL_CallbackHandler (Standard Callback Handler)
   ---
    1. search object with status "transmit request" with index from 0 .. max, if no object found -> return(SAP_CB_BREAK)
    2.  check telegram rate limitation (based on timer)
    3. check object properties (transmit enable, communication enable), on error set object (RAM flags) to transmiterror
    4. set object (RAM flags) to transmitting
    5. set buspriority in CTRL byte of frame
    6. value read : -> data request : set object number to read,set APCI
    7. value write : -> no data request : set object number to write, copy object data, set APCI
    8. return (SAP_CB_SEND)
 */
/*
   Depend on the "return values" of handler the following things will be done :

    - SAP_CB_SEND

        1. save 'on' to SAP
        2. get 'cn' (on error - cn not exists - negative T_Group_Data_conf)
        3. get 'sn '
        4. check 'sn' <> 'on' (on error - this is not a sending object - negative T_Group_Data_conf [AL : set errorflags in object] )
        5. force T_Group_Datat_requ (per scheduler cycle only _one_ request)

    - Other -> clears message
 */

/* *** INCOMING *** */
/*
   AL_CallbackHandler (Standard Callback Handler)
   ---

   Incoming T_Group_Data_conf :

    1. Set object (ramflags) of former T_Group_Data_requ
    2. Return (SAP_CB_OK)

   Incoming T_Group_Data_ind :

    1. Get properties of object, on error (object not found) return (SAP_CB_OK)
    2. Check if object "communication enable", on error return (SAP_CB_OK)
    3. Check APCI
        on value read : check readable
            error : return (SAP_CB_OK)
            ok: read object data, set APCI response, return (SAP_CB_SEND)

        on value write : check writeable
            error : return (SAP_CB_OK)
            ok: copy object data, return (SAP_CB_OK)

        on value response : check writeable/ response update
            error : return (SAP_CB_OK)
            ok: copy object data, return (SAP_CB_OK)

        on unknown APCI : return (SAP_CB_BREAK)
 */

STATIC uint8_t APP_RAMFlags[(APP_NUM_OF_COM_OBJS + 1) / 2 ];

uint8_t APP_ObjectData[APP_OBJECT_DATA_SIZE];

STATIC uint8_t ALG_NumQueuedGroupMessages;

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxALG_Task(void)
#else
void KnxALG_Task(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxDisp_DispatchLayer(TASK_AL_ID, ALG_ServiceTable);
}


/*
**
**  Services from Transport-Layer.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_DataGroupInd(void)
#else
STATIC void Disp_T_DataGroupInd(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t apci;

    apci = KnxAL_GetAPCIType(KnxMSG_GetMessagePtr(KnxMSG_ScratchBufferPtr));

    if (LSM_IsGrOATLoaded()) {
        switch (apci) {
            case apciGROUP_VALUE_WRITE:
                /* When the Application Layer of a device receives an A_GroupValue_Write-Service, it searches the */
                /* TSAP in all entries of the association-table and informs all the associated ASAP. */
                KnxALG_UpdateAssociatedASAPs(KnxMSG_ScratchBufferPtr, (KNX_OBJ_COMM_ENABLE | KNX_OBJ_WRITE_ENABLE));
                KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
                break;
            case apciGROUP_VALUE_RESP:
                KnxALG_UpdateAssociatedASAPs(KnxMSG_ScratchBufferPtr,
                                             (KNX_OBJ_COMM_ENABLE | KNX_OBJ_WRITE_ENABLE | KNX_OBJ_UPDATE_ENABLE));
                KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
                break;
            case apciGROUP_VALUE_READ:
                /*  When the Application Layer of a device receives an A_GroupValue_Read-Service, it searches the */
                /* TSAP in all entries of the Association Table and informs all the associated ASAPs. Only one read */
                /* response should be generated by the user (Abbruch nach dem ersten gefundenen ASAP). */
                break;
            default:
                /* invalid APCI. */
                KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
                break;
        }
    } else {
        /* AssocTab not loaded. */
        KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_DataGroupCon(void)
#else
STATIC void Disp_T_DataGroupCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{

}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE)  Disp_T_PollDataCon(void)
#else
STATIC void Disp_T_PollDataCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{

}


/*
**
**  Services from User-Layer.
**
*/

#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_A_DataGroupReq(void)
#else
STATIC void Disp_A_DataGroupReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_A_PollDataReq(void)
#else
STATIC void Disp_A_PollDataReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!! */
}


/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxALG_Init(void)
#else
void KnxALG_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    ALG_NumQueuedGroupMessages = ((uint8_t)0x00);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_GroupValue_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                              Knx_PriorityType prio)
#else
void A_GroupValue_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, Knx_PriorityType prio)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetAPCI(pBuffer, apciGROUP_VALUE_READ);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, prio);
    KnxMSG_SetLen(pBuffer, (uint8_t)8);

    pBuffer->service = T_DATA_GROUP_REQ;

    (void)KnxMSG_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_GroupValue_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                               Knx_PriorityType prio,
                                               P2VAR(uint8_t, AUTOMATIC,
                                                     KSTACK_APPL_DATA) data,
                                               uint8_t len)
#else
void A_GroupValue_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, Knx_PriorityType prio,
                            uint8_t * data,
                            uint8_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetAPCI(pBuffer, apciGROUP_VALUE_WRITE);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, prio);
    KnxMSG_SetLen(pBuffer, 8);

    pBuffer->service = T_DATA_GROUP_REQ;

    (void)KnxMSG_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxALG_PollCycle(void)
#else
void KnxALG_PollCycle(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t               idx;
    uint8_t               flags;
    KnxMSG_BufferPtr    pBuffer;
    Knx_AddressType     source;
    Knx_AddressType     dest;
    uint16_t              assoc;
    Knx_PriorityType    prio;

    if (!LSM_IsGrOATLoaded()) {  /* Association-Table must be 'LOADED'. */
        return;
    }

    if (ALG_NumQueuedGroupMessages != (uint8_t)0) {
        return;
    }

    for (idx = (uint8_t)0; idx < KnxALG_GetNumCommObjs(); idx++) {
        flags = KnxALG_GetRAMFlags(idx);

        if ((flags & KNX_OBJ_TRANSMIT_REQ) == KNX_OBJ_TRANSMIT_REQ) {
            if (!KnxALG_ObjTransmitEnabled(KnxALG_GetCommObjDescr(idx)->Config)) {
                /* Transmitting not configured. */
                KnxALG_SetRAMFlags(idx, KNX_OBJ_IDLE_ERROR);
                continue;
            }

            assoc = KnxADR_GetAssoc(idx);

            if (LOBYTE(assoc) != idx) {
                /* this is not a sending object. */
                KnxALG_SetRAMFlags(idx, KNX_OBJ_IDLE_ERROR);
                continue;
            }

            if (HIBYTE(assoc) >= KnxADR_GrATLength()) {   /* todo: handle 'INVALID_/UNUSED_TSAP'!!! */
                /* TSAP does not exist. */
                KnxALG_SetRAMFlags(idx, KNX_OBJ_IDLE_ERROR);
                continue;
            }

            pBuffer = KnxMSG_AllocateBuffer();

            if (pBuffer == (KnxMSG_BufferPtr)NULL) {
                /* no Message-Buffer available. */
                return;
            }

            pBuffer->sap   = idx;
            source         = KnxADR_GetPhysAddr();
            dest           = KnxADR_GetGroupAddress(HIBYTE(assoc));
            prio           = KnxALG_GetObjPriority(idx);

            if ((flags & KNX_OBJ_DATA_REQUEST) == KNX_OBJ_DATA_REQUEST) {
                A_GroupValue_Read_Req(pBuffer, source, dest, prio);

            } else {
                /* todo: 'WriteShort-Req' for lens <7 Bit!!!  */
                A_GroupValue_Write_Req(pBuffer, source, dest, prio, NULL, (uint8_t)0);
                /* WRITE_req */

            }

            KnxALG_SetRAMFlags(idx, KNX_OBJ_TRANSMITTING);
            return;
        }
    }
}


/*
   void AL_SetAPDUShortData(const KNX_StandardFrameRefType pmsg,uint8_t data,uint8_t nbits)
   {
    if (nbits>6) {
        return;
    }

    pmsg->apci=(pmsg->apci & (uint8_t)~0x3f) | (data & KNX_AL_SHORT_DATA_MASK[nbits-1]);
   }
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t *, KSTACK_CODE) KnxALG_GetObjectDataPointer(uint8_t objectNr)
#else
uint8_t * KnxALG_GetObjectDataPointer(uint8_t objectNr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (objectNr < KnxALG_GetNumCommObjs()) {
        return (uint8_t *)&APP_ObjectData[KnxALG_GetCommObjDescr(objectNr)->DataPtr];
    } else {
        return (uint8_t *)NULL;
    }
}

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxALG_SetRAMFlags(uint16_t objectNr, uint8_t flags)
#else
void KnxALG_SetRAMFlags(uint16_t objectNr, uint8_t flags)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if ((objectNr % (uint8_t)2) == (uint8_t)1) {
        APP_RAMFlags[objectNr >> 1] = ((flags & (uint8_t)0x0f) << 4);
    } else {
        APP_RAMFlags[objectNr >> 1] = (flags & (uint8_t)0x0f);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxALG_GetRAMFlags(uint16_t objectNr)
#else
uint8_t KnxALG_GetRAMFlags(uint16_t objectNr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t b;

    b = APP_RAMFlags[objectNr >> 1];

    if ((objectNr % (uint8_t)2) == 1) {
        return (b & (uint8_t)0xf0) >> 4;
    } else {
        return b & (uint8_t)0x0f;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t *, KSTACK_CODE) KnxALG_GetRAMFlagPointer(void)
#else
uint8_t * KnxALG_GetRAMFlagPointer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return (uint8_t *)&APP_RAMFlags;
}

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxALG_UpdateAssociatedASAPs(KnxMSG_BufferPtr pBuffer, uint8_t testFlags)
#else
void KnxALG_UpdateAssociatedASAPs(KnxMSG_BufferPtr pBuffer, uint8_t testFlags)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint16_t      ca;
    uint16_t *    ap = KnxADR_GrOATBasePtr();
    uint8_t       asap;
    uint8_t       numAssocs = KnxADR_GrOATLength();
    uint8_t       len_lsdu;
    uint8_t       len_obj;

    if ((pBuffer->sap == KNX_INVALID_TSAP) || (pBuffer->sap == KNX_UNUSED_TSAP)) {
        return;
    }

    while (numAssocs--) {
        ca = btohs(*ap++);

        if (HIBYTE(ca) == pBuffer->sap) {
            asap = LOBYTE(ca);

            if (asap <= KnxALG_GetNumCommObjs()) {
                if (KnxALG_ObjCheckEnabled(KnxALG_GetCommObjDescr(asap)->Config, testFlags)) {
/*                if (AL_ObjWriteEnabled(AL_GetCommObjDescr(asap)->Config)) */
                    len_lsdu   = KnxMSG_GetLSDULen(pBuffer);
                    len_obj    = KnxALG_GetObjLen(KnxALG_GetCommObjDescr(asap)->Type);

                    if (len_lsdu - (uint8_t)1 != len_obj) {
                        continue;
                    }

                    if (len_lsdu >= (uint8_t)2) {
                        /* Normal-Data. */
                        if (len_obj == (uint8_t)1) {
                            *KnxALG_GetObjectDataPointer(asap) = KnxAL_GetAPDUDataByte(KnxMSG_GetMessagePtr(pBuffer), 0)
                                                                 & KNX_AL_SHORT_DATA_MASK[KnxALG_GetCommObjDescr(asap)->Type];
                        } else {
                            Utl_MemCopy(KnxALG_GetObjectDataPointer(asap), KnxMSG_GetMessagePtr(pBuffer)->data, len_obj);
                        }
                    } else if (len_lsdu == (uint8_t)1) {
                        /* Short-Data. */
                        *KnxALG_GetObjectDataPointer(asap) = KnxAL_GetAPDUShortData(KnxMSG_GetMessagePtr(
                                                                                        pBuffer), KnxALG_GetCommObjDescr(
                                                                                        asap)->Type);
                    } else {
                        /* 'len_lsdu==0' ==> Error in Telegram. */
                        continue;
                    }
                }

                KnxALG_SetRAMFlags(asap, (KNX_OBJ_UPDATED | KNX_OBJ_IDLE_OK));
                /* todo: generate 'A_Event_ind'? */
            }
        }
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
