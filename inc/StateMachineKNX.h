#if !defined(__STATEMACHINE_KNX)
#define __STATEMACHINE_KNX

#include "Messaging.h"

#define MAX_REP_COUNT   ((uint8)3)

typedef enum tagKNX_TlcStateType {  /* todo: Präfix 'tlc' (vieeel Arbeit...). */
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
                                    /* Hinweis: Projekt-Abhängig, weg hier!!! */

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
