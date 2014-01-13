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
#include "knx_tlc.h"

/*
** Local function prototypes.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, KSTACK_CODE) Disp_T_DataConnectedReq(void), Disp_T_ConnectReq(void), Disp_T_DisconnectReq(void);
STATIC  FUNC(void, KSTACK_CODE) Disp_N_Data_IndividualInd(void), Disp_T_DataIndividualReq(void), Disp_T_DataBroadcastReq(void);
STATIC  FUNC(void, KSTACK_CODE) Disp_N_DataBroadcastInd(void), Disp_N_DataIndividualCon(void), Disp_N_DataBroadcast_Con(void);


#else
STATIC void Disp_T_DataConnectedReq(void), Disp_T_ConnectReq(void), Disp_T_DisconnectReq(void);
STATIC void Disp_N_Data_IndividualInd(void), Disp_T_DataIndividualReq(void), Disp_T_DataBroadcastReq(void);
STATIC void Disp_N_DataBroadcastInd(void), Disp_N_DataIndividualCon(void), Disp_N_DataBroadcast_Con(void);


#endif /* KSTACK_MEMORY_MAPPING */

/*
** Local variables.
*/
STATIC uint8            KnxTLC_SequenceNumberSend, KnxTLC_SequenceNumberReceived;
STATIC uint8            KnxTLC_RepetitionCount, KnxTLC_SequenceNumberOfPDU;
STATIC Knx_AddressType  KnxTLC_SourceAddress, KnxTLC_ConnectionAddress;

/*
** Local constants.
*/
STATIC const Knx_LayerServiceFunctionType TLC_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      N_DATA_INDIVIDUAL_IND   */ Disp_N_Data_IndividualInd,
/*      N_DATA_INDIVIDUAL_CON   */ Disp_N_DataIndividualCon,
/*      N_DATA_BROADCAST_IND    */ Disp_N_DataBroadcastInd,
/*      N_DATA_BROADCAST_CON    */ Disp_N_DataBroadcast_Con,
/*      T_CONNECT_REQ           */ Disp_T_ConnectReq,
/*      T_DISCONNECT_REQ        */ Disp_T_DisconnectReq,
/*      T_DATA_CONNECTED_REQ    */ Disp_T_DataConnectedReq,
/*      T_DATA_INDIVIDUAL_REQ   */ Disp_T_DataIndividualReq,
/*      T_DATA_BROADCAST_REQ    */ Disp_T_DataBroadcastReq
/*      ====================================================*/
};

STATIC const Knx_LayerServicesType TLC_ServiceTable[] = {
    {KNX_TLC_SERVICES, 9, TLC_Services}
};

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_Task(void)
#else
void KnxTLC_Task(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (KnxTmr_IsExpired(TMR_TIMER_TLC_CON_TIMEOUT)) {
        KnxTLC_StateMachine(tlcTIMEOUT_CON);
    }

    if (KnxTmr_IsExpired(TMR_TIMER_TLC_ACK_TIMEOUT)) {
        KnxTLC_StateMachine(tlcTIMEOUT_ACK);
    }

    KnxDisp_DispatchLayer(TASK_TC_ID, TLC_ServiceTable);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_Init(void)
#else
void KnxTLC_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_SetSequenceNumberSend((uint8)0);
    KnxTLC_SetSequenceNumberReceived((uint8)0);
    KnxTLC_SetRepetitionCount((uint8)0);
    KnxTLC_SetSequenceNumberOfPDU((uint8)0);

    KnxTLC_SetState(CLOSED);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Connect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#else
void T_Connect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(pBuffer, T_CONNECT_REQ_PDU);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, (uint8)7);
    pBuffer->service = N_DATA_INDIVIDUAL_REQ; /* T_CONNECT_REQ; */

    (void)KnxMSG_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Disconnect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#else
void T_Disconnect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(pBuffer, T_DISCONNECT_REQ_PDU);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, 7);
    pBuffer->service = N_DATA_INDIVIDUAL_REQ; /* T_DISCONNECT_REQ; */

    (void)KnxMSG_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Ack_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo)
#else
void T_Ack_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(pBuffer, T_ACK_PDU | ((SeqNo & (uint8)0x0f) << 2));
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, 7);
    pBuffer->service = N_DATA_INDIVIDUAL_REQ;

    (void)KnxMSG_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Nak_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo)
#else
void T_Nak_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(pBuffer, T_NAK_PDU | ((SeqNo & (uint8)0x0f) << 2));
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, 7);
    pBuffer->service = N_DATA_INDIVIDUAL_REQ;

    (void)KnxMSG_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8, KSTACK_CODE) KnxTLC_GetSequenceNumberSend(void)
#else
uint8 KnxTLC_GetSequenceNumberSend(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTLC_SequenceNumberSend;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8, KSTACK_CODE) KnxTLC_GetSequenceNumberReceived(void)
#else
uint8 KnxTLC_GetSequenceNumberReceived(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTLC_SequenceNumberReceived;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8, KSTACK_CODE) KnxTLC_GetRepetitionCount(void)
#else
uint8 KnxTLC_GetRepetitionCount(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTLC_RepetitionCount;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8, KSTACK_CODE) KnxTLC_GetSequenceNumberOfPDU(void)
#else
uint8 KnxTLC_GetSequenceNumberOfPDU(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTLC_SequenceNumberOfPDU;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_AddressType, KSTACK_CODE) KnxTLC_GetSourceAddress(void)
#else
Knx_AddressType KnxTLC_GetSourceAddress(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTLC_SourceAddress;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_AddressType, KSTACK_CODE) KnxTLC_GetConnectionAddress(void)
#else
Knx_AddressType KnxTLC_GetConnectionAddress(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTLC_ConnectionAddress;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_SetSequenceNumberSend(uint8 SequenceNumberSend)
#else
void KnxTLC_SetSequenceNumberSend(uint8 SequenceNumberSend)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_SequenceNumberSend = (SequenceNumberSend & ((uint8)0x0f));
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_SetSequenceNumberReceived(uint8 SequenceNumberReceived)
#else
void KnxTLC_SetSequenceNumberReceived(uint8 SequenceNumberReceived)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_SequenceNumberReceived = (SequenceNumberReceived & ((uint8)0x0f));
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)  KnxTLC_SetRepetitionCount(uint8 RepetitionCount)
#else
void KnxTLC_SetRepetitionCount(uint8 RepetitionCount)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_RepetitionCount = RepetitionCount;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_SetSequenceNumberOfPDU(uint8 SequenceNumberOfPDU)
#else
void KnxTLC_SetSequenceNumberOfPDU(uint8 SequenceNumberOfPDU)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_SequenceNumberOfPDU = SequenceNumberOfPDU;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_SetSourceAddress(Knx_AddressType SourceAddress)
#else
void KnxTLC_SetSourceAddress(Knx_AddressType SourceAddress)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_SourceAddress = SourceAddress;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_SetConnectionAddress(Knx_AddressType ConnectionAddress)
#else
void KnxTLC_SetConnectionAddress(Knx_AddressType ConnectionAddress)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_ConnectionAddress = ConnectionAddress;
}


/*
**
** Local functions.
**
*/

/*
**  Services from Network-Layer.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_Data_IndividualInd(void)
#else
STATIC void Disp_N_Data_IndividualInd(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8 tpci;

    KnxTLC_SetSourceAddress(KnxMSG_GetSourceAddress(KnxMSG_ScratchBufferPtr)); /* todo: !!! TESTEN !!! */

    tpci = KnxMSG_GetTPCI(KnxMSG_ScratchBufferPtr) & (uint8)0xc0;

    switch (tpci) {
        case TPCI_UDT:   /* Unnumbered Data (1:1-Connection-Less). */
            KnxMSG_ScratchBufferPtr->service = T_DATA_INDIVIDUAL_IND;
            (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
            break;
        case TPCI_NDT:   /* Numbered Data (T_DATA_CONNECTED_REQ_PDU, 1:1-Connection-Oriented). */
            KnxTLC_SetSequenceNumberOfPDU(KnxMSG_GetSeqNo(KnxMSG_ScratchBufferPtr));
            KnxTLC_StateMachine((KNX_TlcEventType)tlcDATA_CONNECTED_IND);
            break;
        case TPCI_UCD:   /* Unnumbered Control. (CONNECT|DISCONNECT). */
            tpci = KnxMSG_GetTPCI(KnxMSG_ScratchBufferPtr);

            if (tpci == T_CONNECT_REQ_PDU) {
                /* T_CONNECT_IND */
                KnxTLC_StateMachine((KNX_TlcEventType)tlcCONNECT_IND);
            } else if (tpci == T_DISCONNECT_REQ_PDU) {
                /* T_DISCONNECT_IND */
                KnxTLC_StateMachine((KNX_TlcEventType)tlcDISCONNECT_IND);
            } else {
                KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
            }

            break;
        case TPCI_NCD:                                                      /* Numbered Control (TACK|TNACK). */
            tpci = KnxMSG_GetTPCI(KnxMSG_ScratchBufferPtr) & (uint8)0xC3;   /* 0x03 */
            KnxTLC_SetSequenceNumberOfPDU(KnxMSG_GetSeqNo(KnxMSG_ScratchBufferPtr));

            if (tpci == T_ACK_PDU) {
                KnxTLC_StateMachine((KNX_TlcEventType)tlcACK_IND);
            } else if (tpci == T_NAK_PDU) {
                KnxTLC_StateMachine((KNX_TlcEventType)tlcNAK_IND);
            } else {
                KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
            }

            break;
        default:
            ASSERT(FALSE);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataBroadcastInd(void)
#else
STATIC void Disp_N_DataBroadcastInd(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ScratchBufferPtr->service = T_DATA_BROADCAST_IND;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataIndividualCon(void)
#else
STATIC void Disp_N_DataIndividualCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement !!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataBroadcast_Con(void)
#else
STATIC void Disp_N_DataBroadcast_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ScratchBufferPtr->service = T_DATA_BROADCAST_CON;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


/*
**  Services from Application-Layer.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_DataIndividualReq(void)
#else
STATIC void Disp_T_DataIndividualReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UDT);
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_DataConnectedReq(void)
#else
STATIC void Disp_T_DataConnectedReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_NDT);
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_ConnectReq(void)
#else
STATIC void Disp_T_ConnectReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UCD);
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_DisconnectReq(void)
#else
STATIC void Disp_T_DisconnectReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UCD);
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_DataBroadcastReq(void)
#else
STATIC void Disp_T_DataBroadcastReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UDT);
    KnxMSG_ScratchBufferPtr->service = N_DATA_BROADCAST_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
