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
constexpr uint16_t KNX_LL_TIMEOUT = 500;
#else
constexpr uint16_t KNX_LL_TIMEOUT = 25;
#endif

/*
** Local variables.
*/
static uint16_t Tmr_DataLinkCounter;
static bool Tmr_DataLinkTimerRunning;

/*
** Global functions.
*/

#if 0
void Tmr_Init(void)
{
    uint8_t idx;

    Tmr_DataLinkCounter = (uint16_t)0U;
    Tmr_DataLinkTimerRunning = false;
    TMR_LOCK_MAIN_TIMER();
    for (idx = (uint8_t)0; idx < TMR_NUM_TIMERS; idx++) {
        KNX_Timer[idx].expire_counter  = (uint32_t)0UL;
        KNX_Timer[idx].state           = State::STOPPED;
        KNX_Timer[idx].base            = Resolution::MS;
    }
    TMR_UNLOCK_MAIN_TIMER();
}
#endif

Tmr_TickType Timer::msCounter = 0;
Tmr_TickType Timer::secondCounter = 0;

std::array<Timer*, TMR_NUM_TIMERS> Timer::instances {};
uint8_t instanceCounter = 0;


constexpr Timer::Timer() noexcept : state {State::STOPPED}, expire_counter {0UL}, base {Resolution::MS}
{
    instances[instanceCounter++] = this;
}


bool Timer::start(Resolution base, Tmr_TickType ticks)
{
    if (!(state == State::RUNNING)) {
        TMR_LOCK_MAIN_TIMER();
        expire_counter = ticks;
        state = State::RUNNING;
        base = base;
        TMR_UNLOCK_MAIN_TIMER();
        return true;
    } else {
        return false;
    }
}


bool Timer::stop()
{
    // TODO: Check state1!!
    TMR_LOCK_MAIN_TIMER();
    state = State::STOPPED;
    TMR_UNLOCK_MAIN_TIMER();
    return true;
}


bool Timer::isExpired() const
{
    return state == State::EXPIRED;
}


bool Timer::isRunning() const
{
    return state == State::EXPIRED;
}


bool Timer::getRemainder(Tmr_TickType& remainder) const
{
    if (!(state == State::RUNNING)) {
        return false;
    } else {
        TMR_LOCK_MAIN_TIMER();
        remainder = expire_counter;
        TMR_UNLOCK_MAIN_TIMER();
        return true;
    }
}


Tmr_TickType Timer::getSystemTime(Resolution res)
{
    Tmr_TickType timerValue = 0UL;

    TMR_LOCK_MAIN_TIMER();

    if (res == Resolution::MS) {
        timerValue = msCounter;
    } else if (res == Resolution::SEC) {
        timerValue = secondCounter;
    } else {
        ASSERT(false);
    }

    TMR_UNLOCK_MAIN_TIMER();

    return timerValue;
}

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) Tmr_SecondCallback(void)
#else
void Tmr_SecondCallback(void)
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
void Tmr_DataLinkTimerStart(void)
{
    TMR_LOCK_DL_TIMER();
    if (!Tmr_DataLinkTimerIsRunning()) {
        Tmr_DataLinkCounter = (uint16_t)0U;
        Tmr_DataLinkTimerRunning = true;
    }
    TMR_UNLOCK_DL_TIMER();
}

void Tmr_DataLinkTimerStop(void)
{
    TMR_LOCK_DL_TIMER();
    if (Tmr_DataLinkTimerIsRunning()) {
        Tmr_DataLinkTimerRunning = false;
    }
    TMR_UNLOCK_DL_TIMER();
}

bool Tmr_DataLinkTimerIsRunning(void)
{
    bool result;

    TMR_LOCK_DL_TIMER();
    result = Tmr_DataLinkTimerRunning;
    TMR_UNLOCK_DL_TIMER();
    return result;
}

void Timer::tickHandler()
{
    uint8_t idx;
    bool SecondChanged = false;

    msCounter += TMR_TICK_RESOLUTION;

    if (Tmr_DataLinkTimerIsRunning()) {
        TMR_LOCK_DL_TIMER();
        Tmr_DataLinkCounter += TMR_TICK_RESOLUTION;
        if (Tmr_DataLinkCounter >= KNX_LL_TIMEOUT) {
//            KnxLL_TimeoutCB();  // Link-Layer timed out.
            Tmr_DataLinkTimerStop();
        }
        TMR_UNLOCK_DL_TIMER();
    }

    if ((msCounter % (uint32_t)1000UL) == (uint32_t)0UL) {
        secondCounter++;
        SecondChanged = true;
//        Tmr_SecondCallback();
    }
#if 0
    for (idx = (uint8_t)0; idx < TMR_NUM_TIMERS; idx++) {
        tm = &KNX_Timer[idx];

        if (tm->state == State::RUNNING) {
            if (tm->base == Resolution::MS) {
                if (--tm->expire_counter == (uint32_t)0UL) {
                    tm->state = State::EXPIRED;
                }
            } else if (tm->base == Resolution::SEC) {
                TMR_LOCK_MAIN_TIMER();
                if (SecondChanged == true)  {
                    tm->expire_counter -= (uint32_t)1UL;
                    if (tm->expire_counter == (uint32_t)0UL) {
                        tm->state = State::EXPIRED;
                        if (Tmr_Callbacks[idx] != NULL) {
                            Tmr_Callbacks[idx]();
                        }
                    }
                }
                TMR_UNLOCK_MAIN_TIMER();
            }
        }
    }
#endif
    SecondChanged = false;
}


constexpr Tmr_TickType operator "" _s (unsigned long long int x) noexcept
{
    return static_cast<Tmr_TickType>(x * 1000);
}

#if 0
constexpr Tmr_TickType operator "" _S (unsigned long long int x) noexcept
{
    return static_cast<Tmr_TickType>(x * 1000);
}
#endif

constexpr Tmr_TickType operator "" _ms (unsigned long long int  x) noexcept
{
    return static_cast<Tmr_TickType>(x);
}

#if 0
constexpr Tmr_TickType operator "" _MS (unsigned long long int  x) noexcept
{
    return static_cast<Tmr_TickType>(x);
}
#endif


} // namespace knx

