/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#include "knx_tlc.h"
#include "knx_ffi.h"

/*
** Local function prototypes.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, KSTACK_CODE) T_DataConnected_Req(void), T_Connect_ReqSrv(void), T_Disconnect_ReqSrv(void);
STATIC  FUNC(void, KSTACK_CODE) N_Data_Individual_Ind(void), T_DataIndividual_Req(void), T_DataBroadcast_Req(void);
STATIC  FUNC(void, KSTACK_CODE) N_DataBroadcast_Ind(void), N_DataIndividual_Con(void), N_DataBroadcast_Con(void);


#else
STATIC void T_DataConnected_Req(void), T_Connect_ReqSrv(void), T_Disconnect_ReqSrv(void);
STATIC void N_Data_Individual_Ind(void), T_DataIndividual_Req(void), T_DataBroadcast_Req(void);
STATIC void N_DataBroadcast_Ind(void), N_DataIndividual_Con(void), N_DataBroadcast_Con(void);


#endif /* KSTACK_MEMORY_MAPPING */

/*
** Local variables.
*/
STATIC uint8_t KnxTlc_SequenceNumberSend, KnxTlc_SequenceNumberReceived;
STATIC uint8_t KnxTlc_RepetitionCount, KnxTlc_SequenceNumberOfPDU;
STATIC Knx_AddressType KnxTlc_SourceAddress, KnxTlc_ConnectionAddress;

/*
** Local constants.
*/
STATIC const Knx_LayerServiceFunctionType KnxTlc_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      N_DATA_INDIVIDUAL_IND   */ N_Data_Individual_Ind,
/*      N_DATA_INDIVIDUAL_CON   */ N_DataIndividual_Con,
/*      N_DATA_BROADCAST_IND    */ N_DataBroadcast_Ind,
/*      N_DATA_BROADCAST_CON    */ N_DataBroadcast_Con,
/*      T_CONNECT_REQ           */ T_Connect_ReqSrv,
/*      T_DISCONNECT_REQ        */ T_Disconnect_ReqSrv,
/*      T_DATA_CONNECTED_REQ    */ T_DataConnected_Req,
/*      T_DATA_INDIVIDUAL_REQ   */ T_DataIndividual_Req,
/*      T_DATA_BROADCAST_REQ    */ T_DataBroadcast_Req
/*      ====================================================*/
};

STATIC const Knx_LayerServicesType KnxTlc_ServiceTable[] = {
    { KNX_TLC_SERVICES, SIZEOF_ARRAY(KnxTlc_Services), KnxTlc_Services }
};

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_Task(void)
#else
void KnxTlc_Task(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (KnxTmr_IsRunning(TMR_TIMER_TLC_CON_TIMEOUT) && KnxTmr_IsExpired(TMR_TIMER_TLC_CON_TIMEOUT)) {
        KnxTlc_StateMachine(tlcTIMEOUT_CON);
    }

    if (KnxTmr_IsRunning(TMR_TIMER_TLC_ACK_TIMEOUT) && KnxTmr_IsExpired(TMR_TIMER_TLC_ACK_TIMEOUT)) {
        KnxTlc_StateMachine(tlcTIMEOUT_ACK);
    }

    KnxDisp_DispatchLayer(TASK_TC_ID, KnxTlc_ServiceTable);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_Init(void)
#else
void KnxTlc_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTlc_SetSequenceNumberSend((uint8_t)0);
    KnxTlc_SetSequenceNumberReceived((uint8_t)0);
    KnxTlc_SetRepetitionCount((uint8_t)0);
    KnxTlc_SetSequenceNumberOfPDU((uint8_t)0);

    KnxTlc_SetState(KNX_TLC_STATE_CLOSED);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Connect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#else
void T_Connect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(pBuffer, T_CONNECT_REQ_PDU);
    KnxMsg_SetSourceAddress(pBuffer, source);
    KnxMsg_SetDestAddress(pBuffer, dest);
    KnxMsg_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMsg_SetLen(pBuffer, (uint8_t)7);
    pBuffer->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ; /* T_CONNECT_REQ; */

    (void)KnxMsg_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Disconnect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#else
void T_Disconnect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(pBuffer, T_DISCONNECT_REQ_PDU);
    KnxMsg_SetSourceAddress(pBuffer, source);
    KnxMsg_SetDestAddress(pBuffer, dest);
    KnxMsg_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMsg_SetLen(pBuffer, 7);
    pBuffer->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ; /* T_DISCONNECT_REQ; */

    (void)KnxMsg_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Ack_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo)
#else
void T_Ack_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(pBuffer, T_ACK_PDU | ((SeqNo & (uint8_t)0x0f) << 2));
    KnxMsg_SetSourceAddress(pBuffer, source);
    KnxMsg_SetDestAddress(pBuffer, dest);
    KnxMsg_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMsg_SetLen(pBuffer, 7);
    pBuffer->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ;

    (void)KnxMsg_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Nak_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo)
#else
void T_Nak_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(pBuffer, T_NAK_PDU | ((SeqNo & (uint8_t)0x0f) << 2));
    KnxMsg_SetSourceAddress(pBuffer, source);
    KnxMsg_SetDestAddress(pBuffer, dest);
    KnxMsg_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMsg_SetLen(pBuffer, 7);
    pBuffer->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ;

    (void)KnxMsg_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxTlc_GetSequenceNumberSend(void)
#else
uint8_t KnxTlc_GetSequenceNumberSend(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTlc_SequenceNumberSend;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxTlc_GetSequenceNumberReceived(void)
#else
uint8_t KnxTlc_GetSequenceNumberReceived(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTlc_SequenceNumberReceived;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxTlc_GetRepetitionCount(void)
#else
uint8_t KnxTlc_GetRepetitionCount(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTlc_RepetitionCount;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxTlc_GetSequenceNumberOfPDU(void)
#else
uint8_t KnxTlc_GetSequenceNumberOfPDU(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTlc_SequenceNumberOfPDU;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_AddressType, KSTACK_CODE) KnxTlc_GetSourceAddress(void)
#else
Knx_AddressType KnxTlc_GetSourceAddress(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTlc_SourceAddress;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_AddressType, KSTACK_CODE) KnxTlc_GetConnectionAddress(void)
#else
Knx_AddressType KnxTlc_GetConnectionAddress(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTlc_ConnectionAddress;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_SetSequenceNumberSend(uint8_t SequenceNumberSend)
#else
void KnxTlc_SetSequenceNumberSend(uint8_t SequenceNumberSend)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTlc_SequenceNumberSend = (SequenceNumberSend & ((uint8_t)0x0f));
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_SetSequenceNumberReceived(uint8_t SequenceNumberReceived)
#else
void KnxTlc_SetSequenceNumberReceived(uint8_t SequenceNumberReceived)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTlc_SequenceNumberReceived = (SequenceNumberReceived & ((uint8_t)0x0f));
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)  KnxTlc_SetRepetitionCount(uint8_t RepetitionCount)
#else
void KnxTlc_SetRepetitionCount(uint8_t RepetitionCount)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTlc_RepetitionCount = RepetitionCount;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU)
#else
void KnxTlc_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTlc_SequenceNumberOfPDU = SequenceNumberOfPDU;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_SetSourceAddress(Knx_AddressType SourceAddress)
#else
void KnxTlc_SetSourceAddress(Knx_AddressType SourceAddress)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTlc_SourceAddress = SourceAddress;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress)
#else
void KnxTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTlc_ConnectionAddress = ConnectionAddress;
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
STATIC FUNC(void, KSTACK_CODE) N_Data_Individual_Ind(void)
#else
STATIC void N_Data_Individual_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t tpci;

    KnxTlc_SetSourceAddress(KnxMsg_GetSourceAddress(KnxMsg_ScratchBufferPtr)); /* todo: !!! TESTEN !!! */

    tpci = KnxMsg_GetTPCI(KnxMsg_ScratchBufferPtr);

    //printf("N_Data_Individual_Ind [%02x]\n", tpci);

    switch (tpci  & (uint8_t)0xc0) {
        case TPCI_UDT:   /* Unnumbered Data (1:1-Connection-Less). */
            KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_T_DATA_INDIVIDUAL_IND;
            (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
            break;
        case TPCI_NDT:   /* Numbered Data (T_DATA_CONNECTED_REQ_PDU, 1:1-Connection-Oriented). */
            KnxTlc_SetSequenceNumberOfPDU(KnxMsg_GetSeqNo(KnxMsg_ScratchBufferPtr));
            KnxTlc_StateMachine((KNX_TlcEventType)tlcDATA_CONNECTED_IND);
            break;
        case TPCI_UCD:   /* Unnumbered Control. (CONNECT|DISCONNECT). */
            printf("TPCI_UCD [%02x]\n", tpci);
            if (tpci == T_CONNECT_REQ_PDU) {
                /* T_CONNECT_IND */
                KnxTlc_StateMachine((KNX_TlcEventType)tlcCONNECT_IND);
                printf("T_CONNECT_IND\n");
                KNX_CALLBACK_T_CONNECT_IND();
            } else if (tpci == T_DISCONNECT_REQ_PDU) {
                /* T_DISCONNECT_IND */
                KnxTlc_StateMachine((KNX_TlcEventType)tlcDISCONNECT_IND);
                KNX_CALLBACK_T_DISCONNECT_IND();
                printf("T_DISCONNECT_IND\n");
            } else {
                KnxMsg_ReleaseBuffer(KnxMsg_ScratchBufferPtr);
            }
            break;
        case TPCI_NCD:                                                      /* Numbered Control (TACK|TNACK). */
            tpci  &= (uint8_t)0xC3;
            KnxTlc_SetSequenceNumberOfPDU(KnxMsg_GetSeqNo(KnxMsg_ScratchBufferPtr));

            if (tpci == T_ACK_PDU) {
                KnxTlc_StateMachine((KNX_TlcEventType)tlcACK_IND);
            } else if (tpci == T_NAK_PDU) {
                KnxTlc_StateMachine((KNX_TlcEventType)tlcNAK_IND);
            } else {
                KnxMsg_ReleaseBuffer(KnxMsg_ScratchBufferPtr);
            }

            break;
        default:
            ASSERT(FALSE);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) N_DataBroadcast_Ind(void)
#else
STATIC void N_DataBroadcast_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_T_DATA_BROADCAST_IND;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) N_DataIndividual_Con(void)
#else
STATIC void N_DataIndividual_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement !!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) N_DataBroadcast_Con(void)
#else
STATIC void N_DataBroadcast_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_T_DATA_BROADCAST_CON;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


/*
**  Services from Application-Layer.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_DataIndividual_Req(void)
#else
STATIC void T_DataIndividual_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(KnxMsg_ScratchBufferPtr, TPCI_UDT);
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_DataConnected_Req(void)
#else
STATIC void T_DataConnected_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(KnxMsg_ScratchBufferPtr, TPCI_NDT);
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_Connect_ReqSrv(void)
#else
STATIC void T_Connect_ReqSrv(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(KnxMsg_ScratchBufferPtr, TPCI_UCD);
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_Disconnect_ReqSrv(void)
#else
STATIC void T_Disconnect_ReqSrv(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(KnxMsg_ScratchBufferPtr, TPCI_UCD);
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_DataBroadcast_Req(void)
#else
STATIC void T_DataBroadcast_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(KnxMsg_ScratchBufferPtr, TPCI_UDT);
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_N_DATA_BROADCAST_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
