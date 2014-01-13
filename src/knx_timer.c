/*
 *   KONNEX/EIB-Protocol-Stack.
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

#include "knx_timer.h"

/*
   DWORD WINAPI MSTickerThread(LPVOID *lpThreadParm);
   HANDLE hTickerThread;
   DWORD TickerThreadID;
 */

#include "kdk/common/CPU_Primitives.h"

#define DISABLE_ALL_INTERRUPTS()    CPU_DISABLE_ALL_INTERRUPTS()
#define ENABLE_ALL_INTERRUPTS()     CPU_ENABLE_ALL_INTERRUPTS()

#define TMR_LOCK()      DISABLE_ALL_INTERRUPTS()
#define TMR_UNLOCK()    ENABLE_ALL_INTERRUPTS()


/*
** Local variables.
*/
static Tmr_TimerType KNX_Timer[TMR_NUM_TIMERS];
static Tmr_TickType  Tmr_SysMsCounter;
static Tmr_TickType  Tmr_SysSecondCounter;

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
    uint8_least idx;

    Tmr_SysMsCounter = Tmr_SysSecondCounter = (uint32)0UL;

    DISABLE_ALL_INTERRUPTS();

    for (idx = (uint8)0; idx < TMR_NUM_TIMERS; idx++) {
        KNX_Timer[idx].expire_counter  = (uint32)0UL;
        KNX_Timer[idx].state           = TMR_STATE_STOPPED;
        KNX_Timer[idx].base            = TMR_RESOLUTION_MS;
    }

    ENABLE_ALL_INTERRUPTS();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_Start(uint8 timer, Tmr_ResolutionType base, Tmr_TickType ticks)
#else
boolean KnxTmr_Start(uint8 timer, Tmr_ResolutionType base, Tmr_TickType ticks)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state & TMR_STATE_RUNNING)) {
            DISABLE_ALL_INTERRUPTS();
            KNX_Timer[timer].expire_counter    = ticks;
            KNX_Timer[timer].state             = TMR_STATE_RUNNING;
            KNX_Timer[timer].base              = base;
            ENABLE_ALL_INTERRUPTS();
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_Stop(uint8 timer)
#else
boolean KnxTmr_Stop(uint8 timer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        DISABLE_ALL_INTERRUPTS();
        KNX_Timer[timer].state = TMR_STATE_STOPPED;
        ENABLE_ALL_INTERRUPTS();
        return TRUE;
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_IsExpired(uint8 timer)
#else
boolean KnxTmr_IsExpired(uint8 timer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    Tmr_StateType state;

    if (timer < TMR_NUM_TIMERS) {
        DISABLE_ALL_INTERRUPTS();
        state                      = KNX_Timer[timer].state;
        KNX_Timer[timer].state    &= (~TMR_STATE_EXPIRED);  /* TODO: FixMe */

        ENABLE_ALL_INTERRUPTS();
        return (state & TMR_STATE_EXPIRED) == TMR_STATE_EXPIRED;
    } else {
        return FALSE;   /* Invalid Timer. */
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_IsRunning(uint8 timer)
#else
boolean KnxTmr_IsRunning(uint8 timer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        return (KNX_Timer[timer].state & TMR_STATE_RUNNING) == TMR_STATE_RUNNING;
    } else {
        return FALSE;   /* Invalid Timer. */
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxTmr_GetRemainder(uint8 timer, Tmr_TickRefType remainder)
#else
boolean KnxTmr_GetRemainder(uint8 timer, Tmr_TickRefType remainder)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (timer < TMR_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state & TMR_STATE_RUNNING)) {
            return FALSE;
        } else {
            DISABLE_ALL_INTERRUPTS();
            *remainder = KNX_Timer[timer].expire_counter;
            ENABLE_ALL_INTERRUPTS();
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
    Tmr_TickType t;

    DISABLE_ALL_INTERRUPTS();

    if (base == TMR_RESOLUTION_MS) {
        t = Tmr_SysMsCounter;
    } else if (base == TMR_RESOLUTION_SEC) {
        t = Tmr_SysSecondCounter;
    }

    ENABLE_ALL_INTERRUPTS();

    return t;
}


/*
   void Tmr_Delay(Tmr_TickType ms
   {

   }

   void Tmr_DelayHMS(WORD H,WORD M,WORD S)
   {
    Tmr_TickType delay_time,end_time;

    delay_time=((Tmr_TickType)H*60*60)+((Tmr_TickType)M*60)+((Tmr_TickType)S);	// in Seconds !!!

    end_time=delay_time+Tmr_SysSecondCounter;

   //	while
   }
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTmr_SecondCallback(void)
#else
void KnxTmr_SecondCallback(void)
#endif /* KSTACK_MEMORY_MAPPING */
{

}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTmr_SystemTickHandler(void)
#else
void KnxTmr_SystemTickHandler(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    Tmr_TimerType *  tm;
    uint8           idx;
    boolean         SecondChanged = FALSE;

    Tmr_SysMsCounter++;

    if ((Tmr_SysMsCounter % (uint32)1000UL) == (uint32)0UL) {
        Tmr_SysSecondCounter++;
        SecondChanged = TRUE;
        KnxTmr_SecondCallback();
    }

    for (idx = (uint8)0; idx < TMR_NUM_TIMERS; idx++) {
        tm = &KNX_Timer[idx];

        if ((tm->state & TMR_STATE_RUNNING) == TMR_STATE_RUNNING) {
            if (tm->base == TMR_RESOLUTION_MS) {
                if (--tm->expire_counter == (uint32)0UL) {
                    tm->state = TMR_STATE_EXPIRED;
                }
            } else if (tm->base == TMR_RESOLUTION_SEC) {
                tm->expire_counter -= (uint32)1UL;
                if ((SecondChanged == TRUE) && (tm->expire_counter == (uint32)0UL)) {
                    tm->state = TMR_STATE_EXPIRED;
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

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
