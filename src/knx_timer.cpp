/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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

#include "knx_timer.hpp"
#include "knx_debug.hpp"
#include <time.h>

namespace knx {

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
STATIC bool       Tmr_DataLinkTimerRunning;


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
    Tmr_DataLinkTimerRunning = false;
    TMR_LOCK_MAIN_TIMER();
    for (idx = (uint8_t)0; idx < TMR_NUM_TIMERS; idx++) {
        KNX_Timer[idx].expire_counter  = (uint32_t)0UL;
        KNX_Timer[idx].state           = Tmr_StateType::STOPPED;
        KNX_Timer[idx].base            = Tmr_ResolutionType::MS;
    }
    TMR_UNLOCK_MAIN_TIMER();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(bool, KSTACK_CODE) KnxTmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks)
#else
bool KnxTmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state == Tmr_StateType::RUNNING)) {
            TMR_LOCK_MAIN_TIMER();
            KNX_Timer[timer].expire_counter = ticks;
            KNX_Timer[timer].state = Tmr_StateType::RUNNING;
            KNX_Timer[timer].base = base;
            TMR_UNLOCK_MAIN_TIMER();
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(bool, KSTACK_CODE) KnxTmr_Stop(uint8_t timer)
#else
bool KnxTmr_Stop(uint8_t timer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        TMR_LOCK_MAIN_TIMER();
        KNX_Timer[timer].state = Tmr_StateType::STOPPED;
        TMR_UNLOCK_MAIN_TIMER();
        return true;
    } else {
        return false;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(bool, KSTACK_CODE) KnxTmr_IsExpired(uint8_t timer)
#else
bool KnxTmr_IsExpired(uint8_t timer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    Tmr_StateType state;

    if (timer < TMR_NUM_TIMERS) {
        TMR_LOCK_MAIN_TIMER();
        state = KNX_Timer[timer].state;
        TMR_UNLOCK_MAIN_TIMER();
        return state  == Tmr_StateType::EXPIRED;
    } else {
        return false;   /* Invalid Timer. */
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(bool, KSTACK_CODE) KnxTmr_IsRunning(uint8_t timer)
#else
bool KnxTmr_IsRunning(uint8_t timer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        return KNX_Timer[timer].state == Tmr_StateType::RUNNING;
    } else {
        return false;   /* Invalid Timer. */
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(bool, KSTACK_CODE) KnxTmr_GetRemainder(uint8_t timer, Tmr_TickType& remainder)
#else
bool KnxTmr_GetRemainder(uint8_t timer, Tmr_TickType& remainder)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state == Tmr_StateType::RUNNING)) {
            return false;
        } else {
            TMR_LOCK_MAIN_TIMER();
            remainder = KNX_Timer[timer].expire_counter;
            TMR_UNLOCK_MAIN_TIMER();
            return true;
        }
    } else {
        return false;
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

    if (base == Tmr_ResolutionType::MS) {
        timerValue = Tmr_SysMsCounter;
    } else if (base == Tmr_ResolutionType::SEC) {
        timerValue = Tmr_SysSecondCounter;
    } else {
        ASSERT(false);
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
        Tmr_DataLinkTimerRunning = true;
    }
    TMR_UNLOCK_DL_TIMER();
}

void KnxTmr_DataLinkTimerStop(void)
{
    TMR_LOCK_DL_TIMER();
    if (KnxTmr_DataLinkTimerIsRunning()) {
        Tmr_DataLinkTimerRunning = false;
    }
    TMR_UNLOCK_DL_TIMER();
}

bool KnxTmr_DataLinkTimerIsRunning(void)
{
    bool result;

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
    bool SecondChanged = false;

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
        SecondChanged = true;
        KnxTmr_SecondCallback();
    }

    for (idx = (uint8_t)0; idx < TMR_NUM_TIMERS; idx++) {
        tm = &KNX_Timer[idx];

        if (tm->state == Tmr_StateType::RUNNING) {
            if (tm->base == Tmr_ResolutionType::MS) {
                if (--tm->expire_counter == (uint32_t)0UL) {
                    tm->state = Tmr_StateType::EXPIRED;
                }
            } else if (tm->base == Tmr_ResolutionType::SEC) {
                TMR_LOCK_MAIN_TIMER();
                if (SecondChanged == true)  {
                    tm->expire_counter -= (uint32_t)1UL;
                    if (tm->expire_counter == (uint32_t)0UL) {
                        tm->state = Tmr_StateType::EXPIRED;
                        if (KnxTmr_Callbacks[idx] != NULL) {
                            KnxTmr_Callbacks[idx]();
                        }
                    }
                }
                TMR_UNLOCK_MAIN_TIMER();
            }
        }
    }
    SecondChanged = false;
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

} // namespace knx

