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
#if !defined(__KNX_TIMER_H)
#define __KNX_TIMER_H

#include "kdk/common/Std_Types.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
** Global defines.
*/
#define TMR_TIMER_TLC_CON_TIMEOUT    ((uint8)0)
#define TMR_TIMER_TLC_ACK_TIMEOUT    ((uint8)1)

#define TMR_NUM_TIMERS               5  /* TODO: Config! */

#if TMR_NUM_TIMERS < 3
    #error "ERROR: Number of timers must be at least two!!!"
#endif

/*
** Global types.
*/
typedef uint32 Tmr_TickType, * Tmr_TickRefType;

typedef enum tagTmr_ResolutionType {
    TMR_RESOLUTION_MS,
    TMR_RESOLUTION_SEC
} Tmr_ResolutionType;

typedef enum tagTmr_StateType {              /* Constants for Timer-State-Machines. */
    TMR_STATE_STOPPED,
    TMR_STATE_RUNNING,
    TMR_STATE_EXPIRED
} Tmr_StateType;

typedef struct tagTmr_TimerType {
    Tmr_StateType state;
    Tmr_ResolutionType base;
    Tmr_TickType expire_counter;
} Tmr_TimerType;

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)     KnxTmr_Init(void);

FUNC(boolean, KSTACK_CODE)  KnxTmr_Start(uint8 timer, Tmr_ResolutionType base, Tmr_TickType ticks);
FUNC(boolean, KSTACK_CODE)  KnxTmr_Stop(uint8 timer);

FUNC(boolean, KSTACK_CODE)  KnxTmr_IsExpired(uint8 timer);
FUNC(boolean, KSTACK_CODE)  KnxTmr_IsRunning(uint8 timer);

FUNC(boolean, KSTACK_CODE)  KnxTmr_GetRemainder(uint8 timer, Tmr_TickRefType remainder);

FUNC(Tmr_TickType, KSTACK_CODE) Tmr_TickType KnxTmr_GetSystemTime(Tmr_ResolutionType base);


FUNC(void, KSTACK_CODE)     KnxTmr_Delay(Tmr_TickType ms);
FUNC(void, KSTACK_CODE)     KnxTmr_DelayHMS(uint16 H, uint16 M, uint16 S);

FUNC(void, KSTACK_CODE)     KnxTmr_SystemTimeHandler(void);
FUNC(void, KSTACK_CODE)     KnxTmr_SecondCallback(void);
#else
void KnxTmr_Init(void);

boolean KnxTmr_Start(uint8 timer, Tmr_ResolutionType base, Tmr_TickType ticks);
boolean KnxTmr_Stop(uint8 timer);

boolean KnxTmr_IsExpired(uint8 timer);
boolean KnxTmr_IsRunning(uint8 timer);

boolean KnxTmr_GetRemainder(uint8 timer, Tmr_TickRefType remainder);

Tmr_TickType KnxTmr_GetSystemTime(Tmr_ResolutionType base);

void    KnxTmr_Delay(Tmr_TickType ms);
void    KnxTmr_DelayHMS(uint16 H, uint16 M, uint16 S);

void    KnxTmr_SystemTimeHandler(void);
void    KnxTmr_SecondCallback(void);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TIMER_H */
