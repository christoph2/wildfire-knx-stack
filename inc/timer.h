#if !defined(__TIMER_H)
#define __TIMER_H

/* #include "target.h" */
#include <Std_Types.h>

void TimerTest(void);

#define	TM_TIMER_TLC_CON_TIMEOUT	((uint8)0)
#define	TM_TIMER_TLC_ACK_TIMEOUT	((uint8)1)

#define TM_NUM_TIMERS   5	/* Hinweis: Projekt-abhängig!!! */

#if TM_NUM_TIMERS<3
    #error "ERROR: Number of Timers must be at least two!!!"
#endif

typedef uint32 TM_TickType,*TM_TickRefType;

typedef enum tagTM_BaseType {
    TM_BASE_MS,
    TM_BASE_SEC
} TM_BaseType;

typedef enum tagTM_StateType {              /* Konstanten für die Timer-State-Machines. */
    TM_STATE_STOPPED,
    TM_STATE_RUNNING,
    TM_STATE_EXPIRED
} TM_StateType;


typedef struct tagTM_TimerType {
    TM_StateType state;	        
    TM_BaseType base;        
    TM_TickType expire_counter;
} TM_TimerType;

void TM_Init(void);

boolean TM_Start(uint8 timer,TM_BaseType base,TM_TickType ticks);
boolean TM_Stop(uint8 timer);

boolean TM_IsExpired(uint8 timer);
boolean TM_IsRunning(uint8 timer);

boolean TM_GetRemainder(uint8 timer,TM_TickRefType remainder);

TM_TickType TM_GetSystemTime(TM_BaseType base);

void TM_Delay(TM_TickType ms);
void TM_DelayHMS(uint16 H,uint16 M,uint16 S);

void TM_SystemTimeHandler(void);
void TM_SecondCallback(void);

#endif /* __TIMER_H */
