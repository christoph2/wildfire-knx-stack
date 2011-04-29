/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <chris@konnex-tools.de,
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
**
**      Konnex Transport-Layer-Statemachines.
**
**/

#include "knx_layer_transport.h"

static KnxTLC_StateType KnxTLC_State;

static Knx_MessageType _StoredMsg;     /* Client-only. */

static void StartConnectionTimeoutTimer(void);
static void RestartConnectionTimeoutTimer(void);
static void StopConnectionTimeoutTimer(void);
static void StartAcknowledgementTimeoutTimer(void);
static void StopAcknowledgementTimeoutTimer(void);

void    T_Disconnect_Ind(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
void    T_Disconnect_Con(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);

typedef uint8 (*EVENT_FUNC)(void);

static uint8    EventConnectInd(void), EventDisconnectInd(void), EventDataConnectedInd(void);
static uint8    EventAckInd(void), EventNakInd(void), EventConnectReq(void);
static uint8    EventDisconnectReq(void), EventDataConnectedReq(void), EventConnectCon(void);
static uint8    EventDisconnectCon(void), EventDataConnectedCon(void), EventAckCon(void);
static uint8    EventNakCon(void), EventTimeoutCon(void), EventTimeoutAck(void);
static uint8    EventUndefined(void);

static void A0(void), A1(void), A2(void), A3(void), A4(void), A5(void), A6(void), A7(void), A8(void);
static void A8b(void), A9(void), A10(void), A11(void), A12(void), A13(void), A14(void), A14b(void), A15(void);

#if TL_STYLE == 3
/* Transport-Layer-Statemachine-Style #3 */
static const KnxTLC_ActionListType Actions[] = {
    { /* 0,  */ {{A1, OPEN_IDLE}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }, {A0,  CONNECTING    }}        },
    { /* 1,  */ {{A1, OPEN_IDLE}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT}, {A10, CONNECTING   }}        },
    { /* 2,  */ {{A0, CLOSED}, {A5, CLOSED }, {A5, CLOSED  },        {A5,  CLOSED        }}        },
    { /* 3,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A0,  CONNECTING    }}        },
    { /* 4,  */ {{A0, CLOSED}, {A2, OPEN_IDLE}, {A2, OPEN_WAIT },    {A6,  CLOSED        }}        },
    { /* 5,  */ {{A0, CLOSED}, {A3, OPEN_IDLE}, {A3, OPEN_WAIT },    {A3,  CONNECTING    }}        },
    { /* 6,  */ {{A0, CLOSED}, {A4, OPEN_IDLE}, {A4, OPEN_WAIT },    {A6,  CONNECTING    }}        },
    { /* 7,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A10, CONNECTING    }}        },
    { /* 8,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A8, OPEN_IDLE },    {A6,  CLOSED        }}        },
    { /* 9,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A6, CLOSED },       {A6,  CLOSED        }}        },
    { /* 10, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A10, CONNECTING    }}        },
    { /* 11, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A6,  CLOSED        }}        },
    { /* 12, */ {{A0, CLOSED}, {A6, CLOSED }, {A9, OPEN_WAIT },      {A6,  CLOSED        }}        },
    { /* 13, */ {{A0, CLOSED}, {A6, CLOSED }, {A6, CLOSED  },        {A6,  CLOSED        }}        },
    { /* 14, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A10, CONNECTING    }}        },
    { /* 15, */ {{A0, CLOSED}, {A7, OPEN_WAIT}, {A11, OPEN_WAIT },   {A11, CONNECTING    }}        },
    { /* 16, */ {{A0, CLOSED}, {A6, CLOSED }, {A6, CLOSED  },        {A6,  CLOSED        }}        },
    { /* 17, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A9, OPEN_WAIT },    {A0,  CONNECTING    }}        },
    { /* 18, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A6, CLOSED },       {A0,  CONNECTING    }}        },
    { /* 19, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A13, OPEN_IDLE     }}        },
    { /* 20, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A5,  CLOSED        }}        },
    { /* 21, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A0,  CONNECTING    }}        },
    { /* 22, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A0,  CONNECTING    }}        },
    { /* 23, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A0,  CONNECTING    }}        },
    { /* 24, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A0,  CONNECTING    }}        },
    { /* 25, */ {{A12, CONNECTING}, {A6, CLOSED}, {A6, CLOSED },     {A6,  CLOSED        }}        },
    { /* 26, */ {{A15, CLOSED}, {A14, CLOSED }, {A14, CLOSED },      {A14, CLOSED        }}        },
    { /* 27, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },    {A0,  CONNECTING    }}        },
};
#elif (TL_STYLE == 1) || (TL_STYLE == 2)
/* Transport-Layer-Statemachine, Styles #1 and #2 */
static const ACTION_LIST Actions[] = {
#if     TL_STYLE == 1
    { /* 0,  */ {{A1, OPEN_IDLE}, {A6, CLOSED }, {A6, CLOSED }}      },
#else
/* #if TL_STYLE==2 */
    { /* 0,  */ {{A1, OPEN_IDLE}, {A0, OPEN_IDLE}, {A0, OPEN_IDLE }} },
#endif

#if     TL_STYLE == 1
    { /* 1,  */ {{A1, OPEN_IDLE}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT}} },
#else
    { /* 1,  */ {{A1, OPEN_IDLE}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }} },
#endif
    { /* 2,  */ {{A0, CLOSED}, {A5, CLOSED }, {A5, CLOSED  }}        },
    { /* 3,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
#if     TL_STYLE == 1
    { /* 4,  */ {{A10, CLOSED}, {A2, OPEN_IDLE}, {A2, OPEN_WAIT }}   },
#else
    { /* 4,  */ {{A0, CLOSED}, {A2, OPEN_IDLE}, {A2, OPEN_WAIT }}    },
#endif
#if     TL_STYLE == 1
    { /* 5,  */ {{A10, CLOSED}, {A3, OPEN_IDLE}, {A3, OPEN_WAIT }}   },
#else
    { /* 5,  */ {{A0, CLOSED}, {A3, OPEN_IDLE}, {A3, OPEN_WAIT }}    },
#endif
#if     TL_STYLE == 1
    { /* 6,  */ {{A10, CLOSED}, {A4, OPEN_IDLE}, {A4, OPEN_WAIT }}   },
#else
    { /* 6,  */ {{A0, CLOSED}, {A4, OPEN_IDLE}, {A4, OPEN_WAIT }}    },
#endif
#if     TL_STYLE == 1
    { /* 7,  */ {{A10, CLOSED}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT }} },
#else
    { /* 7,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
#endif
#if     TL_STYLE == 1
    { /* 8,  */ {{A10, CLOSED}, {A6, CLOSED }, {A8, OPEN_IDLE }}     },
#else
    { /* 8,  */ {{A0, CLOSED}, {A6, CLOSED }, {A8b, OPEN_IDLE }}     },
#endif
#if     TL_STYLE == 1
    { /* 9,  */ {{A10, CLOSED}, {A6, CLOSED }, {A6, CLOSED }}        },
#else
    { /* 9,  */ {{A0, CLOSED}, {A6, CLOSED }, {A0, OPEN_WAIT }}      },
#endif
#if     TL_STYLE == 1
    { /* 10, */ {{A10, CLOSED}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT }} },
#else
    { /* 10, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
#endif
#if     TL_STYLE == 1
    { /* 11, */ {{A10, CLOSED}, {A6, CLOSED }, {A6, CLOSED }}        },
#else
    { /* 11, */ {{A0, CLOSED}, {A6, CLOSED }, {A0, OPEN_WAIT }}      },
#endif
#if     TL_STYLE == 1
    { /* 12, */ {{A10, CLOSED}, {A6, CLOSED }, {A9, OPEN_WAIT }}     },
#else
    { /* 12, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A9, OPEN_WAIT }}    },
#endif
#if     TL_STYLE == 1
    { /* 13, */ {{A10, CLOSED}, {A6, CLOSED }, {A6, CLOSED }}        },
#else
    { /* 13, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A6, CLOSED }}       },
#endif
#if     TL_STYLE == 1
    { /* 14, */ {{A10, CLOSED}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT }} },
#else
    { /* 14, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
#endif
#if     TL_STYLE == 1
    { /* 15, */ {{A5, CLOSED}, {A7, OPEN_WAIT}, {A6, CLOSED }}       },
#else
    { /* 15, */ {{A0, CLOSED}, {A7, OPEN_WAIT}, {A11, OPEN_WAIT }}   },
#endif
    { /* 16, */ {{A0, CLOSED}, {A6, CLOSED }, {A6, CLOSED  }}        },
    { /* 17, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A9, OPEN_WAIT }}    },
    { /* 18, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A6, CLOSED }}       },
#if     TL_STYLE == 1
    { /* 19, */ {{A0, CLOSED}, {A13, OPEN_IDLE}, {A13, OPEN_WAIT }}  },
#else
    { /* 19, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
#endif
    { /* 20, */ {{A0, CLOSED}, {A5, CLOSED }, {A5, CLOSED  }}        },
    { /* 21, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
    { /* 22, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
    { /* 23, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
    { /* 24, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
#if     TL_STYLE == 1
    { /* 25, */ {{A12, OPEN_IDLE}, {A6, CLOSED}, {A6, CLOSED }}      },
#else
    { /* 25, */ {{A12, OPEN_IDLE}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT}} },
#endif
#if     TL_STYLE == 1
    { /* 26, */ {{A15, CLOSED}, {A14, CLOSED }, {A14, CLOSED }}      },
#else
    { /* 26, */ {{A0, CLOSED}, {A14b, CLOSED }, {A11, OPEN_WAIT }}   },
#endif
    { /* 27, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}    },
};
#else
#error "Invalid TL_STYLE [1|2|3]"
#endif

static const EVENT_FUNC TLC_Events[] = {
/*      Event                       Handler                 */
/*      ====================================================*/
/*      tlcCONNECT_IND          */ EventConnectInd,
/*      tlcDISCONNECT_IND       */ EventDisconnectInd,
/*      tlcDATA_CONNECTED_IND   */ EventDataConnectedInd,
/*      tlcACK_IND              */ EventAckInd,
/*      tlcNAK_IND              */ EventNakInd,
/*      tlcCONNECT_REQ          */ EventConnectReq,
/*      tlcDISCONNECT_REQ       */ EventDisconnectReq,
/*      tlcDATA_CONNECTED_REQ   */ EventDataConnectedReq,
/*      tlcCONNECT_CON          */ EventConnectCon,
/*      tlcDISCONNECT_CON       */ EventDisconnectCon,
/*      tlcDATA_CONNECTED_CON   */ EventDataConnectedCon,
/*      tlcACK_CON              */ EventAckCon,
/*      tlcNAK_CON              */ EventNakCon,
/*      tlcTIMEOUT_CON          */ EventTimeoutCon,
/*      tlcTIMEOUT_ACK          */ EventTimeoutAck,
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

#define StoreMessage()      Utl_MemCopy(_StoredMsg, KnxMSG_ScratchBufferPtr->msg, MSG_LEN)
#define RestoreMessage()    Utl_MemCopy(KnxMSG_ScratchBufferPtr->msg, _StoredMsg, MSG_LEN)

KnxTLC_StateType KnxTLC_GetState(void)
{
    return KnxTLC_State;
}

void KnxTLC_SetState(KnxTLC_StateType State)
{
    KnxTLC_State = State;
}

void T_Disconnect_Ind(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
{
    KnxMSG_SetTPCI(pBuffer, T_DISCONNECT_REQ_PDU);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);

    KnxMSG_GetMessagePtr(pBuffer)->ctrl    = (uint8)0xB0; /* short-cuts... */
    KnxMSG_GetMessagePtr(pBuffer)->ncpi    = (uint8)0x60;

    KnxMSG_SetLen(pBuffer, (uint8)7);
    pBuffer->service = T_DISCONNECT_IND;

    (void)KnxMSG_Post(pBuffer);
}

void T_Disconnect_Con(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
{
    KnxMSG_SetTPCI(pBuffer, T_DISCONNECT_REQ_PDU);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);

    KnxMSG_GetMessagePtr(pBuffer)->ctrl    = (uint8)0xb0; /* short-cuts... */
    KnxMSG_GetMessagePtr(pBuffer)->ncpi    = (uint8)0x60;

    KnxMSG_SetLen(pBuffer, (uint8)7);
    pBuffer->service = T_DISCONNECT_CON;

    (void)KnxMSG_Post(pBuffer);
}

static void StartConnectionTimeoutTimer(void)
{
    (void)TM_Start(TM_TIMER_TLC_CON_TIMEOUT, TM_BASE_SEC, TLC_CONNECTION_TIMEOUT);
}

static void RestartConnectionTimeoutTimer(void)
{
    (void)TM_Stop(TM_TIMER_TLC_CON_TIMEOUT);
    (void)TM_Start(TM_TIMER_TLC_CON_TIMEOUT, TM_BASE_SEC, TLC_CONNECTION_TIMEOUT);
}

static void StopConnectionTimeoutTimer(void)
{
    (void)TM_Stop(TM_TIMER_TLC_CON_TIMEOUT);
}

static void StartAcknowledgementTimeoutTimer(void)
{
    (void)TM_Start(TM_TIMER_TLC_ACK_TIMEOUT, TM_BASE_SEC, TLC_ACKNOWLEDGE_TIMEOUT);
}

static void StopAcknowledgementTimeoutTimer(void)
{
    (void)TM_Stop(TM_TIMER_TLC_ACK_TIMEOUT);
}

void A0(void)
{
    /* do nothing. */
    if (KnxMSG_ScratchBufferPtr != (KnxMSG_BufferPtr)NULL) {
        (void)KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
    }
}

void A1(void)
{
    KnxTLC_SetConnectionAddress(KnxTLC_GetSourceAddress());

/* Send a T_CONNECT_ind to the user. */
    KnxMSG_ScratchBufferPtr->service = T_CONNECT_IND;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);

    KnxTLC_SetSequenceNumberSend((uint8)0);
    KnxTLC_SetSequenceNumberReceived((uint8)0);
    StartConnectionTimeoutTimer();
}

void A2(void)
{
    KnxMSG_BufferPtr pBuffer;

/*      Send a N_Data_Individual.req with T_ACK_PDU, priority = SYSTEM, destination = */
/*      connection_address, sequence =SeqNoRcv to the network layer (remote device).  */
    pBuffer = KnxMSG_AllocateBuffer();

    if (pBuffer != (KnxMSG_BufferPtr)NULL) {
        T_Ack_Req(pBuffer, KnxADR_GetPhysAddr(), KnxTLC_GetConnectionAddress(), KnxTLC_GetSequenceNumberReceived());
        KnxTLC_SetSequenceNumberReceived(KnxTLC_GetSequenceNumberReceived() + 1);
    } else {
        /* Errorhandling. */
    }

/*      Send the received buffer as a T_Data_Connected.ind to the user. */
    KnxMSG_ScratchBufferPtr->service = T_DATA_CONNECTED_IND;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);

    RestartConnectionTimeoutTimer();
}

void A3(void)
{
/*
**      Send an N_Data_Individual.req with T_ACK_PDU, priority = SYSTEM, destination =
**      connection_address, sequence = sequence of received message to the network
**      layer (remote device).
*/

    (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    T_Ack_Req(KnxMSG_ScratchBufferPtr, KnxADR_GetPhysAddr(), KnxTLC_GetConnectionAddress(), KnxTLC_GetSequenceNumberReceived());
    RestartConnectionTimeoutTimer();
}

void A4(void)
{
/*
**      Send an N_Data_Individual.req with T_NAK_PDU, priority = SYSTEM, destination =
**      connection_address, sequence = sequence of received message to the network
**      layer (remote device).
*/

    (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    T_Nak_Req(KnxMSG_ScratchBufferPtr, KnxADR_GetPhysAddr(), KnxTLC_GetConnectionAddress(), KnxTLC_GetSequenceNumberReceived());

    RestartConnectionTimeoutTimer();
}

void A5(void)
{
    /* Send a T_Disconnect.ind to the user. */
    (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    T_Disconnect_Ind(KnxMSG_ScratchBufferPtr, KnxTLC_GetConnectionAddress(), KnxADR_GetPhysAddr());

    StopAcknowledgementTimeoutTimer();
    StopConnectionTimeoutTimer();
}

void A6(void)
{
    KnxMSG_BufferPtr pBuffer;

/*
**      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM,
**      destination = connection_address, sequence = 0 to the network layer (remote device).
*/

    pBuffer = KnxMSG_AllocateBuffer();

    if (pBuffer != (KnxMSG_BufferPtr)NULL) {
        T_Disconnect_Req(pBuffer, KnxADR_GetPhysAddr(), KnxTLC_GetConnectionAddress());
    } else {
        /* Errorhandling. */
    }

/* Send a T_Disconnect.ind to the user. */
    if (KnxMSG_ScratchBufferPtr == (KnxMSG_BufferPtr)NULL) {
        KnxMSG_ScratchBufferPtr = KnxMSG_AllocateBuffer();
    } else {
        (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    }

    T_Disconnect_Ind(KnxMSG_ScratchBufferPtr, KnxADR_GetPhysAddr(), KnxTLC_GetConnectionAddress());

    StopAcknowledgementTimeoutTimer();
    StopConnectionTimeoutTimer();
}

void A7(void)   /* Nur local-user (Client only). */
{
/*
**              Store the received T_Data_Connected.req and send as a N_Data_Individual.req
**              with T_DATA_CONNECTED_REQ_PDU,destination = connection_address,
**              sequence = SeqNoSend to the network layer (remote device).
*/

    StoreMessage();

/*    MSG_ScratchBuffer->service=T_DATA_CONNECTED_REQ; */
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    KnxMSG_SetSeqNo(KnxMSG_ScratchBufferPtr, KnxTLC_GetSequenceNumberSend());
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);

    KnxTLC_SetRepetitionCount((uint8)0);

    StartAcknowledgementTimeoutTimer();
    RestartConnectionTimeoutTimer();
}

void A8(void)  /* only local-user (Client only). */
{
    StopAcknowledgementTimeoutTimer();
    KnxTLC_SetSequenceNumberSend(KnxTLC_GetSequenceNumberSend() + (uint8)1);

    RestoreMessage();

/* Send the stored buffer as a T_Data_Connected.con with cleared errorbits, */
/* connection number = 0 to the user. */

/* dest: connection_addr, sequence=SeqNoSend. */

    RestartConnectionTimeoutTimer();
}

void A8b(void)        /* only local-user (Client only). */
{
    (void)KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
    StopAcknowledgementTimeoutTimer();
    KnxTLC_SetSequenceNumberSend(KnxTLC_GetSequenceNumberSend() + (uint8)1);
    RestartConnectionTimeoutTimer();
}

void A9(void)  /* only local-user (Client only). */
{
    if (KnxMSG_ScratchBufferPtr == (KnxMSG_BufferPtr)NULL) {
        KnxMSG_ScratchBufferPtr = KnxMSG_AllocateBuffer();
    } else {
        (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    }

/*  Send the stored message as a N_Data_Individual.req to the network layer (remote device). */
    RestoreMessage();
    KnxMSG_SetSeqNo(KnxMSG_ScratchBufferPtr, KnxTLC_GetSequenceNumberSend());
    KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);

    KnxTLC_SetRepetitionCount(KnxTLC_GetRepetitionCount() + (uint8)1);

    StartAcknowledgementTimeoutTimer();
    RestartConnectionTimeoutTimer();
}

void A10(void)  /* GO AWAY!!! */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU Priority = SYSTEM, */
/*      Destination = source (rbuffer), Sequence = 0 back to sender. */

    Knx_AddressType source, dest;

    source = KnxMSG_GetSourceAddress(KnxMSG_ScratchBufferPtr);
    dest   = KnxMSG_GetDestAddress(KnxMSG_ScratchBufferPtr);

    (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    T_Disconnect_Req(KnxMSG_ScratchBufferPtr, dest, source);

}

void A11(void)  /* (Client only???) */
{
/* Store event back and handle after next event. Don�t change order of T_Data_Connected.req events. */

    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);   /* push-bask event. */
}

void A12(void)                                              /* (Client only.) */
{
    KnxTLC_SetConnectionAddress(KnxTLC_GetSourceAddress()); /* connection_address=address from T_CONNECT_requ */

/*  send N_Data_Individual.req with T_CONNECT_REQ_PDU */
    (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    T_Connect_Req(KnxMSG_ScratchBufferPtr, KnxADR_GetPhysAddr(), KnxTLC_GetConnectionAddress());

    KnxTLC_SetSequenceNumberSend((uint8)0);
    KnxTLC_SetSequenceNumberReceived((uint8)0);
    StartConnectionTimeoutTimer();
}

void A13(void)  /* Nur local-user (Client only). */
{
/*     Send a T_Connect.con to the user. */
    KnxMSG_ScratchBufferPtr->service = T_CONNECT_CON;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}

void A14(void)    /* only local-user (Client only). */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM, */
/*      destination = connection_address, sequence = 0 to the network layer (remote device). */

/*      Send a T_Disconnect.con to the user. */

    StopAcknowledgementTimeoutTimer();
    StopConnectionTimeoutTimer();
}

void A14b(void)  /* wie A14, nur ohne T_DISCONNECT_CON to user. */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM, */
/*      destination = connection_address, sequence = 0 to the network layer (remote device). */

    StopAcknowledgementTimeoutTimer();
    StopConnectionTimeoutTimer();
}

void A15(void)   /* only local-user (Client only). */
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
    if (KnxTLC_GetSourceAddress() == KnxTLC_GetConnectionAddress()) {
        return (uint8)0;
    } else {
        return (uint8)1;
    }
}

static uint8 EventDisconnectInd(void)
{
    if (KnxTLC_GetSourceAddress() == KnxTLC_GetConnectionAddress()) {
        return (uint8)2;
    } else {
        return (uint8)3;
    }
}

static uint8 EventDataConnectedInd(void)
{
    uint8 event_num;

    if (KnxTLC_GetSourceAddress() == KnxTLC_GetConnectionAddress()) {
        if (KnxTLC_GetSequenceNumberOfPDU() == KnxTLC_GetSequenceNumberReceived()) {
            event_num = (uint8)4;
        } else if (KnxTLC_GetSequenceNumberOfPDU() == ((KnxTLC_GetSequenceNumberReceived() - (uint8)1) & (uint8)0x0f)) {
            event_num = (uint8)5;
        } else if ((KnxTLC_GetSequenceNumberOfPDU() != KnxTLC_GetSequenceNumberReceived() - (uint8)1)
                   && (KnxTLC_GetSequenceNumberOfPDU() != ((KnxTLC_GetSequenceNumberReceived() - (uint8)1) & (uint8)0x0f)))
        {
            event_num = (uint8)6;
        } else {
            event_num = (uint8)27;
        }
    } else {
        event_num = (uint8)7;
    }

    return event_num;
}

static uint8 EventAckInd(void)
{
    uint8 event_num;

    if (KnxTLC_GetSourceAddress() == KnxTLC_GetConnectionAddress()) {
        if (KnxTLC_GetSequenceNumberOfPDU() == KnxTLC_GetSequenceNumberSend()) {
            event_num = (uint8)8;
        } else {
            event_num = (uint8)9;
        }
    } else {
        event_num = (uint8)10;
    }

    return event_num;
}

static uint8 EventNakInd(void)
{
    uint8 event_num;

    if (KnxTLC_GetSourceAddress() == KnxTLC_GetConnectionAddress()) {
        if (KnxTLC_GetSequenceNumberOfPDU() != KnxTLC_GetSequenceNumberSend()) {
            event_num = (uint8)11;
        } else {
            if (KnxTLC_GetRepetitionCount() < MAX_REP_COUNT) {
                event_num = (uint8)12;
            } else {
                event_num = (uint8)13;
            }
        }
    } else {
        event_num = (uint8)14;
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
    if (KnxMSG_GetIAK(KnxMSG_ScratchBufferPtr) == IAK_OK) {
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
    if (KnxTLC_GetRepetitionCount() < MAX_REP_COUNT) {
        return (uint8)17;
    } else {
        return (uint8)18;
    }
}

static uint8 EventUndefined(void)
{
    return (uint8)27;
}

void KnxTLC_StateMachine(KNX_TlcEventType event)
{
    uint8               event_num;
    KnxTLC_ActionType   action;

    if (event < tlcUNDEFINED) {
        event_num = TLC_Events[event]();
    } else {
        event_num = EventUndefined();
    }

    action = Actions[event_num].Action[KnxTLC_GetState()];
    KnxTLC_SetState(action.Next);
    action.Function();
}

