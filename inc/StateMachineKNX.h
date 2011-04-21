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
#if !defined(__STATEMACHINE_KNX)
#define __STATEMACHINE_KNX

#include "Messaging.h"
#include "Transport.h"

#define MAX_REP_COUNT   ((uint8)3)


#define TL_STYLE       3    /* [1,2,3] - Transport-Layer-Styles gem. KNX-Handbuch 3/3/4. */

typedef enum tagKNXTlc_StateType {
    CLOSED,
    OPEN_IDLE,
    OPEN_WAIT,
    CONNECTING
} KNXTlc_StateType;

typedef enum {
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

typedef void (*KNXTlc_ActionFuncType)(void);

typedef struct tagKNXTlc_ActionType {
    KNXTlc_ActionFuncType Function;
    KNXTlc_StateType Next;
} KNXTlc_ActionType;

typedef struct tagKNXTlc_ActionListType {
/*    const uint8 num; */
#if (TL_STYLE==1) || (TL_STYLE==2)
    const KNXTlc_ActionType Action[3];
#elif TL_STYLE==3
    const KNXTlc_ActionType Action[4];
#else
#error "TL_STYLE muss entweder 1,2 oder 3 sein!"
#endif
} KNXTlc_ActionListType;


#define	TLC_CONNECTION_TIMEOUT		((TM_TickType)6)
#define	TLC_ACKNOWLEDGE_TIMEOUT		((TM_TickType)3)

/*  #define Def_Rep_Count               ((uint8)3) */

void KNXTlc_StateMachine(KNX_TlcEventType event);

KNXTlc_StateType KNXTlc_GetState(void);
void KNXTlc_SetState(KNXTlc_StateType State);

#endif /* __STATEMACHINE_KNX */
