/*
**
**      Konnex Transport-Layer-Statemachines.
**
**/

/* check: existiert für sämtliche State-Machines (in diesem Projekt) ein gemeinsames Pattern??? */

/*
**  todo: Datei in 'TLC' umbenennen!?
*/

/*
**  Hinweis: Der Zusatnd 'CONNECTING' kann nur von der Client-Bibliothek eingenommen werden!!!
*/

/*
**  check: Flag 'bTimeout' für Events wie A0a und A6.
*/


/*
**  todo: Verify-Mode.
**        "If set to 1, verify mode is enabled. If the verify mode is enabled during "
**        "a TL connection, the system answers each reveived A_Memory_Write with an "
**        "A_Memory_Response automatically."
**        "Verfiy mode is automatically disabled when the TL connection is finished. (e.g. T_Disconnect)."
*/        
 

#include "Transport.h"
#include "Memory.h"

static KNX_TlcStateType KNXTlc_State;

static KNX_MSG_T _StoredMsg;     /* Client-only. */

static void StartConnectionTimeoutTimer(void);
static void RestartConnectionTimeoutTimer(void);
static void StopConnectionTimeoutTimer(void);
static void StartAcknowledgementTimeoutTimer(void);
static void StopAcknowledgementTimeoutTimer(void);

static void A0(void),A1(void),A2(void),A3(void),A4(void),A5(void),A6(void),A7(void),A8(void);
static void A8b(void),A9(void),A10(void),A11(void),A12(void),A13(void),A14(void),A14b(void),A15(void);

void T_Disconnect_Ind(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest);
void T_Disconnect_Con(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest);

/*  todo: 'KNX_TLC_STYLE' statt 'TL_STYLE'!!! */
/*  todo:   'KNX_Tlc_Actions' statt 'Actions' bzw. besserer Name!!! */
                         
#if TL_STYLE==3                                                         /* Transport-Layer-Statemachine-Style #3 */
static const KNX_TlcActionListType Actions[]={
        {       0,              {{A1,OPEN_IDLE},        {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A0,CONNECTING}}        },
        {       1,              {{A1,OPEN_IDLE},        {A10,OPEN_IDLE},        {A10,OPEN_WAIT},        {A10,CONNECTING}}       },
        {       2,              {{A0,CLOSED},           {A5,CLOSED},            {A5,CLOSED},            {A5,CLOSED}}            },
        {       3,              {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A0,CONNECTING}}        },
        {       4,              {{A0,CLOSED},           {A2,OPEN_IDLE},         {A2,OPEN_WAIT},         {A6,CLOSED}}            },
        {       5,              {{A0,CLOSED},           {A3,OPEN_IDLE},         {A3,OPEN_WAIT},         {A3,CONNECTING}}        },
        {       6,              {{A0,CLOSED},           {A4,OPEN_IDLE},         {A4,OPEN_WAIT},         {A6,CONNECTING}}        },
        {       7,              {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A10,CONNECTING}}       },
        {       8,              {{A0,CLOSED},           {A0,OPEN_IDLE},         {A8,OPEN_IDLE},         {A6,CLOSED}}            },
        {       9,              {{A0,CLOSED},           {A0,OPEN_IDLE},         {A6,CLOSED},            {A6,CLOSED}}            },
        {       10,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A10,CONNECTING}}       },
        {       11,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A6,CLOSED}}            },
        {       12,             {{A0,CLOSED},           {A6,CLOSED},            {A9,OPEN_WAIT},         {A6,CLOSED}}            },
        {       13,             {{A0,CLOSED},           {A6,CLOSED},            {A6,CLOSED},            {A6,CLOSED}}            },
        {       14,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A10,CONNECTING}}       },
        {       15,             {{A0,CLOSED},           {A7,OPEN_WAIT},         {A11,OPEN_WAIT},        {A11,CONNECTING}}       },
        {       16,             {{A0,CLOSED},           {A6,CLOSED},            {A6,CLOSED},            {A6,CLOSED}}            },
        {       17,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A9,OPEN_WAIT},         {A0,CONNECTING}}        },
        {       18,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A6,CLOSED},            {A0,CONNECTING}}        },
        {       19,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A13,OPEN_IDLE}}        },
        {       20,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A5,CLOSED}}            },
        {       21,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A0,CONNECTING}}        },
        {       22,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A0,CONNECTING}}        },
        {       23,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A0,CONNECTING}}        },
        {       24,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A0,CONNECTING}}        },
        {       25,             {{A12,CONNECTING},      {A6,CLOSED},            {A6,CLOSED},            {A6,CLOSED}}            },
        {       26,             {{A15,CLOSED},          {A14,CLOSED},           {A14,CLOSED},           {A14,CLOSED}}           },
        {       27,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT},         {A0,CONNECTING}}        },
};
#elif (TL_STYLE==1) || (TL_STYLE==2)            /* Transport-Layer-Statemachine, Styles #1 und #2 */

static const ACTION_LIST Actions[]={
#if     TL_STYLE==1
        {       0,              {{A1,OPEN_IDLE},        {A6,CLOSED},            {A6,CLOSED}}            },
#else
//#if TL_STYLE=2
        {       0,              {{A1,OPEN_IDLE},        {A0,OPEN_IDLE},         {A0,OPEN_IDLE}}         },
#endif

#if     TL_STYLE==1
        {       1,              {{A1,OPEN_IDLE},        {A10,OPEN_IDLE},        {A10,OPEN_WAIT}}        },
#else
        {       1,              {{A1,OPEN_IDLE},        {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
#endif
        {       2,              {{A0,CLOSED},           {A5,CLOSED},            {A5,CLOSED}}            },
        {       3,              {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
#if     TL_STYLE==1
        {       4,              {{A10,CLOSED},          {A2,OPEN_IDLE},         {A2,OPEN_WAIT}}         },
#else
        {       4,              {{A0,CLOSED},           {A2,OPEN_IDLE},         {A2,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       5,              {{A10,CLOSED},          {A3,OPEN_IDLE},         {A3,OPEN_WAIT}}         },
#else
        {       5,              {{A0,CLOSED},           {A3,OPEN_IDLE},         {A3,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       6,              {{A10,CLOSED},          {A4,OPEN_IDLE},         {A4,OPEN_WAIT}}         },
#else
        {       6,              {{A0,CLOSED},           {A4,OPEN_IDLE},         {A4,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       7,              {{A10,CLOSED},          {A10,OPEN_IDLE},        {A10,OPEN_WAIT}}        },
#else
        {       7,              {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       8,              {{A10,CLOSED},          {A6,CLOSED},            {A8,OPEN_IDLE}}         },
#else
        {       8,              {{A0,CLOSED},           {A6,CLOSED},            {A8b,OPEN_IDLE}}        },
#endif
#if     TL_STYLE==1
        {       9,              {{A10,CLOSED},          {A6,CLOSED},            {A6,CLOSED}}            },
#else
        {       9,              {{A0,CLOSED},           {A6,CLOSED},            {A0,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       10,             {{A10,CLOSED},          {A10,OPEN_IDLE},        {A10,OPEN_WAIT}}        },
#else
        {       10,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       11,             {{A10,CLOSED},          {A6,CLOSED},            {A6,CLOSED}}            },
#else
        {       11,             {{A0,CLOSED},           {A6,CLOSED},            {A0,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       12,             {{A10,CLOSED},          {A6,CLOSED},            {A9,OPEN_WAIT}}         },
#else
        {       12,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A9,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       13,             {{A10,CLOSED},          {A6,CLOSED},            {A6,CLOSED}}            },
#else
        {       13,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A6,CLOSED}}            },
#endif
#if     TL_STYLE==1
        {       14,             {{A10,CLOSED},          {A10,OPEN_IDLE},        {A10,OPEN_WAIT}}        },
#else
        {       14,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       15,             {{A5,CLOSED},           {A7,OPEN_WAIT},         {A6,CLOSED}}            },
#else
        {       15,             {{A0,CLOSED},           {A7,OPEN_WAIT},         {A11,OPEN_WAIT}}        },
#endif
        {       16,             {{A0,CLOSED},           {A6,CLOSED},            {A6,CLOSED}}            },
        {       17,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A9,OPEN_WAIT}}         },
        {       18,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A6,CLOSED}}            },
#if     TL_STYLE==1
        {       19,             {{A0,CLOSED},           {A13,OPEN_IDLE},        {A13,OPEN_WAIT}}        },
#else
        {       19,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
#endif
        {       20,             {{A0,CLOSED},           {A5,CLOSED},            {A5,CLOSED}}            },
        {       21,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
        {       22,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
        {       23,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
        {       24,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
#if     TL_STYLE==1
        {       25,             {{A12,OPEN_IDLE},       {A6,CLOSED},            {A6,CLOSED}}            },
#else
        {       25,             {{A12,OPEN_IDLE},       {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
#endif
#if     TL_STYLE==1
        {       26,             {{A15,CLOSED},          {A14,CLOSED},           {A14,CLOSED}}           },
#else
        {       26,             {{A0,CLOSED},           {A14b,CLOSED},          {A11,OPEN_WAIT}}        },
#endif
        {       27,             {{A0,CLOSED},           {A0,OPEN_IDLE},         {A0,OPEN_WAIT}}         },
};
#else
#error "Invalid TL_STYLE [1|2|3]"
#endif

/* #if 0 */

typedef uint8 (*EVENT_FUNC)(void);

static uint8 EventConnectInd(void),EventDisconnectInd(void),EventDataConnectedInd(void);
static uint8 EventAckInd(void),EventNakInd(void),EventConnectReq(void);
static uint8 EventDisconnectReq(void),EventDataConnectedReq(void),EventConnectCon(void);
static uint8 EventDisconnectCon(void),EventDataConnectedCon(void),EventAckCon(void);
static uint8 EventNakCon(void),EventTimeoutCon(void),EventTimeoutAck(void);
static uint8 EventUndefined(void);


static const EVENT_FUNC TLC_Events[]={
/*      Event                       Handler                 */
/*      ====================================================*/
/*      tlcCONNECT_IND          */  EventConnectInd,
/*      tlcDISCONNECT_IND       */  EventDisconnectInd,
/*      tlcDATA_CONNECTED_IND   */  EventDataConnectedInd,
/*      tlcACK_IND              */  EventAckInd,
/*      tlcNAK_IND              */  EventNakInd,
/*      tlcCONNECT_REQ          */  EventConnectReq,
/*      tlcDISCONNECT_REQ       */  EventDisconnectReq,
/*      tlcDATA_CONNECTED_REQ   */  EventDataConnectedReq,
/*      tlcCONNECT_CON          */  EventConnectCon,
/*      tlcDISCONNECT_CON       */  EventDisconnectCon,
/*      tlcDATA_CONNECTED_CON   */  EventDataConnectedCon,
/*      tlcACK_CON              */  EventAckCon,
/*      tlcNAK_CON              */  EventNakCon,
/*      tlcTIMEOUT_CON          */  EventTimeoutCon,
/*      tlcTIMEOUT_ACK          */  EventTimeoutAck,
/*      ====================================================*/
};

/* #endif */
        

/*
VOID CALLBACK ConnectionTimeout(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
}
*/

/*
void TLConnectionLess()
{

}

void TLConnennectionOriented()
{

}
*/

#define StoreMessage()  CopyRAM(_StoredMsg,MSG_ScratchBuffer->msg,MSG_LEN)
#define RestoreMessage()  CopyRAM(MSG_ScratchBuffer->msg,_StoredMsg,MSG_LEN)

KNX_TlcStateType KNXTlcGetState(void)
{
    return KNXTlc_State;
}

void KNXTlcSetState(KNX_TlcStateType State)
{
    KNXTlc_State=State;
}

void T_Disconnect_Ind(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest)
{
    MSG_SetTPCI(pBuffer,T_DISCONNECT_REQ_PDU);
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    
    MSG_GetMessagePtr(pBuffer)->ctrl=0xb0;  /* short-cuts... */
    MSG_GetMessagePtr(pBuffer)->ncpi=0x60;

    MSG_SetLen(pBuffer,7);
    pBuffer->service=T_DISCONNECT_IND;
    
    (void)MSG_Post(pBuffer);
}


void T_Disconnect_Con(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest)
{
    MSG_SetTPCI(pBuffer,T_DISCONNECT_REQ_PDU);
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);

    MSG_GetMessagePtr(pBuffer)->ctrl=0xb0;  /* short-cuts... */
    MSG_GetMessagePtr(pBuffer)->ncpi=0x60;
    
    MSG_SetLen(pBuffer,7);
    pBuffer->service=T_DISCONNECT_CON;
    
    (void)MSG_Post(pBuffer);
}

static void StartConnectionTimeoutTimer(void)
{
	(void)TM_Start(TM_TIMER_TLC_CON_TIMEOUT,TM_BASE_SEC,TLC_CONNECTION_TIMEOUT);
}

static void RestartConnectionTimeoutTimer(void)
{
	(void)TM_Stop(TM_TIMER_TLC_CON_TIMEOUT);
	(void)TM_Start(TM_TIMER_TLC_CON_TIMEOUT,TM_BASE_SEC,TLC_CONNECTION_TIMEOUT);
}

static void StopConnectionTimeoutTimer(void)
{
	(void)TM_Stop(TM_TIMER_TLC_CON_TIMEOUT);
}

static void StartAcknowledgementTimeoutTimer(void)
{
	(void)TM_Start(TM_TIMER_TLC_ACK_TIMEOUT,TM_BASE_SEC,TLC_ACKNOWLEDGE_TIMEOUT);
}

static void StopAcknowledgementTimeoutTimer(void)
{
	(void)TM_Stop(TM_TIMER_TLC_ACK_TIMEOUT);
}

static void A0(void)
{
    /* do nothing. */
    if (MSG_ScratchBuffer!=(PMSG_Buffer)NULL) {  /* eventuell MSG-Buffer freigeben. */
        (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
    }
}

static void A1(void)
{
    KNXTlcSetConnectionAddress(KNXTlcGetSourceAddress());

/* Send a T_CONNECT_ind to the user. */
    MSG_ScratchBuffer->service=T_CONNECT_IND;
    (void)MSG_Post(MSG_ScratchBuffer);                

    KNXTlcSetSequenceNumberSend((uint8)0);
    KNXTlcSetSequenceNumberReceived((uint8)0);
    StartConnectionTimeoutTimer();
}


static void A2(void){
    PMSG_Buffer pBuffer;    

/*      Send a N_Data_Individual.req with T_ACK_PDU, priority = SYSTEM, destination = */
/*      connection_address, sequence =SeqNoRcv to the network layer (remote device).  */
    pBuffer=MSG_AllocateBuffer();       /* neuen Puffer anfordern - check: wieso??? */

    if (pBuffer!=(PMSG_Buffer)NULL) {
        T_Ack_Req(pBuffer,ADR_GetPhysAddr(),KNXTlcGetConnectionAddress(),KNXTlcGetSequenceNumberReceived());        
        KNXTlcSetSequenceNumberReceived(KNXTlcGetSequenceNumberReceived()+1);
    } else {
        /* Fehlerbehandlung. */
    }

/*      Send the received buffer as a T_Data_Connected.ind to the user. */
    MSG_ScratchBuffer->service=T_DATA_CONNECTED_IND;
    (void)MSG_Post(MSG_ScratchBuffer);

    RestartConnectionTimeoutTimer();
}

static void A3(void)
{
/*    
**      Send an N_Data_Individual.req with T_ACK_PDU, priority = SYSTEM, destination = 
**      connection_address, sequence = sequence of received message to the network 
**      layer (remote device).
*/    

    (void)MSG_ClearBuffer(MSG_ScratchBuffer);
    T_Ack_Req(MSG_ScratchBuffer,ADR_GetPhysAddr(),KNXTlcGetConnectionAddress(),KNXTlcGetSequenceNumberReceived());
    RestartConnectionTimeoutTimer();
}

static void A4(void)
{
/*    
**      Send an N_Data_Individual.req with T_NAK_PDU, priority = SYSTEM, destination = 
**      connection_address, sequence = sequence of received message to the network 
**      layer (remote device).
*/

    (void)MSG_ClearBuffer(MSG_ScratchBuffer);
    T_Nak_Req(MSG_ScratchBuffer,ADR_GetPhysAddr(),KNXTlcGetConnectionAddress(),KNXTlcGetSequenceNumberReceived());

    RestartConnectionTimeoutTimer();
}

static void A5(void)   /* Hinweis: wird nur benötigt, wenn Messages to the User unterstützt werden!!! */
{   
/*      Send a T_Disconnect.ind to the user. */
    (void)MSG_ClearBuffer(MSG_ScratchBuffer);
    T_Disconnect_Ind(MSG_ScratchBuffer,KNXTlcGetConnectionAddress(),ADR_GetPhysAddr());

    StopAcknowledgementTimeoutTimer();
    StopConnectionTimeoutTimer();
}

static void A6(void)
{
    PMSG_Buffer pBuffer;    
/*
**      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM, 
**      destination = connection_address, sequence = 0 to the network layer (remote device).
*/

    pBuffer=MSG_AllocateBuffer();       /* neuen Puffer anfordern. */

    if (pBuffer!=(PMSG_Buffer)NULL) {
        T_Disconnect_Req(pBuffer,ADR_GetPhysAddr(),KNXTlcGetConnectionAddress());
    } else {
        /* Fehlerbehandlung. */
    }

/* Send a T_Disconnect.ind to the user. */
    if (MSG_ScratchBuffer==(PMSG_Buffer)NULL) {
        MSG_ScratchBuffer=MSG_AllocateBuffer();  /* Im Falle eines Timeouts existiert kein Puffer. */
    } else {
        (void)MSG_ClearBuffer(MSG_ScratchBuffer);
    }
    T_Disconnect_Ind(MSG_ScratchBuffer,ADR_GetPhysAddr(),KNXTlcGetConnectionAddress());  /* check: Stimmen die Adressen??? */

    StopAcknowledgementTimeoutTimer();
    StopConnectionTimeoutTimer();
}

static void A7(void)   /* Nur local-user (Client only). */
{
/*
**              Store the received T_Data_Connected.req and send as a N_Data_Individual.req 
**              with T_DATA_CONNECTED_REQ_PDU,destination = connection_address, 
**              sequence = SeqNoSend to the network layer (remote device).
*/    

    StoreMessage();

/*    MSG_ScratchBuffer->service=T_DATA_CONNECTED_REQ; // geht das so??? oder doch besser eine Funktion (SeqNoSend !!!) */
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    MSG_SetSeqNo(MSG_ScratchBuffer,KNXTlcGetSequenceNumberSend());
    (void)MSG_Post(MSG_ScratchBuffer);

    KNXTlcSetRepetitionCount((uint8)0);

    StartAcknowledgementTimeoutTimer();
    RestartConnectionTimeoutTimer();
}

static void A8(void)  /* Nur local-user (Client only). */
/*
**
** Dieser Event bedeuted, das das vorherige Telegramm korrekt geACKt wurde, also einen 'T_DATA_CONNECTED.CON' an den User.
*/
{
    StopAcknowledgementTimeoutTimer();
    KNXTlcSetSequenceNumberSend(KNXTlcGetSequenceNumberSend()+1);

    RestoreMessage();        

/*      Send the stored buffer as a T_Data_Connected.con with cleared errorbits, */
/*      connection number = 0 to the user. */

/*  Hinweis: mit 'errorbits' ist das Bit #0 des Control-Fields gemeint (wieso Plural???). */

/* dest: connection_addr, sequence=SeqNoSend. */

    RestartConnectionTimeoutTimer();
}

static void A8b(void)        /* Nur local-user (Client only). */
						/* Hinweis: nicht referenziert!!! */
{
    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);    /* Puffer freigeben, da dieser Event keine Message erzeugt. */
    StopAcknowledgementTimeoutTimer();
    KNXTlcSetSequenceNumberSend(KNXTlcGetSequenceNumberSend()+1);    
    RestartConnectionTimeoutTimer();
}

static void A9(void)  /* Nur local-user (Client only). */
{ 
    if (MSG_ScratchBuffer==(PMSG_Buffer)NULL) {
        MSG_ScratchBuffer=MSG_AllocateBuffer();  /* Im Falle eines Timeouts existiert kein Puffer. */
    } else {
        (void)MSG_ClearBuffer(MSG_ScratchBuffer);
    }

/*  Send the stored message as a N_Data_Individual.req to the network layer (remote device). */
    RestoreMessage();
    MSG_SetSeqNo(MSG_ScratchBuffer,KNXTlcGetSequenceNumberSend());   /* check: korrekt??? */
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
    
    KNXTlcSetRepetitionCount(KNXTlcGetRepetitionCount()+1);
    
    StartAcknowledgementTimeoutTimer();
    RestartConnectionTimeoutTimer();
}

static void A10(void)  /* GO AWAY!!! */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU Priority = SYSTEM, */
/*      Destination = source (rbuffer), Sequence = 0 back to sender. */
    
    Knx_AddressType source,dest;

    source=MSG_GetSourceAddress(MSG_ScratchBuffer);
    dest=MSG_GetDestAddress(MSG_ScratchBuffer); 

    (void)MSG_ClearBuffer(MSG_ScratchBuffer);
    T_Disconnect_Req(MSG_ScratchBuffer,dest,source);
    
}

static void A11(void)  /* (Client only???) */
                        /* Hinweis: verwendet keinen Message-Buffer!!! */
{
/* Store event back and handle after next event. Don’t change order of T_Data_Connected.req events. */


    (void)MSG_Post(MSG_ScratchBuffer);   /* push-bask event (check: funktioniert das so?) */
}

static void A12(void)  /* (Client only.) */
{     
    KNXTlcSetConnectionAddress(KNXTlcGetSourceAddress());   /* connection_address=address from T_CONNECT_requ */
    
/*  send N_Data_Individual.req with T_CONNECT_REQ_PDU */
    (void)MSG_ClearBuffer(MSG_ScratchBuffer);
    T_Connect_Req(MSG_ScratchBuffer,ADR_GetPhysAddr(),KNXTlcGetConnectionAddress());

    KNXTlcSetSequenceNumberSend((uint8)0);
    KNXTlcSetSequenceNumberReceived((uint8)0);
    StartConnectionTimeoutTimer();
}

void A13(void)  /* Nur local-user (Client only). */
{
/*     Send a T_Connect.con to the user. */
    MSG_ScratchBuffer->service=T_CONNECT_CON;
    (void)MSG_Post(MSG_ScratchBuffer);             
}

static void A14(void)    /* Nur local-user (Client only). */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM, */
/*      destination = connection_address, sequence = 0 to the network layer (remote device). */

/*      Send a T_Disconnect.con to the user. */

        StopAcknowledgementTimeoutTimer();
        StopConnectionTimeoutTimer();
}

static void A14b(void)  /* wie A14, nur ohne T_DISCONNECT_CON to user. */
                        /* Hinweis: nicht referenziert!!! */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM, */
/*      destination = connection_address, sequence = 0 to the network layer (remote device). */

        StopAcknowledgementTimeoutTimer();
        StopConnectionTimeoutTimer();
}

static void A15(void)   /* Nur local-user (Client only). */
{
/*      Send a T_Disconnect.con to the management user */

        StopAcknowledgementTimeoutTimer();
        StopConnectionTimeoutTimer();
}


/*
**
** Event-Handler.
**
*/
static uint8 EventConnectInd(void)
{
    if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
        return (uint8)0;
    } else {
        return (uint8)1;
    }
}


static uint8 EventDisconnectInd(void)
{
    if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
        return (uint8)2;
    } else {
        return (uint8)3;
    }    
}


static uint8 EventDataConnectedInd(void)
{
    uint8 event_num;
    /* todo: die 'if'-Anweisungen checken!!! */
    if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
        if (KNXTlcGetSequenceNumberOfPDU()==KNXTlcGetSequenceNumberReceived()) {
            event_num=(uint8)4;
        } else if (KNXTlcGetSequenceNumberOfPDU()==((KNXTlcGetSequenceNumberReceived()-1)&0x0f)) {
            event_num=(uint8)5;                        
        } else if ((KNXTlcGetSequenceNumberOfPDU()!=KNXTlcGetSequenceNumberReceived()-1) 
            && (KNXTlcGetSequenceNumberOfPDU()!=((KNXTlcGetSequenceNumberReceived()-1)&0x0f))) {
            event_num=(uint8)6;
        } else {
            event_num=(uint8)27;
        }
    } else {
        event_num=(uint8)7;
    }
    
    return event_num;
}


static uint8 EventAckInd(void)
{
    uint8 event_num;

    if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
        if (KNXTlcGetSequenceNumberOfPDU()==KNXTlcGetSequenceNumberSend()) {
            event_num=(uint8)8;
        } else {
            event_num=(uint8)9;
        }
    } else {
        event_num=(uint8)10;
    }
    return event_num;     
}


static uint8 EventNakInd(void)
{
    uint8 event_num;

    if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
        if (KNXTlcGetSequenceNumberOfPDU()!=KNXTlcGetSequenceNumberSend()) {
            event_num=(uint8)11;
        } else {
            if (KNXTlcGetRepetitionCount()<MAX_REP_COUNT) {
                event_num=(uint8)12;
            } else {
                event_num=(uint8)13;
            }
        }
    } else {
        event_num=(uint8)14;
    }
    return event_num;    
}


static uint8 EventConnectReq(void)
{
    /* Client-only. */
    /* Event 25.    */
    return (uint8)25;
}


static uint8 EventDisconnectReq(void)
{
    /* Client-only. */
    /* Event 26.    */
    return (uint8)26;
}


static uint8 EventDataConnectedReq(void)
{
    /* Client-only. */
    /* Event 15.    */
    return (uint8)15;
}


static uint8 EventConnectCon(void)
{
    /* Client only.         */
    /* Events 19 und 20.    */
    if (MSG_GetIAK(MSG_ScratchBuffer)==IAK_OK) {
        return (uint8)19;
    } else {
        return (uint8)20;
    }
}      


static uint8 EventDisconnectCon(void)
{
    /* Event 21.    */
    return (uint8)21;
}


static uint8 EventDataConnectedCon(void)
{
    /* Event 22.    */
    return (uint8)22;
}


static uint8 EventAckCon(void)
{
    /* Event 23.    */
    return (uint8)23;
}


static uint8 EventNakCon(void)
{
    /* Event 24.    */
    return (uint8)24;    
}


static uint8 EventTimeoutCon(void)
{
    return (uint8)16;    
}


static uint8 EventTimeoutAck(void)
{
    if (KNXTlcGetRepetitionCount()<MAX_REP_COUNT) {
        return (uint8)17;
    } else {
        return (uint8)18;
    }  
}


static uint8 EventUndefined(void)
{
    return (uint8)27; 
}





void TL_StateMachine(KNX_TlcEventType event)
{
        uint8 event_num;
        KNX_TlcActionType action;               

        if (event<tlcUNDEFINED) {
            event_num=TLC_Events[event]();
        } else {
            event_num=EventUndefined();
        }
#if 0        
        switch (event) {
            case tlcCONNECT_IND:
                if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
                    event_num=(uint8)0;
                } else {
                    event_num=(uint8)1;
                }
                break;
            case tlcDISCONNECT_IND:
                if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
                    event_num=(uint8)2;
                } else {
                    event_num=(uint8)3;
                }
                break;
            case tlcDATA_CONNECTED_IND:
              /* todo: die 'if'-Anweisungen checken!!! */
                if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
                    if (KNXTlcGetSequenceNumberOfPDU()==KNXTlcGetSequenceNumberReceived()) {
                        event_num=(uint8)4;
                    } else if (KNXTlcGetSequenceNumberOfPDU()==((KNXTlcGetSequenceNumberReceived()-1)&0x0f)) {
                        event_num=(uint8)5;                        
                    } else if ((KNXTlcGetSequenceNumberOfPDU()!=KNXTlcGetSequenceNumberReceived()-1) 
                               && (KNXTlcGetSequenceNumberOfPDU()!=((KNXTlcGetSequenceNumberReceived()-1)&0x0f))) {
                        event_num=(uint8)6;
                    } else {
                        event_num=(uint8)27;
                    }
                } else {
                    event_num=(uint8)7;
                }
                break;
            case tlcACK_IND:
                if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
                    if (KNXTlcGetSequenceNumberOfPDU()==KNXTlcGetSequenceNumberSend()) {
                        event_num=(uint8)8;
                    } else {
                        event_num=(uint8)9;
                    }
                } else {
                    event_num=(uint8)10;
                }
                break;
            case tlcNAK_IND:
                if (KNXTlcGetSourceAddress()==KNXTlcGetConnectionAddress()) {
                    if (KNXTlcGetSequenceNumberOfPDU()!=KNXTlcGetSequenceNumberSend()) {
                        event_num=(uint8)11;
                    } else {
                        if (KNXTlcGetRepetitionCount()<MAX_REP_COUNT) {
                            event_num=(uint8)12;
                        } else {
                            event_num=(uint8)13;
                        }
                    }
                } else {
                    event_num=(uint8)14;
                }                       
                break;

/*
**
**  !!! IMPLEMENTIEREN !!!
**
*/
            case tlcCONNECT_REQ:         /* Client-only. */
            /* Event 25. */
                event_num=(uint8)25;
                break;
            case tlcDISCONNECT_REQ:      /* Client-only. */
            /* Event 26. */
                event_num=(uint8)26;
                break;
            case tlcDATA_CONNECTED_REQ:  /* Client-only. */
            /* Event 15. */
                event_num=(uint8)15;
                break;
            case tlcCONNECT_CON:         /* Client only. */
            /* Events 19 und 20. */
                break;
            case tlcDISCONNECT_CON:
            /* Event 21. */
                event_num=(uint8)21;
                break;
            case tlcDATA_CONNECTED_CON:
            /* Event 22. */
                event_num=(uint8)22;
                break;
            case tlcACK_CON:
            /* Event 23. */
                event_num=(uint8)23;
                break;            
            case tlcNAK_CON:
            /* Event 24. */
                event_num=(uint8)24;
                break;                            
/*
**
**
**
*/
            case tlcTIMEOUT_CON:
                event_num=(uint8)16;
                break;
                
            case tlcTIMEOUT_ACK:
                if (KNXTlcGetRepetitionCount()<MAX_REP_COUNT) {
                    event_num=(uint8)17;
                } else {
                    event_num=(uint8)18;
                }
                break;

                
            case tlcUNDEFINED:
            default:        /* !!! Schwerwiegender Fehler !!! */
                event_num=(uint8)27;
                break;
        }
#endif
        action=Actions[event_num].Action[KNXTlcGetState()];
/* '__tlState' auf Gültigkeit prüfen (darf nur bei Style #3 'CONNECTING' sein)!!! */
        KNXTlcSetState(action.Next);
        action.Function();
}
