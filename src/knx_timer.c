/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
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

#include "knx_timer.h"

/*
** Local Constants.
*/

/** Link-Layer Timeout in Milli-Seconds.
*
*/
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define KNX_LL_TIMEOUT  (500)
#else
#define KNX_LL_TIMEOUT  (25)
#endif

/*
** Local variables.
*/
STATIC Tmr_TimerType KNX_Timer[TMR_NUM_TIMERS];
STATIC Tmr_TickType  Tmr_SysMsCounter;
STATIC Tmr_TickType  Tmr_SysSecondCounter;
STATIC uint16_t      Tmr_DataLinkCounter;
STATIC boolean       Tmr_DataLinkTimerRunning;


/*
** Required Interfaces.
*/
void KnxLL_TimeoutCB(void);

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTmr_Init(void)
#else
void KnxTmr_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t idx;

    Tmr_SysMsCounter = Tmr_SysSecondCounter = (Tmr_TickType)0;
    Tmr_DataLinkCounter = (uint16_t)0U;
    Tmr_DataLinkTimerRunning = FALSE;
    TMR_LOCK_MAIN_TIMER();
    for (idx = (uint8_t)0; idx < TMR_NUM_TIMERS; idx++) {
        KNX_Timer[idx].expire_counter  = (uint32_t)0UL;
        KNX_Timer[idx].state           = TMR_STATE_STOPPED;
        KNX_Timer[idx].base            = TMR_RESOLUTION_MS;
    }
    TMR_UNLOCK_MAIN_TIMER();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks)
#else
boolean KnxTmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state & TMR_STATE_RUNNING)) {
            TMR_LOCK_MAIN_TIMER();
            KNX_Timer[timer].expire_counter    = ticks;
            KNX_Timer[timer].state             = TMR_STATE_RUNNING;
            KNX_Timer[timer].base              = base;
            TMR_UNLOCK_MAIN_TIMER();
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_Stop(uint8_t timer)
#else
boolean KnxTmr_Stop(uint8_t timer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        TMR_LOCK_MAIN_TIMER();
        KNX_Timer[timer].state = TMR_STATE_STOPPED;
        TMR_UNLOCK_MAIN_TIMER();
        return TRUE;
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_IsExpired(uint8_t timer)
#else
boolean KnxTmr_IsExpired(uint8_t timer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    Tmr_StateType state;

    if (timer < TMR_NUM_TIMERS) {
        TMR_LOCK_MAIN_TIMER();
        state                      = KNX_Timer[timer].state;
        TMR_UNLOCK_MAIN_TIMER();
        return (state & TMR_STATE_EXPIRED) == TMR_STATE_EXPIRED;
    } else {
        return FALSE;   /* Invalid Timer. */
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_IsRunning(uint8_t timer)
#else
boolean KnxTmr_IsRunning(uint8_t timer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        return (KNX_Timer[timer].state & TMR_STATE_RUNNING) == TMR_STATE_RUNNING;
    } else {
        return FALSE;   /* Invalid Timer. */
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_GetRemainder(uint8_t timer, Tmr_TickRefType remainder)
#else
boolean KnxTmr_GetRemainder(uint8_t timer, Tmr_TickRefType remainder)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state & TMR_STATE_RUNNING)) {
            return FALSE;
        } else {
            TMR_LOCK_MAIN_TIMER();
            *remainder = KNX_Timer[timer].expire_counter;
            TMR_UNLOCK_MAIN_TIMER();
            return TRUE;
        }
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Tmr_TickType, KSTACK_CODE) KnxTmr_GetSystemTime(Tmr_ResolutionType base)
#else
Tmr_TickType KnxTmr_GetSystemTime(Tmr_ResolutionType base)
#endif /* KSTACK_MEMORY_MAPPING */
{
    Tmr_TickType timerValue = (Tmr_TickType)0UL;

    TMR_LOCK_MAIN_TIMER();

    if (base == TMR_RESOLUTION_MS) {
        timerValue = Tmr_SysMsCounter;
    } else if (base == TMR_RESOLUTION_SEC) {
        timerValue = Tmr_SysSecondCounter;
    } else {
        ASSERT(FALSE);
    }

    TMR_UNLOCK_MAIN_TIMER();

    return timerValue;
}


/*
   void Tmr_Delay(Tmr_TickType ms
   {

   }

   void Tmr_DelayHMS(WORD H,WORD M,WORD S)
   {
    Tmr_TickType delay_time,end_time;

    delay_time=((Tmr_TickType)H*60*60)+((Tmr_TickType)M*60)+((Tmr_TickType)S);  // in Seconds !!!

    end_time=delay_time+Tmr_SysSecondCounter;

   //   while
   }
 */

#include <time.h>
#include "knx_debug.h"

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTmr_SecondCallback(void)
#else
void KnxTmr_SecondCallback(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    clock_t end, start = clock();
    double elapsedTime;

    end = clock();
    elapsedTime = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("elapsedTime: %f\n", elapsedTime);

    //printf("   One second elapsed.\n");
}

/*
**
** Functions related to Data-Link Timer.
**
*/
void KnxTmr_DataLinkTimerStart(void)
{
    TMR_LOCK_DL_TIMER();
    if (!KnxTmr_DataLinkTimerIsRunning()) {
        Tmr_DataLinkCounter = (uint16_t)0U;
        Tmr_DataLinkTimerRunning = TRUE;
    }
    TMR_UNLOCK_DL_TIMER();
}

void KnxTmr_DataLinkTimerStop(void)
{
    TMR_LOCK_DL_TIMER();
    if (KnxTmr_DataLinkTimerIsRunning()) {
        Tmr_DataLinkTimerRunning = FALSE;
    }
    TMR_UNLOCK_DL_TIMER();
}

boolean KnxTmr_DataLinkTimerIsRunning(void)
{
    boolean result;

    TMR_LOCK_DL_TIMER();
    result = Tmr_DataLinkTimerRunning;
    TMR_UNLOCK_DL_TIMER();
    return result;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTmr_SystemTickHandler(void)
#else
void KnxTmr_SystemTickHandler(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    Tmr_TimerType * tm;
    uint8_t idx;
    boolean SecondChanged = FALSE;

    Tmr_SysMsCounter += TMR_TICK_RESOLUTION;

    if (KnxTmr_DataLinkTimerIsRunning()) {
        TMR_LOCK_DL_TIMER();
        Tmr_DataLinkCounter += TMR_TICK_RESOLUTION;
        if (Tmr_DataLinkCounter >= KNX_LL_TIMEOUT) {
            KnxLL_TimeoutCB();  // Link-Layer timed out.
            KnxTmr_DataLinkTimerStop();
        }
        TMR_UNLOCK_DL_TIMER();
    }

    if ((Tmr_SysMsCounter % (uint32_t)1000UL) == (uint32_t)0UL) {
        Tmr_SysSecondCounter++;
        SecondChanged = TRUE;
        KnxTmr_SecondCallback();
    }

    for (idx = (uint8_t)0; idx < TMR_NUM_TIMERS; idx++) {
        tm = &KNX_Timer[idx];

        if ((tm->state & TMR_STATE_RUNNING) == TMR_STATE_RUNNING) {
            if (tm->base == TMR_RESOLUTION_MS) {
                if (--tm->expire_counter == (uint32_t)0UL) {
                    tm->state = TMR_STATE_EXPIRED;
                }
            } else if (tm->base == TMR_RESOLUTION_SEC) {
                TMR_LOCK_MAIN_TIMER();
                if (SecondChanged == TRUE)  {
                    tm->expire_counter -= (uint32_t)1UL;
                    if (tm->expire_counter == (uint32_t)0UL) {
                        tm->state = TMR_STATE_EXPIRED;
                        if (KnxTmr_Callbacks[idx] != NULL) {
                            KnxTmr_Callbacks[idx]();
                        }
                    }
                }
                TMR_UNLOCK_MAIN_TIMER();
            }
        }
    }
    SecondChanged = FALSE;
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

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

