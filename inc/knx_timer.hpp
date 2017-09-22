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
#if !defined(__KNX_TIMER_HPP)
#define __KNX_TIMER_HPP

#include "Wildfire_Config.hpp"
#include "knx_types.hpp"
#include "knx_macros.hpp"
#include <functional>

namespace knx {

/*
** Global defines.
*/
constexpr uint8_t TMR_TIMER_LLC_TIMEOUT        = 0;
constexpr uint8_t TMR_TIMER_TLC_CON_TIMEOUT    = 1;
constexpr uint8_t TMR_TIMER_TLC_ACK_TIMEOUT    = 2;
constexpr uint8_t TMR_TIMER_USER0              = 3;


#if TMR_NUM_TIMERS < 4
    #error "ERROR: Number of timers must be at least three!"
#endif

/*
** Global types.
*/
using Tmr_TickType = uint32_t;

using Tmr_CallbackFunctionType = std::function<void()>;
using Tmr_TickHandlerType = std::function<void()>;

enum class Tmr_ResolutionType {
    MS,
    SEC
};

enum class Tmr_StateType {              /* Constants for Timer-State-Machines. */
    STOPPED,
    RUNNING,
    EXPIRED
};

struct Tmr_TimerType {
    Tmr_StateType state;
    Tmr_ResolutionType base;
    Tmr_TickType expire_counter;
};

class Timer {
public:

private:

};

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) Tmr_Init();

FUNC(bool, KSTACK_CODE) Tmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks);
FUNC(bool, KSTACK_CODE) Tmr_Stop(uint8_t timer);

FUNC(bool, KSTACK_CODE) Tmr_IsExpired(uint8_t timer);
FUNC(bool, KSTACK_CODE) Tmr_IsRunning(uint8_t timer);

FUNC(bool, KSTACK_CODE) Tmr_GetRemainder(uint8_t timer, Tmr_TickType& remainder);

FUNC(Tmr_TickType, KSTACK_CODE) Tmr_TickType Tmr_GetSystemTime(Tmr_ResolutionType base);


FUNC(void, KSTACK_CODE) Tmr_Delay(Tmr_TickType ms);
FUNC(void, KSTACK_CODE) Tmr_DelayHMS(uint16_t H, uint16_t M, uint16_t S);

FUNC(void, KSTACK_CODE) Tmr_SystemTickHandler();
FUNC(void, KSTACK_CODE) Tmr_SecondCallback();


#else
void Tmr_Init();

bool Tmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks);
bool Tmr_Stop(uint8_t timer);

bool Tmr_IsExpired(uint8_t timer);
bool Tmr_IsRunning(uint8_t timer);

bool Tmr_GetRemainder(uint8_t timer, Tmr_TickType& remainder);

Tmr_TickType Tmr_GetSystemTime(Tmr_ResolutionType base);

bool Tmr_DataLinkTimerIsRunning();
void Tmr_DataLinkTimerStart();
void Tmr_DataLinkTimerStop();

void Tmr_Delay(Tmr_TickType ms);
void Tmr_DelayHMS(uint16_t H, uint16_t M, uint16_t S);

void Tmr_SystemTickHandler(void);
void Tmr_SecondCallback(void);

/*
** Global Variables.
*/
extern Tmr_CallbackFunctionType Tmr_Callbacks[TMR_NUM_TIMERS];


#endif /* KSTACK_MEMORY_MAPPING */

} // namespace knx

#endif  /* __KNX_TIMER_HPP */

