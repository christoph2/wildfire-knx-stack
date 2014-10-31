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
#if !defined(__STATEMACHINE_KNX)
#define __STATEMACHINE_KNX

#include "knx_messaging.h"
#include "knx_tlc.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


#define MAX_REP_COUNT           ((uint8_t)3)
#define TLC_CONNECTION_TIMEOUT  ((Tmr_TickType)6)
#define TLC_ACKNOWLEDGE_TIMEOUT ((Tmr_TickType)3)

/*
** Global types.
*/

typedef enum tagKNX_TlcEventType {
    KNX_TLC_EVENT_CONNECT_IND,
    KNX_TLC_EVENT_DISCONNECT_IND,
    KNX_TLC_EVENT_DATA_CONNECTED_IND,
    KNX_TLC_EVENT_ACK_IND,
    KNX_TLC_EVENT_NAK_IND,
    KNX_TLC_EVENT_CONNECT_REQ,
    KNX_TLC_EVENT_DISCONNECT_REQ,
    KNX_TLC_EVENT_DATA_CONNECTED_REQ,
    KNX_TLC_EVENT_CONNECT_CON,
    KNX_TLC_EVENT_DISCONNECT_CON,
    KNX_TLC_EVENT_DATA_CONNECTED_CON,
    KNX_TLC_EVENT_ACK_CON,
    KNX_TLC_EVENT_NAK_CON,
    KNX_TLC_EVENT_TIMEOUT_CON,
    KNX_TLC_EVENT_TIMEOUT_ACK,
    KNX_TLC_EVENT_UNDEFINED
} KNX_TlcEventType;


typedef enum tagKnxTlc_StateType {
    KNX_TLC_STATE_CLOSED,
    KNX_TLC_STATE_OPEN_IDLE,
    KNX_TLC_STATE_OPEN_WAIT,
    KNX_TLC_STATE_CONNECTING
} KnxTlc_StateType;


typedef void (*KnxTlc_ActionFuncType)(void);

typedef struct tagKnxTlc_ActionType {
    KnxTlc_ActionFuncType   Function;
    KnxTlc_StateType        Next;
} KnxTlc_ActionType;

typedef struct tagKnxTlc_ActionListType {
/*    const uint8_t num; */
#if (KNX_TL_STATEMACHINE_STYLE == 1) || (KNX_TL_STATEMACHINE_STYLE == 2)
    const KnxTlc_ActionType Action[3];
#elif KNX_TL_STATEMACHINE_STYLE == 3
    const KnxTlc_ActionType Action[4];
#else
#error "KNX_TL_STATEMACHINE_STYLE muss entweder 1,2 oder 3 sein!"
#endif
} KnxTlc_ActionListType;

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_StateMachine(KNX_TlcEventType event);

FUNC(KnxTlc_StateType, KSTACK_CODE) KnxTlc_GetState(void);
FUNC(void, KSTACK_CODE) KnxTlc_SetState(KnxTlc_StateType State);
#else
void KnxTlc_StateMachine(KNX_TlcEventType event);

KnxTlc_StateType KnxTlc_GetState(void);
void KnxTlc_SetState(KnxTlc_StateType State);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __STATEMACHINE_KNX */
