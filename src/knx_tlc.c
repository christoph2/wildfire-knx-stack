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
#include "knx_tlc.h"

static void Disp_T_DataConnectedReq(void), Disp_T_ConnectReq(void), Disp_T_DisconnectReq(void);
static void Disp_N_Data_IndividualInd(void), Disp_T_DataIndividualReq(void), Disp_T_DataBroadcastReq(void);
static void Disp_N_DataBroadcastInd(void), Disp_N_DataIndividualCon(void), Disp_N_DataBroadcast_Con(void);

static uint8            KnxTLC_SequenceNumberSend, KnxTLC_SequenceNumberReceived;
static uint8            KnxTLC_RepetitionCount, KnxTLC_SequenceNumberOfPDU;
static Knx_AddressType  KnxTLC_SourceAddress, KnxTLC_ConnectionAddress;

static const Knx_LayerServiceFunctionType TLC_Services[] = {
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

static const Knx_LayerServicesType TLC_ServiceTable[] = {
    {KNX_TLC_SERVICES, 9, TLC_Services}
};

void KnxTLC_Task(void)
{
    if (TM_IsExpired(TM_TIMER_TLC_CON_TIMEOUT)) {
        KnxTLC_StateMachine(tlcTIMEOUT_CON);
    }

    if (TM_IsExpired(TM_TIMER_TLC_ACK_TIMEOUT)) {
        KnxTLC_StateMachine(tlcTIMEOUT_ACK);
    }

    KnxDisp_DispatchLayer(TASK_TC_ID, TLC_ServiceTable);
}

void KnxTLC_Init(void)
{
    KnxTLC_SetSequenceNumberSend((uint8)0);
    KnxTLC_SetSequenceNumberReceived((uint8)0);
    KnxTLC_SetRepetitionCount((uint8)0);
    KnxTLC_SetSequenceNumberOfPDU((uint8)0);

    KnxTLC_SetState(CLOSED);
}

/********************************************************************/
/********************************************************************/
/********************************************************************/

/*
**
**  Services from Network-Layer.
**
*/

static void Disp_N_Data_IndividualInd(void)
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
                (void)KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
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
                (void)KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
            }

            break;
        default:
            ASSERT(FALSE);
    }
}

static void Disp_N_DataBroadcastInd(void)
{
    KnxMSG_ScratchBufferPtr->service = T_DATA_BROADCAST_IND;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}

static void Disp_N_DataIndividualCon(void)
{
    /* todo: Implement !!! */
}

static void Disp_N_DataBroadcast_Con(void)
{
    KnxMSG_ScratchBufferPtr->service = T_DATA_BROADCAST_CON;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}

/*
**
**  Services from Application-Layer.
**
*/

static void Disp_T_DataIndividualReq(void)
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UDT);
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}

static void Disp_T_DataConnectedReq(void)
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_NDT);
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}

static void Disp_T_ConnectReq(void)
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UCD);
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}

static void Disp_T_DisconnectReq(void)
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UCD);
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}

static void Disp_T_DataBroadcastReq(void)
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UDT);
    KnxMSG_ScratchBufferPtr->service = N_DATA_BROADCAST_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}

/********************************************************************/
/********************************************************************/
/********************************************************************/

void T_Connect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
{
    KnxMSG_SetTPCI(pBuffer, T_CONNECT_REQ_PDU);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, (uint8)7);
    pBuffer->service = N_DATA_INDIVIDUAL_REQ; /* T_CONNECT_REQ; */

    (void)KnxMSG_Post(pBuffer);
}

void T_Disconnect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
{
    KnxMSG_SetTPCI(pBuffer, T_DISCONNECT_REQ_PDU);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, 7);
    pBuffer->service = N_DATA_INDIVIDUAL_REQ; /* T_DISCONNECT_REQ; */

    (void)KnxMSG_Post(pBuffer);
}

void T_Ack_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo)
{
    KnxMSG_SetTPCI(pBuffer, T_ACK_PDU | ((SeqNo & (uint8)0x0f) << 2));
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, 7);
    pBuffer->service = N_DATA_INDIVIDUAL_REQ;

    (void)KnxMSG_Post(pBuffer);
}

void T_Nak_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo)
{
    KnxMSG_SetTPCI(pBuffer, T_NAK_PDU | ((SeqNo & (uint8)0x0f) << 2));
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, 7);
    pBuffer->service = N_DATA_INDIVIDUAL_REQ;

    (void)KnxMSG_Post(pBuffer);
}

uint8 KnxTLC_GetSequenceNumberSend(void)
{
    return KnxTLC_SequenceNumberSend;
}

uint8 KnxTLC_GetSequenceNumberReceived(void)
{
    return KnxTLC_SequenceNumberReceived;
}

uint8 KnxTLC_GetRepetitionCount(void)
{
    return KnxTLC_RepetitionCount;
}

uint8 KnxTLC_GetSequenceNumberOfPDU(void)
{
    return KnxTLC_SequenceNumberOfPDU;
}

Knx_AddressType KnxTLC_GetSourceAddress(void)
{
    return KnxTLC_SourceAddress;
}

Knx_AddressType KnxTLC_GetConnectionAddress(void)
{
    return KnxTLC_ConnectionAddress;
}

void KnxTLC_SetSequenceNumberSend(uint8 SequenceNumberSend)
{
    KnxTLC_SequenceNumberSend = (SequenceNumberSend & ((uint8)0x0f));
}

void KnxTLC_SetSequenceNumberReceived(uint8 SequenceNumberReceived)
{
    KnxTLC_SequenceNumberReceived = (SequenceNumberReceived & ((uint8)0x0f));
}

void KnxTLC_SetRepetitionCount(uint8 RepetitionCount)
{
    KnxTLC_RepetitionCount = RepetitionCount;
}

void KnxTLC_SetSequenceNumberOfPDU(uint8 SequenceNumberOfPDU)
{
    KnxTLC_SequenceNumberOfPDU = SequenceNumberOfPDU;
}

void KnxTLC_SetSourceAddress(Knx_AddressType SourceAddress)
{
    KnxTLC_SourceAddress = SourceAddress;
}

void KnxTLC_SetConnectionAddress(Knx_AddressType ConnectionAddress)
{
    KnxTLC_ConnectionAddress = ConnectionAddress;
}

