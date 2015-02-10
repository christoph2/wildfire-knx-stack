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
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/

/*
**
**      Konnex Transport-Layer-Statemachine.
**
**/

#include "knx_layer_transport.h"

/*
** Local types.
*/
typedef uint8_t (*KnxTlc_EventFunctionType)(void);

/*
** Local function prototypes.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, KSTACK_CODE) KnxTlc_StartConnectionTimeoutTimer(void);
STATIC  FUNC(void, KSTACK_CODE) KnxTlc_RestartConnectionTimeoutTimer(void);
STATIC  FUNC(void, KSTACK_CODE) KnxTlc_StopConnectionTimeoutTimer(void);
STATIC  FUNC(void, KSTACK_CODE) KnxTlc_StartAcknowledgementTimeoutTimer(void);
STATIC  FUNC(void, KSTACK_CODE) KnxTlc_StopAcknowledgementTimeoutTimer(void);

STATIC  FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Connect_Ind(void), KnxTlc_Event_Disconnect_Ind(void), KnxTlc_Event_DataConnected_Ind(void);
STATIC  FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Ack_Ind(void), KnxTlc_Event_Nak_Ind(void), KnxTlc_Event_Connect_Req(void);
STATIC  FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Disconnect_Req(void), KnxTlc_Event_DataConnected_Req(void), KnxTlc_Event_Connect_Con(void);
STATIC  FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Disconnect_Con(void), KnxTlc_Event_DataConnected_Con(void), KnxTlc_Event_Ack_Con(void);
STATIC  FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Nak_Con(void), KnxTlc_Event_Timeout_Con(void), KnxTlc_Event_Timeout_Ack(void);
STATIC  FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Undefined(void);

STATIC  FUNC(void, KSTACK_CODE) A0(void), A1(void), A2(void), A3(void), A4(void), A5(void), A6(void), A7(void), A8(void);
STATIC  FUNC(void, KSTACK_CODE) A8b(void), A9(void), A10(void), A11(void), A12(void), A13(void), A14(void), A14b(void), A15(void);


FUNC(void, KSTACK_CODE) T_Disconnect_Ind(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
FUNC(void, KSTACK_CODE) T_Disconnect_Con(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
#else
STATIC void KnxTlc_StartConnectionTimeoutTimer(void);
STATIC void KnxTlc_RestartConnectionTimeoutTimer(void);
STATIC void KnxTlc_StopConnectionTimeoutTimer(void);
STATIC void KnxTlc_StartAcknowledgementTimeoutTimer(void);
STATIC void KnxTlc_StopAcknowledgementTimeoutTimer(void);

STATIC uint8_t KnxTlc_Event_Connect_Ind(void), KnxTlc_Event_Disconnect_Ind(void), KnxTlc_Event_DataConnected_Ind(void);
STATIC uint8_t KnxTlc_Event_Ack_Ind(void), KnxTlc_Event_Nak_Ind(void), KnxTlc_Event_Connect_Req(void);
STATIC uint8_t KnxTlc_Event_Disconnect_Req(void), KnxTlc_Event_DataConnected_Req(void), KnxTlc_Event_Connect_Con(void);
STATIC uint8_t KnxTlc_Event_Disconnect_Con(void), KnxTlc_Event_DataConnected_Con(void), KnxTlc_Event_Ack_Con(void);
STATIC uint8_t KnxTlc_Event_Nak_Con(void), KnxTlc_Event_Timeout_Con(void), KnxTlc_Event_Timeout_Ack(void);
STATIC uint8_t KnxTlc_Event_Undefined(void);

STATIC void A0(void), A1(void), A2(void), A3(void), A4(void), A5(void), A6(void), A7(void), A8(void);
STATIC void A8b(void), A9(void), A10(void), A11(void), A12(void), A13(void), A14(void), A14b(void), A15(void);

void T_Disconnect_Ind(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
void T_Disconnect_Con(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
#endif /* KSTACK_MEMORY_MAPPING */

/*
** Local constants.
*/
#if KNX_TL_STATEMACHINE_STYLE == 3
/* Transport-Layer-Statemachine-Style #3 */
STATIC const KnxTlc_ActionListType KnxTlc_Actions[] = {
    {/* 0  */{{ A1,  KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A0,   KNX_TLC_STATE_CONNECTING }}},
    {/* 1  */{{ A1,  KNX_TLC_STATE_OPEN_IDLE },  { A10,  KNX_TLC_STATE_OPEN_IDLE },  { A10,  KNX_TLC_STATE_OPEN_WAIT },  { A10,  KNX_TLC_STATE_CONNECTING }}},
    {/* 2  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A5,   KNX_TLC_STATE_CLOSED },     { A5,   KNX_TLC_STATE_CLOSED },     { A5,   KNX_TLC_STATE_CLOSED }}},
    {/* 3  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A0,   KNX_TLC_STATE_CONNECTING }}},
    {/* 4  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A2,   KNX_TLC_STATE_OPEN_IDLE },  { A2,   KNX_TLC_STATE_OPEN_WAIT },  { A6,   KNX_TLC_STATE_CLOSED }}},
    {/* 5  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A3,   KNX_TLC_STATE_OPEN_IDLE },  { A3,   KNX_TLC_STATE_OPEN_WAIT },  { A3,   KNX_TLC_STATE_CONNECTING }}},
    {/* 6  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A4,   KNX_TLC_STATE_OPEN_IDLE },  { A4,   KNX_TLC_STATE_OPEN_WAIT },  { A6,   KNX_TLC_STATE_CONNECTING }}},
    {/* 7  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A10,  KNX_TLC_STATE_CONNECTING }}},
    {/* 8  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A8,   KNX_TLC_STATE_OPEN_IDLE },  { A6,   KNX_TLC_STATE_CLOSED }}},
    {/* 9  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED }}},
    {/* 10 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A10,  KNX_TLC_STATE_CONNECTING }}},
    {/* 11 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A6,   KNX_TLC_STATE_CLOSED     }}},
    {/* 12 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A9,   KNX_TLC_STATE_OPEN_WAIT },  { A6,   KNX_TLC_STATE_CLOSED     }}},
    {/* 13 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED     }}},
    {/* 14 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A10,  KNX_TLC_STATE_CONNECTING }}},
    {/* 15 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A7,   KNX_TLC_STATE_OPEN_WAIT },  { A11,  KNX_TLC_STATE_OPEN_WAIT },  { A11,  KNX_TLC_STATE_CONNECTING }}},
    {/* 16 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED     }}},
    {/* 17 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A9,   KNX_TLC_STATE_OPEN_WAIT },  { A0,   KNX_TLC_STATE_CONNECTING }}},
    {/* 18 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A6,   KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_CONNECTING }}},
    {/* 19 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A13,  KNX_TLC_STATE_OPEN_IDLE  }}},
    {/* 20 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A5,   KNX_TLC_STATE_CLOSED     }}},
    {/* 21 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A0,   KNX_TLC_STATE_CONNECTING }}},
    {/* 22 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A0,   KNX_TLC_STATE_CONNECTING }}},
    {/* 23 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A0,   KNX_TLC_STATE_CONNECTING }}},
    {/* 24 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A0,   KNX_TLC_STATE_CONNECTING }}},
    {/* 25 */{{ A12, KNX_TLC_STATE_CONNECTING }, { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED     }}},
    {/* 26 */{{ A15, KNX_TLC_STATE_CLOSED },     { A14,  KNX_TLC_STATE_CLOSED },     { A14,  KNX_TLC_STATE_CLOSED },     { A14,  KNX_TLC_STATE_CLOSED     }}},
    {/* 27 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT },  { A0,   KNX_TLC_STATE_CONNECTING }}},
};
#elif (KNX_TL_STATEMACHINE_STYLE == 1) || (KNX_TL_STATEMACHINE_STYLE == 2)
/* Transport-Layer-Statemachine, Styles #1 and #2 */
STATIC const ACTION_LIST KnxTlc_Actions[] = {
#if     KNX_TL_STATEMACHINE_STYLE == 1
    {/* 0  */{{ A1,  KNX_TLC_STATE_OPEN_IDLE},   { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED }}},
#else
/* #if KNX_TL_STATEMACHINE_STYLE==2 */
    {/* 0  */{{ A1,  KNX_TLC_STATE_OPEN_IDLE},   { A0,   KNX_TLC_STATE_OPEN_IDLE},   { A0,   KNX_TLC_STATE_OPEN_IDLE }}},
#endif

#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 1  */{{ A1,  KNX_TLC_STATE_OPEN_IDLE },  { A10,  KNX_TLC_STATE_OPEN_IDLE },  { A10,  KNX_TLC_STATE_OPEN_WAIT }}},
#else
    {/* 1  */{{ A1,  KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
    {/* 2  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A5,   KNX_TLC_STATE_CLOSED },     { A5,   KNX_TLC_STATE_CLOSED }}},
    {/* 3  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 4  */{{ A10, KNX_TLC_STATE_CLOSED },     { A2,   KNX_TLC_STATE_OPEN_IDLE },  { A2,   KNX_TLC_STATE_OPEN_WAIT }}},
#else
    {/* 4  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A2,   KNX_TLC_STATE_OPEN_IDLE },  { A2,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 5  */{{ A10, KNX_TLC_STATE_CLOSED },     { A3,   KNX_TLC_STATE_OPEN_IDLE },  { A3,   KNX_TLC_STATE_OPEN_WAIT }}},
#else
    {/* 5  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A3,   KNX_TLC_STATE_OPEN_IDLE },  { A3,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 6  */{{ A10, KNX_TLC_STATE_CLOSED },     { A4,   KNX_TLC_STATE_OPEN_IDLE },  { A4,   KNX_TLC_STATE_OPEN_WAIT }}},
#else
    {/* 6  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A4,   KNX_TLC_STATE_OPEN_IDLE },  { A4,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 7  */{{ A10, KNX_TLC_STATE_CLOSED },     { A10,  KNX_TLC_STATE_OPEN_IDLE },  { A10,  KNX_TLC_STATE_OPEN_WAIT }}},
#else
    {/* 7  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 8  */{{ A10, KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A8,   KNX_TLC_STATE_OPEN_IDLE }}},
#else
    {/* 8  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A8b,  KNX_TLC_STATE_OPEN_IDLE }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 9  */{{ A10, KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED }}},
#else
    {/* 9  */{{ A0,  KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 10 */{{ A10, KNX_TLC_STATE_CLOSED },     { A10,  KNX_TLC_STATE_OPEN_IDLE },  { A10,  KNX_TLC_STATE_OPEN_WAIT }}},
#else
    {/* 10 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 11 */{{ A10, KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED }}},
#else
    {/* 11 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 12 */{{ A10, KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A9,   KNX_TLC_STATE_OPEN_WAIT }}},
#else
    {/* 12 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A9,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 13 */{{ A10, KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED }}},
#else
    {/* 13 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A6,   KNX_TLC_STATE_CLOSED }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 14 */{{ A10, KNX_TLC_STATE_CLOSED },     { A10,  KNX_TLC_STATE_OPEN_IDLE },  { A10,  KNX_TLC_STATE_OPEN_WAIT }}},
#else
    {/* 14 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 15 */{{ A5,  KNX_TLC_STATE_CLOSED },     { A7,   KNX_TLC_STATE_OPEN_WAIT },  { A6,   KNX_TLC_STATE_CLOSED }}},
#else
    {/* 15 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A7,   KNX_TLC_STATE_OPEN_WAIT },  { A11,  KNX_TLC_STATE_OPEN_WAIT }}},
#endif
    {/* 16 */{{ A0,  KNX_TLC_STATE_CLOSED},      { A6,   KNX_TLC_STATE_CLOSED },     { A6,   KNX_TLC_STATE_CLOSED }}},
    {/* 17 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A9,   KNX_TLC_STATE_OPEN_WAIT }}},
    {/* 18 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A6,   KNX_TLC_STATE_CLOSED }}},
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 19 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A13,  KNX_TLC_STATE_OPEN_IDLE },  { A13,  KNX_TLC_STATE_OPEN_WAIT }}},
#else
    {/* 19 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
    {/* 20 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A5,   KNX_TLC_STATE_CLOSED },     { A5,   KNX_TLC_STATE_CLOSED }}},
    {/* 21 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
    {/* 22 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
    {/* 23 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
    {/* 24 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0,   KNX_TLC_STATE_OPEN_IDLE },  { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 25 */{{ A12, KNX_TLC_STATE_OPEN_IDLE },  { A6, KNX_TLC_STATE_CLOSED },       { A6,   KNX_TLC_STATE_CLOSED }}},
#else
    {/* 25 */{{ A12, KNX_TLC_STATE_OPEN_IDLE },  { A0, KNX_TLC_STATE_OPEN_IDLE },    { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
#endif
#if KNX_TL_STATEMACHINE_STYLE == 1
    {/* 26 */{{ A15, KNX_TLC_STATE_CLOSED },     { A14, KNX_TLC_STATE_CLOSED },      { A14,  KNX_TLC_STATE_CLOSED }}},
#else
    {/* 26 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A14b, KNX_TLC_STATE_CLOSED },     { A11,  KNX_TLC_STATE_OPEN_WAIT }}},
#endif
    {/* 27 */{{ A0,  KNX_TLC_STATE_CLOSED },     { A0, KNX_TLC_STATE_OPEN_IDLE },    { A0,   KNX_TLC_STATE_OPEN_WAIT }}},
};
#else
#error "Invalid TL_STYLE [1|2|3]"
#endif

STATIC const KnxTlc_EventFunctionType TLC_Events[] = {
/*      Event                                 Handler                     */
/*      ==================================================================*/
/*      KNX_TLC_EVENT_CONNECT_IND          */ KnxTlc_Event_Connect_Ind,
/*      KNX_TLC_EVENT_DISCONNECT_IND       */ KnxTlc_Event_Disconnect_Ind,
/*      KNX_TLC_EVENT_DATA_CONNECTED_IND   */ KnxTlc_Event_DataConnected_Ind,
/*      KNX_TLC_EVENT_ACK_IND              */ KnxTlc_Event_Ack_Ind,
/*      KNX_TLC_EVENT_NAK_IND              */ KnxTlc_Event_Nak_Ind,
/*      KNX_TLC_EVENT_CONNECT_REQ          */ KnxTlc_Event_Connect_Req,
/*      KNX_TLC_EVENT_DISCONNECT_REQ       */ KnxTlc_Event_Disconnect_Req,
/*      KNX_TLC_EVENT_DATA_CONNECTED_REQ   */ KnxTlc_Event_DataConnected_Req,
/*      KNX_TLC_EVENT_CONNECT_CON          */ KnxTlc_Event_Connect_Con,
/*      KNX_TLC_EVENT_DISCONNECT_CON       */ KnxTlc_Event_Disconnect_Con,
/*      KNX_TLC_EVENT_DATA_CONNECTED_CON   */ KnxTlc_Event_DataConnected_Con,
/*      KNX_TLC_EVENT_ACK_CON              */ KnxTlc_Event_Ack_Con,
/*      KNX_TLC_EVENT_NAK_CON              */ KnxTlc_Event_Nak_Con,
/*      KNX_TLC_EVENT_TIMEOUT_CON          */ KnxTlc_Event_Timeout_Con,
/*      KNX_TLC_EVENT_TIMEOUT_ACK          */ KnxTlc_Event_Timeout_Ack,
/*      ==================================================================*/
};


/*
** Local variables.
*/
STATIC KnxTlc_StateType KnxTlc_State;
STATIC Knx_MessageType  _StoredMsg;    /* Client-only. */


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

#define StoreMessage()      Utl_MemCopy(_StoredMsg, KnxMsg_ScratchBufferPtr->msg, MSG_LEN), printf("StoreMessage()\n")
#define RestoreMessage()    Utl_MemCopy(KnxMsg_ScratchBufferPtr->msg, _StoredMsg, MSG_LEN), printf("RestoreMessage()\n")

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxTlc_StateType, KSTACK_CODE) KnxTlc_GetState(void)
#else
KnxTlc_StateType KnxTlc_GetState(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KnxTlc_State;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_SetState(KnxTlc_StateType State)
#else
void KnxTlc_SetState(KnxTlc_StateType State)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTlc_State = State;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Disconnect_Ind(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#else
void T_Disconnect_Ind(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(pBuffer, KNX_TPCI_DISCONNECT_REQ_PDU);
    KnxMsg_SetSourceAddress(pBuffer, source);
    KnxMsg_SetDestAddress(pBuffer, dest);

    KnxMsg_GetMessagePtr(pBuffer)->ctrl    = (uint8_t)0xB0; /* short-cuts... */
    KnxMsg_GetMessagePtr(pBuffer)->npci    = (uint8_t)0x60;

    KnxMsg_SetLen(pBuffer, (uint8_t)7);
    pBuffer->service = KNX_SERVICE_T_DISCONNECT_IND;

    (void)KnxMsg_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) T_Disconnect_Con(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#else
void T_Disconnect_Con(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(pBuffer, KNX_TPCI_DISCONNECT_REQ_PDU);
    KnxMsg_SetSourceAddress(pBuffer, source);
    KnxMsg_SetDestAddress(pBuffer, dest);

    KnxMsg_GetMessagePtr(pBuffer)->ctrl    = (uint8_t)0xb0; /* short-cuts... */
    KnxMsg_GetMessagePtr(pBuffer)->npci    = (uint8_t)0x60;

    KnxMsg_SetLen(pBuffer, (uint8_t)7);
    pBuffer->service = KNX_SERVICE_T_DISCONNECT_CON;

    (void)KnxMsg_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_StateMachine(KNX_TlcEventType event)
#else
void KnxTlc_StateMachine(KNX_TlcEventType event)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxTlc_ActionType action;

    printf("STATE: %u EVENT: %u ", KnxTlc_GetState(), event);

    action = KnxTlc_Actions[((event < KNX_TLC_EVENT_UNDEFINED) ? TLC_Events[event]() : KnxTlc_Event_Undefined())].Action[KnxTlc_GetState()];
    KnxTlc_SetState(action.Next);
    printf("NEXT-STATE: %u\n", action.Next);
    action.Function();
}


/*
**
** Local functions.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) KnxTlc_StartConnectionTimeoutTimer(void)
#else
STATIC void KnxTlc_StartConnectionTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTmr_Start(TMR_TIMER_TLC_CON_TIMEOUT, TMR_RESOLUTION_SEC, TLC_CONNECTION_TIMEOUT);
    printf("StartConnectionTimeoutTimer\n");
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) KnxTlc_RestartConnectionTimeoutTimer(void)
#else
STATIC void KnxTlc_RestartConnectionTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTmr_Stop(TMR_TIMER_TLC_CON_TIMEOUT);
    (void)KnxTmr_Start(TMR_TIMER_TLC_CON_TIMEOUT, TMR_RESOLUTION_SEC, TLC_CONNECTION_TIMEOUT);
    printf("RestartConnectionTimeoutTimer\n");
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) KnxTlc_StopConnectionTimeoutTimer(void)
#else
STATIC void KnxTlc_StopConnectionTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTmr_Stop(TMR_TIMER_TLC_CON_TIMEOUT);
    printf("StopConnectionTimeoutTimer\n");
}

#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) KnxTlc_OnConnectionTimeoutTimer(void)
#else
/*STATIC*/ void KnxTlc_OnConnectionTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    //(void)KnxTmr_Stop(TMR_TIMER_TLC_CON_TIMEOUT);
    printf("OnConnectionTimeoutTimer\n");
    KnxTlc_StateMachine(KNX_TLC_EVENT_TIMEOUT_CON);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) KnxTlc_StartAcknowledgementTimeoutTimer(void)
#else
STATIC void KnxTlc_StartAcknowledgementTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTmr_Start(TMR_TIMER_TLC_ACK_TIMEOUT, TMR_RESOLUTION_SEC, TLC_ACKNOWLEDGE_TIMEOUT);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) KnxTlc_StopAcknowledgementTimeoutTimer(void)
#else
STATIC void KnxTlc_StopAcknowledgementTimeoutTimer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    (void)KnxTmr_Stop(TMR_TIMER_TLC_ACK_TIMEOUT);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A0(void)
#else
STATIC void A0(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* do nothing. */
    DBG_PRINTLN("A0()");
    if (KnxMsg_ScratchBufferPtr != (KnxMsg_BufferPtr)NULL) {
        KnxMsg_ReleaseBuffer(KnxMsg_ScratchBufferPtr);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A1(void)
#else
STATIC void A1(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    DBG_PRINTLN("A1()");

    //KnxTlc_SetConnectionAddress(KnxTlc_GetSourceAddress());
    KnxTlc_SetConnectionAddress(KnxMsg_GetSourceAddress(KnxMsg_ScratchBufferPtr));

    /* Send a T_CONNECT_ind to the user. */
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_T_CONNECT_IND;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);

    KnxTlc_SetSequenceNumberSend((uint8_t)0);
    KnxTlc_SetSequenceNumberReceived((uint8_t)0);
    KnxTlc_StartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A2(void)
#else
STATIC void A2(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_BufferPtr pBuffer;

    DBG_PRINTLN("A2()");

/*      Send a N_Data_Individual.req with T_ACK_PDU, priority = SYSTEM, destination = */
/*      connection_address, sequence =SeqNoRcv to the network layer (remote device).  */
    KnxMsg_AllocateBuffer(&pBuffer);

    if (pBuffer != (KnxMsg_BufferPtr)NULL) {
        T_Ack_Req(pBuffer, KnxTlc_GetSourceAddress(), /* KnxADR_GetPhysAddr(), */ KnxTlc_GetConnectionAddress(), KnxTlc_GetSequenceNumberReceived());
        KnxTlc_SetSequenceNumberReceived(KnxTlc_GetSequenceNumberReceived() + 1);
    } else {
        /* Errorhandling. */
    }

/*      Send the received buffer as a T_Data_Connected.ind to the user. */
    //KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_T_DATA_CONNECTED_IND;
    //(void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);

    KnxTlc_RestartConnectionTimeoutTimer();
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

    DBG_PRINTLN("A3()");

    (void)KnxMsg_ClearBuffer(KnxMsg_ScratchBufferPtr);
    T_Ack_Req(KnxMsg_ScratchBufferPtr, KnxTlc_GetSourceAddress(), /* KnxADR_GetPhysAddr(), */ KnxTlc_GetConnectionAddress(), KnxTlc_GetSequenceNumberReceived());
    KnxTlc_RestartConnectionTimeoutTimer();
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

    DBG_PRINTLN("A4()");

    (void)KnxMsg_ClearBuffer(KnxMsg_ScratchBufferPtr);
    T_Nak_Req(KnxMsg_ScratchBufferPtr, KnxTlc_GetSourceAddress(), /* KnxADR_GetPhysAddr(), */ KnxTlc_GetConnectionAddress(), KnxTlc_GetSequenceNumberReceived());

    KnxTlc_RestartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A5(void)
#else
STATIC void A5(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Send a T_Disconnect.ind to the user. */
    /* Handled by callback. */
    //(void)KnxMsg_ClearBuffer(KnxMsg_ScratchBufferPtr);
    //T_Disconnect_Ind(KnxMsg_ScratchBufferPtr, KnxTlc_GetConnectionAddress(), KnxTlc_GetSourceAddress(), /*KnxADR_GetPhysAddr() */);


    DBG_PRINTLN("A5()");

    KnxTlc_StopAcknowledgementTimeoutTimer();
    KnxTlc_StopConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A6(void)
#else
STATIC void A6(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_BufferPtr pBuffer;

/*
**      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM,
**      destination = connection_address, sequence = 0 to the network layer (remote device).
*/

    DBG_PRINTLN("A6()");

    KnxMsg_AllocateBuffer(&pBuffer);

    if (pBuffer != (KnxMsg_BufferPtr)NULL) {
        T_Disconnect_Req(pBuffer, KnxTlc_GetSourceAddress(), /* KnxADR_GetPhysAddr(), */ KnxTlc_GetConnectionAddress());
    } else {
        /* Errorhandling. */
    }

    /* Send a T_Disconnect.ind to the user. */
    /* Handled by callback. */
//#if 0
    if (KnxMsg_ScratchBufferPtr == (KnxMsg_BufferPtr)NULL) {
        KnxMsg_AllocateBuffer(&KnxMsg_ScratchBufferPtr);
    } else {
        (void)KnxMsg_ClearBuffer(KnxMsg_ScratchBufferPtr);
    }

    T_Disconnect_Ind(KnxMsg_ScratchBufferPtr, KnxTlc_GetSourceAddress(), /* KnxADR_GetPhysAddr(), */ KnxTlc_GetConnectionAddress());
//#endif

    KnxTlc_StopAcknowledgementTimeoutTimer();
    KnxTlc_StopConnectionTimeoutTimer();
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

    DBG_PRINTLN("A7()");
    StoreMessage();

/*    MSG_ScratchBuffer->service=T_DATA_CONNECTED_REQ; */
    //KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ;
    KnxMsg_SetSeqNo(KnxMsg_ScratchBufferPtr, KnxTlc_GetSequenceNumberSend());
    //(void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);

    KnxTlc_SetRepetitionCount((uint8_t)0);

    KnxTlc_StartAcknowledgementTimeoutTimer();
    KnxTlc_RestartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A8(void)
#else
STATIC void A8(void)  /* only local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
{
    DBG_PRINTLN("A8()");

    KnxTlc_StopAcknowledgementTimeoutTimer();
    KnxTlc_SetSequenceNumberSend(KnxTlc_GetSequenceNumberSend() + (uint8_t)1);

    RestoreMessage();

/* Send the stored buffer as a T_Data_Connected.con with cleared errorbits, */
/* connection number = 0 to the user. */

/* dest: connection_addr, sequence=SeqNoSend. */

    KnxTlc_RestartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A8b(void)
#else
STATIC void A8b(void)        /* only local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
{
    DBG_PRINTLN("A8b()");

    KnxMsg_ReleaseBuffer(KnxMsg_ScratchBufferPtr);
    KnxTlc_StopAcknowledgementTimeoutTimer();
    KnxTlc_SetSequenceNumberSend(KnxTlc_GetSequenceNumberSend() + (uint8_t)1);
    KnxTlc_RestartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A9(void)
#else
STATIC void A9(void)  /* only local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
{

    DBG_PRINTLN("A9()");

    if (KnxMsg_ScratchBufferPtr == (KnxMsg_BufferPtr)NULL) {
        KnxMsg_AllocateBuffer(&KnxMsg_ScratchBufferPtr);
    } else {
        (void)KnxMsg_ClearBuffer(KnxMsg_ScratchBufferPtr);
    }

/*  Send the stored message as a N_Data_Individual.req to the network layer (remote device). */
    RestoreMessage();
    KnxMsg_SetSeqNo(KnxMsg_ScratchBufferPtr, KnxTlc_GetSequenceNumberSend());
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_N_DATA_INDIVIDUAL_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);

    KnxTlc_SetRepetitionCount(KnxTlc_GetRepetitionCount() + (uint8_t)1);

    KnxTlc_StartAcknowledgementTimeoutTimer();
    KnxTlc_RestartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A10(void)
#else
STATIC void A10(void)  /* GO AWAY!!! */
#endif /* KSTACK_MEMORY_MAPPING */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU Priority = SYSTEM, */
/*      Destination = source (rbuffer), Sequence = 0 back to sender. */

    DBG_PRINTLN("A10()");

    Knx_AddressType source;
    Knx_AddressType dest;

    source = KnxMsg_GetSourceAddress(KnxMsg_ScratchBufferPtr);
    dest   = KnxMsg_GetDestAddress(KnxMsg_ScratchBufferPtr);

    (void)KnxMsg_ClearBuffer(KnxMsg_ScratchBufferPtr);
    T_Disconnect_Req(KnxMsg_ScratchBufferPtr, dest, source);

}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A11(void)
#else
STATIC void A11(void)  /* (Client only???) */
#endif /* KSTACK_MEMORY_MAPPING */
{
/* Store event back and handle after next event. Don't change order of T_Data_Connected.req events. */

    DBG_PRINTLN("A11()");

//    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);   /* push-bask event. */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A12(void)
#else
STATIC void A12(void)                                              /* (Client only.) */
#endif /* KSTACK_MEMORY_MAPPING */
{
#if 0

/*  send N_Data_Individual.req with T_CONNECT_REQ_PDU */
    (void)KnxMsg_ClearBuffer(KnxMsg_ScratchBufferPtr);
    T_Connect_Req(KnxMsg_ScratchBufferPtr, KnxTlc_GetSourceAddress(), /* KnxADR_GetPhysAddr(), */ KnxTlc_GetConnectionAddress());
#endif

    DBG_PRINTLN("A12()");

//    KnxTlc_SetConnectionAddress(KnxMsg_GetDestAddress(KnxMsg_ScratchBufferPtr)); /* connection_address=address from T_CONNECT_requ */

    KnxTlc_SetSequenceNumberSend((uint8_t)0);
    KnxTlc_SetSequenceNumberReceived((uint8_t)0);
    KnxTlc_StartConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A13(void)
#else
STATIC void A13(void)  /* Nur local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
{
/*     Send a T_Connect.con to the user. */

    DBG_PRINTLN("A13()");

    //KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_T_CONNECT_CON;
    //(void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
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

    printf(" A14()\n");

    KnxTlc_StopAcknowledgementTimeoutTimer();
    KnxTlc_StopConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A14b(void)
#else
STATIC void A14b(void)  /* wie A14, nur ohne T_DISCONNECT_CON to user. */
#endif /* KSTACK_MEMORY_MAPPING */
{
/*      Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM, */
/*      destination = connection_address, sequence = 0 to the network layer (remote device). */

    DBG_PRINTLN(" A14b()");

    KnxTlc_StopAcknowledgementTimeoutTimer();
    KnxTlc_StopConnectionTimeoutTimer();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) A15(void)
#else
STATIC void A15(void)   /* only local-user (Client only). */
#endif /* KSTACK_MEMORY_MAPPING */
{
/*      Send a T_Disconnect.con to the management user */

    DBG_PRINTLN("A15()");

    KnxTlc_StopAcknowledgementTimeoutTimer();
    KnxTlc_StopConnectionTimeoutTimer();
}


/*
**
** Event-Handler.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Connect_Ind(void)
#else
STATIC uint8_t KnxTlc_Event_Connect_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (KnxTlc_GetSourceAddress() == KnxTlc_GetConnectionAddress()) {
        return (uint8_t)0;
    } else {
        return (uint8_t)1;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Disconnect_Ind(void)
#else
STATIC uint8_t KnxTlc_Event_Disconnect_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (KnxTlc_GetSourceAddress() == KnxTlc_GetConnectionAddress()) {
        return (uint8_t)2;
    } else {
        return (uint8_t)3;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_DataConnected_Ind(void)
#else
STATIC uint8_t KnxTlc_Event_DataConnected_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t event_num;

    if (KnxTlc_GetSourceAddress() == KnxTlc_GetConnectionAddress()) {
        if (KnxTlc_GetSequenceNumberOfPDU() == KnxTlc_GetSequenceNumberReceived()) {
            event_num = (uint8_t)4;
        } else if (KnxTlc_GetSequenceNumberOfPDU() == ((KnxTlc_GetSequenceNumberReceived() - (uint8_t)1) & (uint8_t)0x0f)) {
            event_num = (uint8_t)5;
        } else if ((KnxTlc_GetSequenceNumberOfPDU() != KnxTlc_GetSequenceNumberReceived() - (uint8_t)1)
                   && (KnxTlc_GetSequenceNumberOfPDU() != ((KnxTlc_GetSequenceNumberReceived() - (uint8_t)1) & (uint8_t)0x0f)))
        {
            event_num = (uint8_t)6;
        } else {
            event_num = (uint8_t)27;
        }
    } else {
        event_num = (uint8_t)7;
    }

    return event_num;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Ack_Ind(void)
#else
STATIC uint8_t KnxTlc_Event_Ack_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t event_num;

    if (KnxTlc_GetSourceAddress() == KnxTlc_GetConnectionAddress()) {
        printf("OK, sourceAddr == connectionAddr.\n");
        if (KnxTlc_GetSequenceNumberOfPDU() == KnxTlc_GetSequenceNumberSend()) {
            printf("OK, seqNoPDU == seqNoSend.\n");
            event_num = (uint8_t)8;
        } else {
            printf("NOT-OK, seqNoPDU != seqNoSend!\n");
            event_num = (uint8_t)9;
        }
    } else {
        printf("NOT-OK, sourceAddr != connectionAddr! ");
        printf("<<%04X>><<%04X>>\n", KnxTlc_GetSourceAddress(), KnxTlc_GetConnectionAddress());
        event_num = (uint8_t)10;
    }

    return event_num;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Nak_Ind(void)
#else
STATIC uint8_t KnxTlc_Event_Nak_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t event_num;

    if (KnxTlc_GetSourceAddress() == KnxTlc_GetConnectionAddress()) {
        if (KnxTlc_GetSequenceNumberOfPDU() != KnxTlc_GetSequenceNumberSend()) {
            event_num = (uint8_t)11;
        } else {
            if (KnxTlc_GetRepetitionCount() < MAX_REP_COUNT) {
                event_num = (uint8_t)12;
            } else {
                event_num = (uint8_t)13;
            }
        }
    } else {
        event_num = (uint8_t)14;
    }

    return event_num;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Connect_Req(void)
#else
STATIC uint8_t KnxTlc_Event_Connect_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Client-only. */
    /* Event 25.    */
    return (uint8_t)25;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Disconnect_Req(void)
#else
STATIC uint8_t KnxTlc_Event_Disconnect_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Client-only. */
    /* Event 26.    */
    return (uint8_t)26;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_DataConnected_Req(void)
#else
STATIC uint8_t KnxTlc_Event_DataConnected_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Client-only. */
    /* Event 15.    */
    return (uint8_t)15;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Connect_Con(void)
#else
STATIC uint8_t KnxTlc_Event_Connect_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Client only.         */
    /* Events 19 and 20.    */
    printf("EventConnectCon\n");
    if (KnxMsg_ScratchBufferPtr->status == KNX_E_OK) {
        return (uint8_t)19;
    } else {
        return (uint8_t)20;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Disconnect_Con(void)
#else
STATIC uint8_t KnxTlc_Event_Disconnect_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Event 21.    */
    return (uint8_t)21;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_DataConnected_Con(void)
#else
STATIC uint8_t KnxTlc_Event_DataConnected_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Event 22.    */
    return (uint8_t)22;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Ack_Con(void)
#else
STATIC uint8_t KnxTlc_Event_Ack_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Event 23.    */
    return (uint8_t)23;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Nak_Con(void)
#else
STATIC uint8_t KnxTlc_Event_Nak_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* Event 24.    */
    return (uint8_t)24;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Timeout_Con(void)
#else
STATIC uint8_t KnxTlc_Event_Timeout_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return (uint8_t)16;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Timeout_Ack(void)
#else
STATIC uint8_t KnxTlc_Event_Timeout_Ack(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (KnxTlc_GetRepetitionCount() < MAX_REP_COUNT) {
        return (uint8_t)17;
    } else {
        return (uint8_t)18;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxTlc_Event_Undefined(void)
#else
STATIC uint8_t KnxTlc_Event_Undefined(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return (uint8_t)27;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

