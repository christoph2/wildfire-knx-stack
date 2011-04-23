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

/*
**  Application-Layer / Group-Oriented.
*/

#include "knx_layer_application.h"

static void Disp_A_DataGroupReq(void), Disp_A_PollDataReq(void), Disp_T_PollDataCon(void);
static void Disp_T_DataGroupInd(void), Disp_T_DataGroupCon(void);

static const KNXLayerServiceFunctionType ALG_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      T_DATA_GROUP_IND        */ Disp_T_DataGroupInd,
/*      T_DATA_GROUP_CON        */ Disp_T_DataGroupCon,
/*      T_POLL_DATA_CON         */ Disp_T_PollDataCon,
/*      A_DATA_GROUP_REQ        */ Disp_A_DataGroupReq,
/*      A_POLL_DATA_REQ         */ Disp_A_PollDataReq,
/*      ====================================================*/
};

static const KNXLayerServicesType ALG_ServiceTable[] = {
    {KNX_ALG_SERVICES, 5, ALG_Services}
};

#define SAP_CB_OK       ((uint8)0)      /* Ok (Return messagebuffer to system). */
#define SAP_CB_BREAK    ((uint8)1)      /* Break and free buffer */
#define SAP_CB_GETBUF   ((uint8)2)      /* Hold buffer and break */
#define SAP_CB_SEND     ((uint8)3)      /* Send message and break */

const uint8 KNX_AL_SHORT_DATA_MASK[] = {
    (uint8)(1 << 1) - 1, (uint8)(1 << 2) - 1, (uint8)(1 << 3) - 1, (uint8)(1 << 4) - 1,
    (uint8)(1 << 5) - 1, (uint8)(1 << 6) - 1, (uint8)(1 << 7) - 1, (uint8)(1 << 8) - 1
};

const uint8 KNX_OBJ_LEN_TAB[16] = {
    (uint8)1, (uint8)1,  (uint8)1,  (uint8)1,  (uint8)1, (uint8)1,
    (uint8)1, (uint8)1,  (uint8)2,  (uint8)3,  (uint8)4, (uint8)6,
    (uint8)8, (uint8)10, (uint8)14, (uint8)0
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

static uint8 APP_RAMFlags[(APP_NUM_OF_COM_OBJS + 1) / 2 ];

uint8 APP_ObjectData[APP_OBJECT_DATA_SIZE];

static uint8 ALG_NumQueuedGroupMessages;

void ALG_Task(void)
{
    KNXDispDispatchLayer(TASK_AL_ID, ALG_ServiceTable);
}

/*
**
**  Services from Transport-Layer.
**
*/
static void Disp_T_DataGroupInd(void)
{
    uint8 apci;

    apci = AL_GetAPCIType(MSG_GetMessagePtr(MSG_ScratchBuffer));

    if (LSM_IsGrOATLoaded()) {
        switch (apci) {
            case apciGROUP_VALUE_WRITE:
                /* When the Application Layer of a device receives an A_GroupValue_Write-Service, it searches the */
                /* TSAP in all entries of the association-table and informs all the associated ASAP. */
                AL_UpdateAssociatedASAPs(MSG_ScratchBuffer, (KNX_OBJ_COMM_ENABLE | KNX_OBJ_WRITE_ENABLE));
                (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
                break;
            case apciGROUP_VALUE_RESP:
                AL_UpdateAssociatedASAPs(MSG_ScratchBuffer, (KNX_OBJ_COMM_ENABLE | KNX_OBJ_WRITE_ENABLE | KNX_OBJ_UPDATE_ENABLE));
                (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
                break;
            case apciGROUP_VALUE_READ:
                /*  When the Application Layer of a device receives an A_GroupValue_Read-Service, it searches the */
                /* TSAP in all entries of the Association Table and informs all the associated ASAPs. Only one read */
                /* response should be generated by the user (Abbruch nach dem ersten gefundenen ASAP). */
                break;
            default:
                /* invalid APCI. */
                (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
                break;
        }
    } else {
        /* AssocTab not loaded. */
        (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
    }
}

static void Disp_T_DataGroupCon(void)
{

}

static void Disp_T_PollDataCon(void)
{

}

/*
**
**  Services from User-Layer.
**
*/

static void Disp_A_DataGroupReq(void)
{
    /* todo: Implement!! */
}

static void Disp_A_PollDataReq(void)
{
    /* todo: Implement!! */
}

/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/

void ALG_Init(void)
{
    ALG_NumQueuedGroupMessages = ((uint8)0x00);
}

void A_GroupValue_Read_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, KNX_PriorityType prio)
{
    MSG_SetAPCI(pBuffer, apciGROUP_VALUE_READ);
    MSG_SetSourceAddress(pBuffer, source);
    MSG_SetDestAddress(pBuffer, dest);
    MSG_SetPriority(pBuffer, prio);
    MSG_SetLen(pBuffer, (uint8)8);

    pBuffer->service = T_DATA_GROUP_REQ;

    (void)MSG_Post(pBuffer);
}

void A_GroupValue_Write_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, KNX_PriorityType prio, uint8 * data,
                            uint8 len)
{
    MSG_SetAPCI(pBuffer, apciGROUP_VALUE_WRITE);
    MSG_SetSourceAddress(pBuffer, source);
    MSG_SetDestAddress(pBuffer, dest);
    MSG_SetPriority(pBuffer, prio);
    MSG_SetLen(pBuffer, 8);

    pBuffer->service = T_DATA_GROUP_REQ;

    (void)MSG_Post(pBuffer);
}

void ALG_PollCycle(void)
{
    uint8               idx, flags;
    PMSG_Buffer         pBuffer;
    Knx_AddressType     source, dest;
    uint16              assoc;
    KNX_PriorityType    prio;

    if (!LSM_IsGrOATLoaded()) {  /* Association-Table must be 'LOADED'. */
        return;
    }

    if (ALG_NumQueuedGroupMessages != (uint8)0) {
        return;
    }

    for (idx = (uint8)0; idx < AL_GetNumCommObjs(); idx++) {
        flags = AL_GetRAMFlags(idx);

        if ((flags & KNX_OBJ_TRANSMIT_REQ) == KNX_OBJ_TRANSMIT_REQ) {
            if (!AL_ObjTransmitEnabled(AL_GetCommObjDescr(idx)->Config)) {
                /* Transmitting not configured. */
                AL_SetRAMFlags(idx, KNX_OBJ_IDLE_ERROR);
                continue;
            }

            assoc = ADR_GetAssoc(idx);

            if (LOBYTE(assoc) != idx) {
                /* this is not a sending object. */
                AL_SetRAMFlags(idx, KNX_OBJ_IDLE_ERROR);
                continue;
            }

            if (HIBYTE(assoc) >= ADR_GrATLength()) {   /* todo: handle 'INVALID_/UNUSED_TSAP'!!! */
                /* TSAP does not exist. */
                AL_SetRAMFlags(idx, KNX_OBJ_IDLE_ERROR);
                continue;
            }

            pBuffer = MSG_AllocateBuffer();

            if (pBuffer == (PMSG_Buffer)NULL) {
                /* no Message-Buffer available. */
                return;
            }

            pBuffer->sap   = idx;
            source         = ADR_GetPhysAddr();
            dest           = ADR_GetGroupAddress(HIBYTE(assoc));
            prio           = AL_GetObjPriority(idx);

            if ((flags & KNX_OBJ_DATA_REQUEST) == KNX_OBJ_DATA_REQUEST) {
                A_GroupValue_Read_Req(pBuffer, source, dest, prio);

            } else {
                /* todo: 'WriteShort-Req' for lens <7 Bit!!!  */
                A_GroupValue_Write_Req(pBuffer, source, dest, prio, NULL, (uint8)0);
                /* WRITE_req */

            }

            AL_SetRAMFlags(idx, KNX_OBJ_TRANSMITTING);
            return;
        }
    }
}

/*
   void AL_SetAPDUShortData(const KNX_StandardFrameRefType pmsg,uint8 data,uint8 nbits)
   {
    if (nbits>6) {
        return;
    }

    pmsg->apci=(pmsg->apci & (uint8)~0x3f) | (data & KNX_AL_SHORT_DATA_MASK[nbits-1]);
   }
 */

uint8 * AL_GetObjectDataPointer(uint8 objectNr)
{
    if (objectNr < AL_GetNumCommObjs()) {
        return (uint8 *)&APP_ObjectData[AL_GetCommObjDescr(objectNr)->DataPtr];
    } else {
        return (uint8 *)NULL;
    }
}

void AL_SetRAMFlags(uint16 objectNr, uint8 flags)
{
    if ((objectNr % (uint8)2) == (uint8)1) {
        APP_RAMFlags[objectNr >> 1] = ((flags & (uint8)0x0f) << 4);
    } else {
        APP_RAMFlags[objectNr >> 1] = (flags & (uint8)0x0f);
    }
}

uint8 AL_GetRAMFlags(uint16 objectNr)
{
    uint8 b;

    b = APP_RAMFlags[objectNr >> 1];

    if ((objectNr % (uint8)2) == 1) {
        return (b & (uint8)0xf0) >> 4;
    } else {
        return (b & (uint8)0x0f);
    }
}

uint8 * AL_GetRAMFlagPointer(void)
{
    return (uint8 *)&APP_RAMFlags;
}

void AL_UpdateAssociatedASAPs(PMSG_Buffer pBuffer, uint8 testFlags)
{
    uint16  ca, * ap = ADR_GrOATBasePtr();
    uint8   asap;
    uint8   numAssocs = ADR_GrOATLength();
    uint8   len_lsdu, len_obj;

    if ((pBuffer->sap == KNX_INVALID_TSAP) || (pBuffer->sap == KNX_UNUSED_TSAP)) {
        return;
    }

    while (numAssocs--) {
        ca = btohs(*ap++);

        if (HIBYTE(ca) == pBuffer->sap) {
            asap = LOBYTE(ca);

            if (asap <= AL_GetNumCommObjs()) {
                if (AL_ObjCheckEnabled(AL_GetCommObjDescr(asap)->Config, testFlags)) {
/*                if (AL_ObjWriteEnabled(AL_GetCommObjDescr(asap)->Config)) */
                    len_lsdu   = MSG_GetLSDULen(pBuffer);
                    len_obj    = AL_GetObjLen(AL_GetCommObjDescr(asap)->Type);

                    if (len_lsdu - (uint8)1 != len_obj) {
                        continue;
                    }

                    if (len_lsdu >= (uint8)2) {
                        /* Normal-Data. */
                        if (len_obj == (uint8)1) {
                            *AL_GetObjectDataPointer(asap) = AL_GetAPDUDataByte(MSG_GetMessagePtr(pBuffer), 0)
                                                             & KNX_AL_SHORT_DATA_MASK[AL_GetCommObjDescr(asap)->Type];
                        } else {
                            Utl_MemCopy(AL_GetObjectDataPointer(asap), MSG_GetMessagePtr(pBuffer)->data, len_obj);
                        }
                    } else if (len_lsdu == (uint8)1) {
                        /* Short-Data. */
                        *AL_GetObjectDataPointer(asap) = AL_GetAPDUShortData(MSG_GetMessagePtr(pBuffer), AL_GetCommObjDescr(
                                                                                 asap)->Type);
                    } else {
                        /* 'len_lsdu==0' ==> Error in Telegram. */
                        continue;
                    }
                }

                AL_SetRAMFlags(asap, (KNX_OBJ_UPDATED | KNX_OBJ_IDLE_OK));
                /* todo: generate 'A_Event_ind'? */
            }
        }
    }
}

