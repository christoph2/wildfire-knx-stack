/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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

#include <Std_Types.h>

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

void TimerTest(void);

#define TM_TIMER_TLC_CON_TIMEOUT    ((uint8)0)
#define TM_TIMER_TLC_ACK_TIMEOUT    ((uint8)1)

#define TM_NUM_TIMERS               5

#if TM_NUM_TIMERS < 3
    #error "ERROR: Number of Timers must be at least two!!!"
#endif

typedef uint32 TM_TickType, * TM_TickRefType;

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
    TM_StateType    state;
    TM_BaseType     base;
    TM_TickType     expire_counter;
} TM_TimerType;

void TM_Init(void);

boolean TM_Start(uint8 timer, TM_BaseType base, TM_TickType ticks);
boolean TM_Stop(uint8 timer);

boolean TM_IsExpired(uint8 timer);
boolean TM_IsRunning(uint8 timer);

boolean TM_GetRemainder(uint8 timer, TM_TickRefType remainder);

TM_TickType TM_GetSystemTime(TM_BaseType base);

void    TM_Delay(TM_TickType ms);
void    TM_DelayHMS(uint16 H, uint16 M, uint16 S);

void    TM_SystemTimeHandler(void);
void    TM_SecondCallback(void);

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TIMER_H */

