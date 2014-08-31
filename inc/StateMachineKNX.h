/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
*                                       cpu12.gems@googlemail.com>
*
*   All Rights Reserved
*
*  This program is free softwKNXe; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free SoftwKNXe Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
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

/* TODO: Config. */
#define MAX_REP_COUNT           ((uint8_t)3)
#define TL_STYLE                3 /* [1,2,3] - Transport-Layer-Styles gem. KNX-Handbuch 3/3/4. */
#define TLC_CONNECTION_TIMEOUT  ((Tmr_TickType)6)
#define TLC_ACKNOWLEDGE_TIMEOUT ((Tmr_TickType)3)
/*  #define Def_Rep_Count               ((uint8_t)3) */

/*
** Global types.
*/
typedef enum tagKnxTLC_StateType {
    CLOSED,
    OPEN_IDLE,
    OPEN_WAIT,
    CONNECTING
} KnxTLC_StateType;

typedef enum tagKNX_TlcEventType {
    tlcCONNECT_IND,
    tlcDISCONNECT_IND,
    tlcDATA_CONNECTED_IND,
    tlcACK_IND,
    tlcNAK_IND,
    tlcCONNECT_REQ,
    tlcDISCONNECT_REQ,
    tlcDATA_CONNECTED_REQ,
    tlcCONNECT_CON,
    tlcDISCONNECT_CON,
    tlcDATA_CONNECTED_CON,
    tlcACK_CON,
    tlcNAK_CON,
    tlcTIMEOUT_CON,
    tlcTIMEOUT_ACK,
    tlcUNDEFINED
} KNX_TlcEventType;

typedef void (*KnxTLC_ActionFuncType)(void);

typedef struct tagKnxTLC_ActionType {
    KnxTLC_ActionFuncType   Function;
    KnxTLC_StateType        Next;
} KnxTLC_ActionType;

typedef struct tagKnxTLC_ActionListType {
/*    const uint8_t num; */
#if (TL_STYLE == 1) || (TL_STYLE == 2)
    const KnxTLC_ActionType Action[3];
#elif TL_STYLE == 3
    const KnxTLC_ActionType Action[4];
#else
#error "TL_STYLE muss entweder 1,2 oder 3 sein!"
#endif
} KnxTLC_ActionListType;

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)         KnxTLC_StateMachine(KNX_TlcEventType event);

FUNC(KnxTLC_StateType, KSTACK_CODE) KnxTLC_GetState(void);
FUNC(void, KSTACK_CODE)         KnxTLC_SetState(KnxTLC_StateType State);
#else
void KnxTLC_StateMachine(KNX_TlcEventType event);

KnxTLC_StateType    KnxTLC_GetState(void);
void                KnxTLC_SetState(KnxTLC_StateType State);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __STATEMACHINE_KNX */

