/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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

#define MAX_REP_COUNT   ((uint8)3)

typedef enum tagKNX_TlcStateType {
    CLOSED,OPEN_IDLE,OPEN_WAIT,CONNECTING
} KNX_TlcStateType;

typedef enum {
    tlcCONNECT_IND,tlcDISCONNECT_IND,tlcDATA_CONNECTED_IND,
    tlcACK_IND,tlcNAK_IND,tlcCONNECT_REQ,tlcDISCONNECT_REQ,
    tlcDATA_CONNECTED_REQ,tlcCONNECT_CON,tlcDISCONNECT_CON,
    tlcDATA_CONNECTED_CON,tlcACK_CON,tlcNAK_CON,
    tlcTIMEOUT_CON,tlcTIMEOUT_ACK,tlcUNDEFINED
} KNX_TlcEventType;

#define TL_STYLE        3           /* [1,2,3] - Transport-Layer-Styles gem. KNX-Handbuch 3/3/4. */
                                  
typedef void (*KNX_TlcActionFuncType)(void);

typedef struct tagKNX_TlcActionType {
        KNX_TlcActionFuncType Function;
        KNX_TlcStateType Next;
} KNX_TlcActionType;

typedef struct tagKNX_TlcActionListType {
        uint8 num;
#if (TL_STYLE==1) || (TL_STYLE==2)
        KNX_TlcActionType  Action[3];
#elif TL_STYLE==3
        KNX_TlcActionType Action[4];
#else
#error "TL_STYLE muss entweder 1,2 oder 3 sein!"
#endif
} KNX_TlcActionListType;


#define	TLC_CONNECTION_TIMEOUT		((TM_TickType)6)
#define	TLC_ACKNOWLEDGE_TIMEOUT		((TM_TickType)3)

/*  #define Def_Rep_Count               ((uint8)3) */

void TL_StateMachine(KNX_TlcEventType event);

KNX_TlcStateType KNXTlcGetState(void);
void KNXTlcSetState(KNX_TlcStateType State);

#endif /* __STATEMACHINE_KNX */

