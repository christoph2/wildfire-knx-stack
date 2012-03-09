/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2012 by Christoph Schueler <chris@konnex-tools.de,
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


/*
** Local variables.
*/
STATIC KnxTLC_StateType KnxTLC_State;
STATIC Knx_MessageType _StoredMsg;     /* Client-only. */


/*
** Local types.
*/
typedef uint8 (*EVENT_FUNC)(void);


/*
** Local function prototypes.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE)     StartConnectionTimeoutTimer(void);
STATIC FUNC(void, KSTACK_CODE)     RestartConnectionTimeoutTimer(void);
STATIC FUNC(void, KSTACK_CODE)     StopConnectionTimeoutTimer(void);
STATIC FUNC(void, KSTACK_CODE)     StartAcknowledgementTimeoutTimer(void);
STATIC FUNC(void, KSTACK_CODE)     StopAcknowledgementTimeoutTimer(void);

STATIC FUNC(uint8, KSTACK_CODE)    EventConnectInd(void), EventDisconnectInd(void), EventDataConnectedInd(void);
STATIC FUNC(uint8, KSTACK_CODE)    EventAckInd(void), EventNakInd(void), EventConnectReq(void);
STATIC FUNC(uint8, KSTACK_CODE)    EventDisconnectReq(void), EventDataConnectedReq(void), EventConnectCon(void);
STATIC FUNC(uint8, KSTACK_CODE)    EventDisconnectCon(void), EventDataConnectedCon(void), EventAckCon(void);
STATIC FUNC(uint8, KSTACK_CODE)    EventNakCon(void), EventTimeoutCon(void), EventTimeoutAck(void);
STATIC FUNC(uint8, KSTACK_CODE)    EventUndefined(void);

STATIC FUNC(void, KSTACK_CODE)     A0(void), A1(void), A2(void), A3(void), A4(void), A5(void), A6(void), A7(void), A8(void);
STATIC FUNC(void, KSTACK_CODE)     A8b(void), A9(void), A10(void), A11(void), A12(void), A13(void), A14(void), A14b(void), A15(void);

FUNC(void, KSTACK_CODE)                T_Disconnect_Ind(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);   /* TODO: include file. */
FUNC(void, KSTACK_CODE)                T_Disconnect_Con(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
#else
STATIC void    StartConnectionTimeoutTimer(void);
STATIC void    RestartConnectionTimeoutTimer(void);
STATIC void    StopConnectionTimeoutTimer(void);
STATIC void    StartAcknowledgementTimeoutTimer(void);
STATIC void    StopAcknowledgementTimeoutTimer(void);

STATIC uint8    EventConnectInd(void), EventDisconnectInd(void), EventDataConnectedInd(void);
STATIC uint8    EventAckInd(void), EventNakInd(void), EventConnectReq(void);
STATIC uint8    EventDisconnectReq(void), EventDataConnectedReq(void), EventConnectCon(void);
STATIC uint8    EventDisconnectCon(void), EventDataConnectedCon(void), EventAckCon(void);
STATIC uint8    EventNakCon(void), EventTimeoutCon(void), EventTimeoutAck(void);
STATIC uint8    EventUndefined(void);

STATIC void    A0(void), A1(void), A2(void), A3(void), A4(void), A5(void), A6(void), A7(void), A8(void);
STATIC void    A8b(void), A9(void), A10(void), A11(void), A12(void), A13(void), A14(void), A14b(void), A15(void);

void               T_Disconnect_Ind(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);   /* TODO: include file. */
void               T_Disconnect_Con(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
#endif /* KSTACK_MEMORY_MAPPING */

/*
** Local constants.
*/
#if TL_STYLE == 3
/* Transport-Layer-Statemachine-Style #3 */
STATIC const KnxTLC_ActionListType Actions[] = {
    { /* 0,  */ {{A1, OPEN_IDLE}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }, {A0, CONNECTING     }}        },
    { /* 1,  */ {{A1, OPEN_IDLE}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT}, {A10, CONNECTING   }}        },
    { /* 2,  */ {{A0, CLOSED}, {A5, CLOSED }, {A5, CLOSED },        {A5,  CLOSED         }}        },
    { /* 3,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A0,  CONNECTING     }}        },
    { /* 4,  */ {{A0, CLOSED}, {A2, OPEN_IDLE}, {A2, OPEN_WAIT },   {A6,  CLOSED         }}        },
    { /* 5,  */ {{A0, CLOSED}, {A3, OPEN_IDLE}, {A3, OPEN_WAIT },   {A3,  CONNECTING     }}        },
    { /* 6,  */ {{A0, CLOSED}, {A4, OPEN_IDLE}, {A4, OPEN_WAIT },   {A6,  CONNECTING     }}        },
    { /* 7,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A10, CONNECTING     }}        },
    { /* 8,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A8, OPEN_IDLE },   {A6,  CLOSED         }}        },
    { /* 9,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A6, CLOSED },      {A6,  CLOSED         }}        },
    { /* 10, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A10, CONNECTING     }}        },
    { /* 11, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A6,  CLOSED         }}        },
    { /* 12, */ {{A0, CLOSED}, {A6, CLOSED }, {A9, OPEN_WAIT },     {A6,  CLOSED         }}        },
    { /* 13, */ {{A0, CLOSED}, {A6, CLOSED }, {A6, CLOSED },        {A6,  CLOSED         }}        },
    { /* 14, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A10, CONNECTING     }}        },
    { /* 15, */ {{A0, CLOSED}, {A7, OPEN_WAIT}, {A11, OPEN_WAIT },  {A11, CONNECTING     }}        },
    { /* 16, */ {{A0, CLOSED}, {A6, CLOSED }, {A6, CLOSED },        {A6,  CLOSED         }}        },
    { /* 17, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A9, OPEN_WAIT },   {A0,  CONNECTING     }}        },
    { /* 18, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A6, CLOSED },      {A0,  CONNECTING     }}        },
    { /* 19, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A13, OPEN_IDLE      }}        },
    { /* 20, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A5,  CLOSED         }}        },
    { /* 21, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A0,  CONNECTING     }}        },
    { /* 22, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A0,  CONNECTING     }}        },
    { /* 23, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A0,  CONNECTING     }}        },
    { /* 24, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A0,  CONNECTING     }}        },
    { /* 25, */ {{A12, CONNECTING}, {A6, CLOSED}, {A6, CLOSED },    {A6,  CLOSED         }}        },
    { /* 26, */ {{A15, CLOSED}, {A14, CLOSED }, {A14, CLOSED },     {A14, CLOSED         }}        },
    { /* 27, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT },   {A0,  CONNECTING     }}        },
};
#elif (TL_STYLE == 1) || (TL_STYLE == 2)
/* Transport-Layer-Statemachine, Styles #1 and #2 */
STATIC const ACTION_LIST Actions[] = {
#if     TL_STYLE == 1
    { /* 0,  */ {{A1, OPEN_IDLE}, {A6, CLOSED }, {A6, CLOSED }}     },
#else
/* #if TL_STYLE==2 */
    { /* 0,  */ {{A1, OPEN_IDLE}, {A0, OPEN_IDLE}, {A0, OPEN_IDLE }} },
#endif

#if     TL_STYLE == 1
    { /* 1,  */ {{A1, OPEN_IDLE}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT}} },
#else
    { /* 1,  */ {{A1, OPEN_IDLE}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }} },
#endif
    { /* 2,  */ {{A0, CLOSED}, {A5, CLOSED }, {A5, CLOSED }}        },
    { /* 3,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
#if     TL_STYLE == 1
    { /* 4,  */ {{A10, CLOSED}, {A2, OPEN_IDLE}, {A2, OPEN_WAIT }}  },
#else
    { /* 4,  */ {{A0, CLOSED}, {A2, OPEN_IDLE}, {A2, OPEN_WAIT }}   },
#endif
#if     TL_STYLE == 1
    { /* 5,  */ {{A10, CLOSED}, {A3, OPEN_IDLE}, {A3, OPEN_WAIT }}  },
#else
    { /* 5,  */ {{A0, CLOSED}, {A3, OPEN_IDLE}, {A3, OPEN_WAIT }}   },
#endif
#if     TL_STYLE == 1
    { /* 6,  */ {{A10, CLOSED}, {A4, OPEN_IDLE}, {A4, OPEN_WAIT }}  },
#else
    { /* 6,  */ {{A0, CLOSED}, {A4, OPEN_IDLE}, {A4, OPEN_WAIT }}   },
#endif
#if     TL_STYLE == 1
    { /* 7,  */ {{A10, CLOSED}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT }} },
#else
    { /* 7,  */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
#endif
#if     TL_STYLE == 1
    { /* 8,  */ {{A10, CLOSED}, {A6, CLOSED }, {A8, OPEN_IDLE }}    },
#else
    { /* 8,  */ {{A0, CLOSED}, {A6, CLOSED }, {A8b, OPEN_IDLE }}    },
#endif
#if     TL_STYLE == 1
    { /* 9,  */ {{A10, CLOSED}, {A6, CLOSED }, {A6, CLOSED }}       },
#else
    { /* 9,  */ {{A0, CLOSED}, {A6, CLOSED }, {A0, OPEN_WAIT }}     },
#endif
#if     TL_STYLE == 1
    { /* 10, */ {{A10, CLOSED}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT }} },
#else
    { /* 10, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
#endif
#if     TL_STYLE == 1
    { /* 11, */ {{A10, CLOSED}, {A6, CLOSED }, {A6, CLOSED }}       },
#else
    { /* 11, */ {{A0, CLOSED}, {A6, CLOSED }, {A0, OPEN_WAIT }}     },
#endif
#if     TL_STYLE == 1
    { /* 12, */ {{A10, CLOSED}, {A6, CLOSED }, {A9, OPEN_WAIT }}    },
#else
    { /* 12, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A9, OPEN_WAIT }}   },
#endif
#if     TL_STYLE == 1
    { /* 13, */ {{A10, CLOSED}, {A6, CLOSED }, {A6, CLOSED }}       },
#else
    { /* 13, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A6, CLOSED }}      },
#endif
#if     TL_STYLE == 1
    { /* 14, */ {{A10, CLOSED}, {A10, OPEN_IDLE}, {A10, OPEN_WAIT }} },
#else
    { /* 14, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
#endif
#if     TL_STYLE == 1
    { /* 15, */ {{A5, CLOSED}, {A7, OPEN_WAIT}, {A6, CLOSED }}      },
#else
    { /* 15, */ {{A0, CLOSED}, {A7, OPEN_WAIT}, {A11, OPEN_WAIT }}  },
#endif
    { /* 16, */ {{A0, CLOSED}, {A6, CLOSED }, {A6, CLOSED }}        },
    { /* 17, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A9, OPEN_WAIT }}   },
    { /* 18, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A6, CLOSED }}      },
#if     TL_STYLE == 1
    { /* 19, */ {{A0, CLOSED}, {A13, OPEN_IDLE}, {A13, OPEN_WAIT }} },
#else
    { /* 19, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
#endif
    { /* 20, */ {{A0, CLOSED}, {A5, CLOSED }, {A5, CLOSED }}        },
    { /* 21, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
    { /* 22, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
    { /* 23, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
    { /* 24, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
#if     TL_STYLE == 1
    { /* 25, */ {{A12, OPEN_IDLE}, {A6, CLOSED}, {A6, CLOSED }}     },
#else
    { /* 25, */ {{A12, OPEN_IDLE}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT}} },
#endif
#if     TL_STYLE == 1
    { /* 26, */ {{A15, CLOSED}, {A14, CLOSED }, {A14, CLOSED }}     },
#else
    { /* 26, */ {{A0, CLOSED}, {A14b, CLOSED }, {A11, OPEN_WAIT }}  },
#endif
    { /* 27, */ {{A0, CLOSED}, {A0, OPEN_IDLE}, {A0, OPEN_WAIT }}   },
};
#else
#error "Invalid TL_STYLE [1|2|3]"
#endif

STATIC const EVENT_FUNC TLC_Events[] = {
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

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#define StoreMessage()      Utl_MemCopy(_StoredMsg, KnxMSG_ScratchBufferPtr->msg, MSG_LEN)
#define RestoreMessage()    Utl_MemCopy(KnxMSG_ScratchBufferPtr->msg, _StoredMsg, MSG_LEN)


/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxTLC_StateType, KSTACK_CODE) KnxTLC_GetState(void)
#else
KnxTLC_StateType KnxTLC_GetState(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTLC_State;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_SetState(KnxTLC_StateType State)
#else
void KnxTLC_SetState(KnxTLC_StateType State)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_State = State;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Disconnect_Ind(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#else
void T_Disconnect_Ind(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Disconnect_Con(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#else
void T_Disconnect_Con(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#endif /* KSTACK_MEMORY_MAPPING */
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

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_StateMachine(KNX_TlcEventType event)
#else
void KnxTLC_StateMachine(KNX_TlcEventType event)
#endif /* KSTACK_MEMORY_MAPPING */
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


/*
**
** Local functions.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) StartConnectionTimeoutTimer(void)
#else
STATIC void StartConnectionTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTMR_Start(TM_TIMER_TLC_CON_TIMEOUT, TM_BASE_SEC, TLC_CONNECTION_TIMEOUT);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) RestartConnectionTimeoutTimer(void)
#else
STATIC void RestartConnectionTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTMR_Stop(TM_TIMER_TLC_CON_TIMEOUT);
    (void)KnxTMR_Start(TM_TIMER_TLC_CON_TIMEOUT, TM_BASE_SEC, TLC_CONNECTION_TIMEOUT);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) StopConnectionTimeoutTimer(void)
#else
STATIC void StopConnectionTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTMR_Stop(TM_TIMER_TLC_CON_TIMEOUT);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) StartAcknowledgementTimeoutTimer(void)
#else
STATIC void StartAcknowledgementTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTMR_Start(TM_TIMER_TLC_ACK_TIMEOUT, TM_BASE_SEC, TLC_ACKNOWLEDGE_TIMEOUT);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) StopAcknowledgementTimeoutTimer(void)
#else
STATIC void StopAcknowledgementTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTMR_Stop(TM_TIMER_TLC_ACK_TIMEOUT);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A0(void)
#else
STATIC void A0(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* do nothing. */
    if (KnxMSG_ScratchBufferPtr != (KnxMSG_BufferPtr)NULL) {
        (void)KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A1(void)
#else
STATIC void A1(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_SetConnectionAddress(KnxTLC_GetSourceAddress());

/* Send a T_CONNECT_ind to the user. */
    KnxMSG_ScratchBufferPtr->service = T_CONNECT_IND;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);

    KnxTLC_SetSequenceNumberSend((uint8)0);
    KnxTLC_SetSequenceNumberReceived((uint8)0);
    StartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A2(void)
#else
STATIC void A2(void)
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A3(void)
#else
STATIC void A3(void)
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A4(void)
#else
STATIC void A4(void)
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A5(void)
#else
STATIC void A5(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Send a T_Disconnect.ind to the user. */
    (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    T_Disconnect_Ind(KnxMSG_ScratchBufferPtr, KnxTLC_GetConnectionAddress(), KnxADR_GetPhysAddr());

    StopAcknowledgementTimeoutTimer();
    StopConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A6(void)
#else
STATIC void A6(void)
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A7(void)
#else
STATIC void A7(void)   /* Nur local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A8(void)
#else
STATIC void A8(void)  /* only local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
{
    StopAcknowledgementTimeoutTimer();
    KnxTLC_SetSequenceNumberSend(KnxTLC_GetSequenceNumberSend() + (uint8)1);

    RestoreMessage();

/* Send the stored buffer as a T_Data_Connected.con with cleared errorbits, */
/* connection number = 0 to the user. */

/* dest: connection_addr, sequence=SeqNoSend. */

    RestartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A8b(void)
#else
STATIC void A8b(void)        /* only local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
    StopAcknowledgementTimeoutTimer();
    KnxTLC_SetSequenceNumberSend(KnxTLC_GetSequenceNumberSend() + (uint8)1);
    RestartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A9(void)
#else
STATIC void A9(void)  /* only local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A10(void)
#else
STATIC void A10(void)  /* GO AWAY!!! */
#endif /* KSTACK_MEMORY_MAPPING */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU Priority = SYSTEM, */
/*      Destination = source (rbuffer), Sequence = 0 back to sender. */

    Knx_AddressType source;
    Knx_AddressType dest;

    source = KnxMSG_GetSourceAddress(KnxMSG_ScratchBufferPtr);
    dest   = KnxMSG_GetDestAddress(KnxMSG_ScratchBufferPtr);

    (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    T_Disconnect_Req(KnxMSG_ScratchBufferPtr, dest, source);

}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A11(void)
#else
STATIC void A11(void)  /* (Client only???) */
#endif /* KSTACK_MEMORY_MAPPING */
{
/* Store event back and handle after next event. Don�t change order of T_Data_Connected.req events. */

    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);   /* push-bask event. */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A12(void)
#else
STATIC void A12(void)                                              /* (Client only.) */
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTLC_SetConnectionAddress(KnxTLC_GetSourceAddress()); /* connection_address=address from T_CONNECT_requ */

/*  send N_Data_Individual.req with T_CONNECT_REQ_PDU */
    (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
    T_Connect_Req(KnxMSG_ScratchBufferPtr, KnxADR_GetPhysAddr(), KnxTLC_GetConnectionAddress());

    KnxTLC_SetSequenceNumberSend((uint8)0);
    KnxTLC_SetSequenceNumberReceived((uint8)0);
    StartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A13(void)
#else
STATIC void A13(void)  /* Nur local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
{
/*     Send a T_Connect.con to the user. */
    KnxMSG_ScratchBufferPtr->service = T_CONNECT_CON;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A14(void)
#else
STATIC void A14(void)    /* only local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM, */
/*      destination = connection_address, sequence = 0 to the network layer (remote device). */

/*      Send a T_Disconnect.con to the user. */

    StopAcknowledgementTimeoutTimer();
    StopConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A14b(void)
#else
STATIC void A14b(void)  /* wie A14, nur ohne T_DISCONNECT_CON to user. */
#endif /* KSTACK_MEMORY_MAPPING */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM, */
/*      destination = connection_address, sequence = 0 to the network layer (remote device). */

    StopAcknowledgementTimeoutTimer();
    StopConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A15(void)
#else
STATIC void A15(void)   /* only local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
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
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventConnectInd(void)
#else
STATIC uint8 EventConnectInd(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (KnxTLC_GetSourceAddress() == KnxTLC_GetConnectionAddress()) {
        return (uint8)0;
    } else {
        return (uint8)1;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventDisconnectInd(void)
#else
STATIC uint8 EventDisconnectInd(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (KnxTLC_GetSourceAddress() == KnxTLC_GetConnectionAddress()) {
        return (uint8)2;
    } else {
        return (uint8)3;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventDataConnectedInd(void)
#else
STATIC uint8 EventDataConnectedInd(void)
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventAckInd(void)
#else
STATIC uint8 EventAckInd(void)
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventNakInd(void)
#else
STATIC uint8 EventNakInd(void)
#endif /* KSTACK_MEMORY_MAPPING */
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


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventConnectReq(void)
#else
STATIC uint8 EventConnectReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Client-only. */
    /* Event 25.    */
    return (uint8)25;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventDisconnectReq(void)
#else
STATIC uint8 EventDisconnectReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Client-only. */
    /* Event 26.    */
    return (uint8)26;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventDataConnectedReq(void)
#else
STATIC uint8 EventDataConnectedReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Client-only. */
    /* Event 15.    */
    return (uint8)15;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventConnectCon(void)
#else
STATIC uint8 EventConnectCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Client only.         */
    /* Events 19 und 20.    */
    if (KnxMSG_GetIAK(KnxMSG_ScratchBufferPtr) == IAK_OK) {
        return (uint8)19;
    } else {
        return (uint8)20;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventDisconnectCon(void)
#else
STATIC uint8 EventDisconnectCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Event 21.    */
    return (uint8)21;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventDataConnectedCon(void)
#else
STATIC uint8 EventDataConnectedCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Event 22.    */
    return (uint8)22;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventAckCon(void)
#else
STATIC uint8 EventAckCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Event 23.    */
    return (uint8)23;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventNakCon(void)
#else
STATIC uint8 EventNakCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Event 24.    */
    return (uint8)24;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventTimeoutCon(void)
#else
STATIC uint8 EventTimeoutCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return (uint8)16;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventTimeoutAck(void)
#else
STATIC uint8 EventTimeoutAck(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (KnxTLC_GetRepetitionCount() < MAX_REP_COUNT) {
        return (uint8)17;
    } else {
        return (uint8)18;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8, KSTACK_CODE) EventUndefined(void)
#else
STATIC uint8 EventUndefined(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return (uint8)27;
}



#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
