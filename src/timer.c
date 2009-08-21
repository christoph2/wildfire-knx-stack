/*
**
**	check:	Zyklischer Timer - wieso nicht???
**			Wenn Zyklische Timer, dann über einen Cycle-/Duty-Mechanismus
**			nachdenken ==> Soft-PWM für Stellantriebe, dadurch darf aber
**			die allgemeine Verwendbarkeit nicht erschwert werden!!!
**			(eine 'SetDuty()' Funktion genügt).
**			Eine Struktur an Parameter könnte effizienter sein!!??
**
*/

/*
**	todo: Einstellbare Tickrate: [1|10]ms (Vieleicht auch 100???).
*/

/*
** todo: Hardware-Timer wie in miniOSEK unterstützen!!!
*/

#include "timer.h"

/*
DWORD WINAPI MSTickerThread(LPVOID *lpThreadParm);
HANDLE hTickerThread;
DWORD TickerThreadID;
*/

/* todo: ins Target-Verzeichnis!!! */
#define DISABLE_ALL_INTERRUPTS()
#define ENABLE_ALL_INTERRUPTS()

static TM_TimerType KNX_Timer[TM_NUM_TIMERS];	/*  todo: Sinnvoller Name!!! */
static TM_TickType TM_SysMsCounter;
static TM_TickType TM_SysSecondCounter;

void TM_Init(void)
{
    uint8_least idx;
	
    TM_SysMsCounter=TM_SysSecondCounter=0UL;
	
    DISABLE_ALL_INTERRUPTS();
    for (idx=0;idx<TM_NUM_TIMERS;idx++) {
        KNX_Timer[idx].expire_counter=0UL;
        KNX_Timer[idx].state=TM_STATE_STOPPED;
        KNX_Timer[idx].base=TM_BASE_MS;
    }
    ENABLE_ALL_INTERRUPTS();
	/* check: Hardware-Timer starten??? */
}

boolean TM_Start(uint8 timer,TM_BaseType base,TM_TickType ticks)
{
/* check: Timer-Restart eigentlich besser!!?? */

    if (timer<TM_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state & TM_STATE_RUNNING)) {	
            DISABLE_ALL_INTERRUPTS();
            KNX_Timer[timer].expire_counter=ticks;
            KNX_Timer[timer].state=TM_STATE_RUNNING;
            KNX_Timer[timer].base=base;
            ENABLE_ALL_INTERRUPTS();
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

boolean TM_Stop(uint8 timer)
{
    if (timer<TM_NUM_TIMERS) {
        DISABLE_ALL_INTERRUPTS();
        KNX_Timer[timer].state=TM_STATE_STOPPED;
        ENABLE_ALL_INTERRUPTS();
        return TRUE;		
    } else {
        return FALSE;
    }
}

boolean TM_IsExpired(uint8 timer)
{
    TM_StateType state;
	
    if (timer<TM_NUM_TIMERS) {
        DISABLE_ALL_INTERRUPTS();
        state=KNX_Timer[timer].state;
        KNX_Timer[timer].state&=(~TM_STATE_EXPIRED);	/* check: muss|darf das sein!!?? */
                                                        /* 'TM_STATE_STOPPED' setzen!!?? */
        ENABLE_ALL_INTERRUPTS();														
            return ((state & TM_STATE_EXPIRED)==TM_STATE_EXPIRED);
    } else {
        return FALSE;	/* Invalid Timer. */
    }
}

boolean TM_IsRunning(uint8 timer)
{
    if (timer<TM_NUM_TIMERS) {
        return ((KNX_Timer[timer].state & TM_STATE_RUNNING)==TM_STATE_RUNNING);
    } else {
        return FALSE;	/* Invalid Timer. */
    }
}

boolean TM_GetRemainder(uint8 timer,TM_TickRefType remainder)
{
    if (timer<TM_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state & TM_STATE_RUNNING)) {
            return FALSE;
        } else {
            DISABLE_ALL_INTERRUPTS();
            *remainder=KNX_Timer[timer].expire_counter;
            ENABLE_ALL_INTERRUPTS();
            return TRUE;
        }
    } else {
        return FALSE;
    }
}

TM_TickType TM_GetSystemTime(TM_BaseType base)
{
    TM_TickType t;
	
    DISABLE_ALL_INTERRUPTS();
    if (base==TM_BASE_MS) {
        t=TM_SysMsCounter;
    } else if (base==TM_BASE_SEC) {
        t=TM_SysSecondCounter;
    }		
    ENABLE_ALL_INTERRUPTS();
	
    return t;
}

/*
void TM_Delay(TM_TickType ms)  // check: Wie optimal integrieren?
{

}

//
//	Hinweis: 'TM_DelayHMS' steht nur in Verbindung mit miniOSEK zu Verfügung!!!
//
void TM_DelayHMS(WORD H,WORD M,WORD S)
{
	TM_TickType delay_time,end_time;
	
	delay_time=((TM_TickType)H*60*60)+((TM_TickType)M*60)+((TM_TickType)S);	// in Sekunden!!!
	
	end_time=delay_time+TM_SysSecondCounter;
	
//	while 
}
*/

void TM_SecondCallback(void)	/* Hinweis: Konfigurations-abhängig!!! */
{

}


void TM_SystemTimeHandler(void)
{
    TM_TimerType *tm;
    uint8 idx;
    boolean SecondChanged=FALSE;
	
    TM_SysMsCounter++;

    if ((TM_SysMsCounter % 1000UL)==0UL) {
        TM_SysSecondCounter++;
        SecondChanged=TRUE;
        TM_SecondCallback();	/* Hinweis: wenn konfiguriert!!! */	
    }
	
    for (idx=0;idx<TM_NUM_TIMERS;idx++) { 
        tm=&KNX_Timer[idx];

        if ((tm->state & TM_STATE_RUNNING)==TM_STATE_RUNNING) {
            if (tm->base==TM_BASE_MS) {
                if (--tm->expire_counter==0UL) {
                    tm->state=TM_STATE_EXPIRED;
                }
            } else if (tm->base==TM_BASE_SEC) {
                if ((SecondChanged==TRUE) && (--tm->expire_counter==0UL)) {
                    tm->state=TM_STATE_EXPIRED;
                }
            }
        }
    }
}

/*
---
2.3.4 Function TmAddStart
Prototype:
	void TmAddStart(TIMER* pTimer, ULONG ticks)
Description:
	This function restarts a timer for the specified ‘ticks’ 
	measured from the last expiration of the timer. Use this 
	function if you want to get timer intervals that do not
	drift away from the real time like it would be if you use ‘TmStart’.
	
Parameters:
TIMER* pTimer:
	A pointer to a timer that should be started from the 
	last expiration
ULONG ticks:
	The number of ticks the timer should run before it will 
	be expired.
Return values:
	none
Comment:
	Before a call of ‘TmAddStart’ there must be at least one 
	call of ‘TmStart’.
*/

/*
DWORD WINAPI MSTickerThread(LPVOID *lpThreadParm)
{
//      struct timeval tv;
//      tv.tv_sec = 0;
//      tv.tv_usec = 35000;     // 35000 Mikrosekunden = 35 Millisekunden
//      select(0, NULL, NULL, NULL, &tv);
    UNREFERENCED_PARAMETER(lpThreadParm);

    while (TRUE) {
        Sleep(1);
        TM_Ticker_Callback();
    }

    return 0;
}
*/

/*
void TimerTest(void)
{
        int t;
        boolean elapsed;

        hTickerThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)MSTickerThread,0,0,&TickerThreadID);  

        TM_Init();
        TM_Start(0,TM_RESOLUTION_ONE_SEC,TM_MODE_PERIODIC,5);
        TM_Start(1,TM_RESOLUTION_ONE_MS,TM_MODE_PERIODIC,1500);
        TM_Start(4,TM_RESOLUTION_ONE_MS,TM_MODE_ONE_SHOT,380);
        
        while (1) {
                // TM_Ticker_Callback();
                t=TM_Elapsed(0,&elapsed);
                if (elapsed==TRUE) {
                        printf("Timer #0\n");
                }

                t=TM_Elapsed(1,&elapsed);
                if (elapsed==TRUE) {
                        printf("Timer #1\n");
                }

                t=TM_Elapsed(4,&elapsed);
                if (elapsed==TRUE) {
                        printf("Timer #4\n");
                }
        }

}
*/

