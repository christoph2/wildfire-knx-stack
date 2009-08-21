#include "Transport.h"

/*
** Hinweis: die 'KNX_TlcEventType'-cast sind überflüssig!!!
*/

static void Disp_T_DataConnectedReq(void),Disp_T_ConnectReq(void),Disp_T_DisconnectReq(void);
static void Disp_N_Data_IndividualInd(void),Disp_T_DataIndividualReq(void),Disp_T_DataBroadcastReq(void);
static void Disp_N_DataBroadcastInd(void),Disp_N_DataIndividualCon(void),Disp_N_DataBroadcast_Con(void);

static uint8 KNXTlc_SequenceNumberSend,KNXTlc_SequenceNumberReceived;
static uint8 KNXTlc_RepetitionCount,KNXTlc_SequenceNumberOfPDU;
static Knx_AddressType KNXTlc_SourceAddress,KNXTlc_ConnectionAddress;

static const KNXLayerServiceFunctionType TLC_Services[]={
/*      Service                     Handler                 */
/*      ====================================================*/
/*      N_DATA_INDIVIDUAL_IND   */  Disp_N_Data_IndividualInd,
/*      N_DATA_INDIVIDUAL_CON   */  Disp_N_DataIndividualCon,
/*      N_DATA_BROADCAST_IND    */  Disp_N_DataBroadcastInd,
/*      N_DATA_BROADCAST_CON    */  Disp_N_DataBroadcast_Con,
/*      T_CONNECT_REQ           */  Disp_T_ConnectReq,
/*      T_DISCONNECT_REQ        */  Disp_T_DisconnectReq,
/*      T_DATA_CONNECTED_REQ    */  Disp_T_DataConnectedReq,
/*      T_DATA_INDIVIDUAL_REQ   */  Disp_T_DataIndividualReq,
/*      T_DATA_BROADCAST_REQ    */  Disp_T_DataBroadcastReq
/*      ====================================================*/
};

static const KNXLayerServicesType TLC_ServiceTable[]={
    {KNX_TLC_SERVICES,9,TLC_Services}
};

void TLC_Task(void)
{
/*
**    check: Timeout-Handling vor oder nach dem Dispatcher???
*/
	if (TM_IsExpired(TM_TIMER_TLC_CON_TIMEOUT)) {
		TL_StateMachine(tlcTIMEOUT_CON);
	}

	if (TM_IsExpired(TM_TIMER_TLC_ACK_TIMEOUT)) {
		TL_StateMachine(tlcTIMEOUT_ACK);
	}

        KNXDispDispatchLayer(TASK_TC_ID,TLC_ServiceTable);
}

/*
//
//  Hinweis: __ConnectionAddress braucht nicht initialisiert werden, macht auch keinen Sinn,
//      denn wenn im Zusatand CLOSED ein T_CONNECT_REQ empfangen wird, wird der korrekte Zustand
//      eingenommen, egal, ob CONN_ADDR gleich oder ungleich SRC_ADDR.
//
//
*/

void TL_Init(void)  /* check: zwischen TLC_ und TLG_ Init unterscheiden??? */
{
    KNXTlcSetSequenceNumberSend((uint8)0);
    KNXTlcSetSequenceNumberReceived((uint8)0);
    KNXTlcSetRepetitionCount((uint8)0);
    KNXTlcSetSequenceNumberOfPDU((uint8)0);

    KNXTlcSetState(CLOSED);
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

    KNXTlcSetSourceAddress(MSG_GetSourceAddress(MSG_ScratchBuffer)); /* todo: !!! TESTEN !!! */

    tpci=MSG_GetTPCI(MSG_ScratchBuffer) & 0xc0;  /* check: muss das so sein oder kann */
                                                            /* 'MSG_GetTPCI()' entsprechend angepasst werden??? */
    switch (tpci) {
        case TPCI_UDT:   /* Unnumbered Data (1:1-Connection-Less). */
            MSG_ScratchBuffer->service=T_DATA_INDIVIDUAL_IND;  /* todo: Überprüfen!!! */
            (void)MSG_Post(MSG_ScratchBuffer);
            break;
        case TPCI_NDT:   /* Numbered Data (T_DATA_CONNECTED_REQ_PDU, 1:1-Connection-Oriented). */
            KNXTlcSetSequenceNumberOfPDU(MSG_GetSeqNo(MSG_ScratchBuffer));
            TL_StateMachine((KNX_TlcEventType)tlcDATA_CONNECTED_IND);
            break;
        case TPCI_UCD:   /* Unnumbered Control. (CONNECT|DISCONNECT). */
            tpci=MSG_GetTPCI(MSG_ScratchBuffer);

            if (tpci==T_CONNECT_REQ_PDU) {
                /* T_CONNECT_IND */
                TL_StateMachine((KNX_TlcEventType)tlcCONNECT_IND);
            } else if (tpci==T_DISCONNECT_REQ_PDU) {
                /* T_DISCONNECT_IND */
                TL_StateMachine((KNX_TlcEventType)tlcDISCONNECT_IND);
            } else {
                (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);      /* ungültige Message löschen. */
            }
            break;
        case TPCI_NCD:   /* Numbered Control (TACK|TNACK). */
            tpci=MSG_GetTPCI(MSG_ScratchBuffer) & 0xC3;  /* 0x03 */
            KNXTlcSetSequenceNumberOfPDU(MSG_GetSeqNo(MSG_ScratchBuffer));

            if (tpci==T_ACK_PDU) {
                TL_StateMachine((KNX_TlcEventType)tlcACK_IND);
            } else if (tpci==T_NAK_PDU) {
                TL_StateMachine((KNX_TlcEventType)tlcNAK_IND);
            } else {
                (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);      /* ungültige Message löschen. */
            }
            break;
        default:
            ASSERT(FALSE);
    }
}

static void Disp_N_DataBroadcastInd(void)
{
    MSG_ScratchBuffer->service=T_DATA_BROADCAST_IND;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_DataIndividualCon(void)
{
    /* todo: Implementieren !!! */
}

static void Disp_N_DataBroadcast_Con(void)
{
    MSG_ScratchBuffer->service=T_DATA_BROADCAST_CON;
    (void)MSG_Post(MSG_ScratchBuffer);
}

/*
**
**  Services from Application-Layer.
**
*/

static void Disp_T_DataIndividualReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_UDT);
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_DataConnectedReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_NDT);
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_ConnectReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_UCD);
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_DisconnectReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_UCD);
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_DataBroadcastReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_UDT);
    MSG_ScratchBuffer->service=N_DATA_BROADCAST_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}


/********************************************************************/
/********************************************************************/
/********************************************************************/

/*
**  todo: die Client-Versionen müssen die State-Machine verwenden!!!
*/
void T_Connect_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest)
{
    MSG_SetTPCI(pBuffer,T_CONNECT_REQ_PDU);
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,7);
    pBuffer->service=N_DATA_INDIVIDUAL_REQ; /* T_CONNECT_REQ; */

    (void)MSG_Post(pBuffer);
}

void T_Disconnect_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest)
{
    MSG_SetTPCI(pBuffer,T_DISCONNECT_REQ_PDU);
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,7);
    pBuffer->service=N_DATA_INDIVIDUAL_REQ; /* T_DISCONNECT_REQ; */

    (void)MSG_Post(pBuffer);
}

void T_Ack_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 SeqNo)
{
    MSG_SetTPCI(pBuffer,T_ACK_PDU | ((SeqNo & 0x0f)<<2));
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,7);
    pBuffer->service=N_DATA_INDIVIDUAL_REQ;

    (void)MSG_Post(pBuffer);
}

void T_Nak_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 SeqNo)
{
    MSG_SetTPCI(pBuffer,T_NAK_PDU | ((SeqNo & 0x0f)<<2));
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,7);
    pBuffer->service=N_DATA_INDIVIDUAL_REQ;

    (void)MSG_Post(pBuffer);
}


uint8 KNXTlcGetSequenceNumberSend(void)
{
    return KNXTlc_SequenceNumberSend;
}
uint8 KNXTlcGetSequenceNumberReceived(void)
{
    return KNXTlc_SequenceNumberReceived;
}

uint8 KNXTlcGetRepetitionCount(void)
{
    return KNXTlc_RepetitionCount;
}

uint8 KNXTlcGetSequenceNumberOfPDU(void)
{
    return KNXTlc_SequenceNumberOfPDU;
}

Knx_AddressType KNXTlcGetSourceAddress(void)
{
    return KNXTlc_SourceAddress;
}

Knx_AddressType KNXTlcGetConnectionAddress(void)
{
    return KNXTlc_ConnectionAddress;
}

void KNXTlcSetSequenceNumberSend(uint8 SequenceNumberSend)
{
    KNXTlc_SequenceNumberSend=(SequenceNumberSend & ((uint8)0x0f));
}
void KNXTlcSetSequenceNumberReceived(uint8 SequenceNumberReceived)
{
    KNXTlc_SequenceNumberReceived=(SequenceNumberReceived & ((uint8)0x0f));
}

void KNXTlcSetRepetitionCount(uint8 RepetitionCount)
{
    KNXTlc_RepetitionCount=RepetitionCount;
}
void KNXTlcSetSequenceNumberOfPDU(uint8 SequenceNumberOfPDU)
{
    KNXTlc_SequenceNumberOfPDU=SequenceNumberOfPDU;
}

void KNXTlcSetSourceAddress(Knx_AddressType SourceAddress)
{
    KNXTlc_SourceAddress=SourceAddress;
}

void KNXTlcSetConnectionAddress(Knx_AddressType ConnectionAddress)
{
    KNXTlc_ConnectionAddress=ConnectionAddress;
}
