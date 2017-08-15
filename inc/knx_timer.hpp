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

namespace knx {

#include "Wildfire_Config.hpp"
#include "knx_types.hpp"
#include "knx_macros.hpp"

/*
** Global defines.
*/
#define TMR_TIMER_LLC_TIMEOUT       ((uint8_t)0)
#define TMR_TIMER_TLC_CON_TIMEOUT   ((uint8_t)1)
#define TMR_TIMER_TLC_ACK_TIMEOUT   ((uint8_t)2)
#define TMR_TIMER_USER0             ((uint8_t)3)


#if TMR_NUM_TIMERS < 4
    #error "ERROR: Number of timers must be at least three!"
#endif

/*
** Global types.
*/
using Tmr_TickType = uint32_t;

using KnxTmr_CallbackFunctionType = void (*)(void);
using KnxTmr_TickHandlerType = void (*)(void);

enum class Tmr_ResolutionType {
    TMR_RESOLUTION_MS,
    TMR_RESOLUTION_SEC
};

enum class Tmr_StateType {              /* Constants for Timer-State-Machines. */
    TMR_STATE_STOPPED,
    TMR_STATE_RUNNING,
    TMR_STATE_EXPIRED
};

struct Tmr_TimerType {
    Tmr_StateType state;
    Tmr_ResolutionType base;
    Tmr_TickType expire_counter;
};

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTmr_Init(void);

FUNC(bool, KSTACK_CODE) KnxTmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks);
FUNC(bool, KSTACK_CODE) KnxTmr_Stop(uint8_t timer);

FUNC(bool, KSTACK_CODE) KnxTmr_IsExpired(uint8_t timer);
FUNC(bool, KSTACK_CODE) KnxTmr_IsRunning(uint8_t timer);

FUNC(bool, KSTACK_CODE) KnxTmr_GetRemainder(uint8_t timer, Tmr_TickType & remainder);

FUNC(Tmr_TickType, KSTACK_CODE) Tmr_TickType KnxTmr_GetSystemTime(Tmr_ResolutionType base);


FUNC(void, KSTACK_CODE) KnxTmr_Delay(Tmr_TickType ms);
FUNC(void, KSTACK_CODE) KnxTmr_DelayHMS(uint16_t H, uint16_t M, uint16_t S);

FUNC(void, KSTACK_CODE) KnxTmr_SystemTickHandler(void);
FUNC(void, KSTACK_CODE) KnxTmr_SecondCallback(void);


#else
void KnxTmr_Init(void);

bool KnxTmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks);
bool KnxTmr_Stop(uint8_t timer);

bool KnxTmr_IsExpired(uint8_t timer);
bool KnxTmr_IsRunning(uint8_t timer);

bool KnxTmr_GetRemainder(uint8_t timer, Tmr_TickType & remainder);

Tmr_TickType KnxTmr_GetSystemTime(Tmr_ResolutionType base);

bool KnxTmr_DataLinkTimerIsRunning(void);
void KnxTmr_DataLinkTimerStart(void);
void KnxTmr_DataLinkTimerStop(void);

void KnxTmr_Delay(Tmr_TickType ms);
void KnxTmr_DelayHMS(uint16_t H, uint16_t M, uint16_t S);

void KnxTmr_SystemTickHandler(void);
void KnxTmr_SecondCallback(void);

/*
** Global Variables.
*/
extern KnxTmr_CallbackFunctionType KnxTmr_Callbacks[TMR_NUM_TIMERS];


#endif /* KSTACK_MEMORY_MAPPING */

} // namespace knx

#endif  /* __KNX_TIMER_HPP */

