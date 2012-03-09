/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
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


/*
** Global defines.
*/	
#define TM_TIMER_TLC_CON_TIMEOUT    ((uint8)0)
#define TM_TIMER_TLC_ACK_TIMEOUT    ((uint8)1)

#define TM_NUM_TIMERS               5

#if TM_NUM_TIMERS < 3
    #error "ERROR: Number of timers must be at least two!!!"
#endif


/*
** Global types.
*/	
typedef uint32 TM_TickType, * TM_TickRefType;

typedef enum tagTM_BaseType {
    TM_BASE_MS,
    TM_BASE_SEC
} TM_BaseType;

typedef enum tagTM_StateType {              /* Constants for Timer-State-Machines. */
    TM_STATE_STOPPED,
    TM_STATE_RUNNING,
    TM_STATE_EXPIRED
} TM_StateType;

typedef struct tagTM_TimerType {
    TM_StateType    state;
    TM_BaseType     base;
    TM_TickType     expire_counter;
} TM_TimerType;


/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)	    KnxTMR_Init(void);

FUNC(boolean, KSTACK_CODE)  KnxTMR_Start(uint8 timer, TM_BaseType base, TM_TickType ticks);
FUNC(boolean, KSTACK_CODE)  KnxTMR_Stop(uint8 timer);

FUNC(boolean, KSTACK_CODE)  KnxTMR_IsExpired(uint8 timer);
FUNC(boolean, KSTACK_CODE)  KnxTMR_IsRunning(uint8 timer);

FUNC(boolean, KSTACK_CODE)  KnxTMR_GetRemainder(uint8 timer, TM_TickRefType remainder);

FUNC(TM_TickType, KSTACK_CODE)TM_TickType KnxTMR_GetSystemTime(TM_BaseType base);

FUNC(void, KSTACK_CODE)	    KnxTMR_Delay(TM_TickType ms);
FUNC(void, KSTACK_CODE)	    KnxTMR_DelayHMS(uint16 H, uint16 M, uint16 S);

FUNC(void, KSTACK_CODE)	    KnxTMR_SystemTimeHandler(void);
FUNC(void, KSTACK_CODE)	    KnxTMR_SecondCallback(void);	
#else
void	KnxTMR_Init(void);

boolean KnxTMR_Start(uint8 timer, TM_BaseType base, TM_TickType ticks);
boolean KnxTMR_Stop(uint8 timer);

boolean KnxTMR_IsExpired(uint8 timer);
boolean KnxTMR_IsRunning(uint8 timer);

boolean KnxTMR_GetRemainder(uint8 timer, TM_TickRefType remainder);

TM_TickType KnxTMR_GetSystemTime(TM_BaseType base);

void    KnxTMR_Delay(TM_TickType ms);
void    KnxTMR_DelayHMS(uint16 H, uint16 M, uint16 S);

void    KnxTMR_SystemTimeHandler(void);
void    KnxTMR_SecondCallback(void);
#endif /* KSTACK_MEMORY_MAPPING */


#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TIMER_H */

