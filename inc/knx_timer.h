/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
*                                       cpu12.gems@googlemail.com>
*
*   All Rights Reserved
*
*  This program is free softwKNXe; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free SoftwKNXe Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#if !defined(__KNX_TIMER_H)
#define __KNX_TIMER_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
** Global defines.
*/
#define TMR_TIMER_LLC_TIMEOUT       ((uint8_t)0)
#define TMR_TIMER_TLC_CON_TIMEOUT   ((uint8_t)1)
#define TMR_TIMER_TLC_ACK_TIMEOUT   ((uint8_t)2)
#define TMR_TIMER_USER0             ((uint8_t)3)

#include "k-ps/config.h"

#if TMR_NUM_TIMERS < 4
    #error "ERROR: Number of timers must be at least three!"
#endif

/*
** Global types.
*/
typedef uint32_t Tmr_TickType, * Tmr_TickRefType;

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

FUNC(boolean, KSTACK_CODE)  KnxTmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks);
FUNC(boolean, KSTACK_CODE)  KnxTmr_Stop(uint8_t timer);

FUNC(boolean, KSTACK_CODE)  KnxTmr_IsExpired(uint8_t timer);
FUNC(boolean, KSTACK_CODE)  KnxTmr_IsRunning(uint8_t timer);

FUNC(boolean, KSTACK_CODE)  KnxTmr_GetRemainder(uint8_t timer, Tmr_TickRefType remainder);

FUNC(Tmr_TickType, KSTACK_CODE) Tmr_TickType KnxTmr_GetSystemTime(Tmr_ResolutionType base);


FUNC(void, KSTACK_CODE)     KnxTmr_Delay(Tmr_TickType ms);
FUNC(void, KSTACK_CODE)     KnxTmr_DelayHMS(uint16_t H, uint16_t M, uint16_t S);

FUNC(void, KSTACK_CODE)     KnxTmr_SystemTickHandler(void);
FUNC(void, KSTACK_CODE)     KnxTmr_SecondCallback(void);
#else
void KnxTmr_Init(void);

boolean KnxTmr_Start(uint8_t timer, Tmr_ResolutionType base, Tmr_TickType ticks);
boolean KnxTmr_Stop(uint8_t timer);

boolean KnxTmr_IsExpired(uint8_t timer);
boolean KnxTmr_IsRunning(uint8_t timer);

boolean KnxTmr_GetRemainder(uint8_t timer, Tmr_TickRefType remainder);

Tmr_TickType KnxTmr_GetSystemTime(Tmr_ResolutionType base);

boolean KnxTmr_DataLinkTimerIsRunning(void);
void KnxTmr_DataLinkTimerStart(void);
void KnxTmr_DataLinkTimerStop(void);

void    KnxTmr_Delay(Tmr_TickType ms);
void    KnxTmr_DelayHMS(uint16_t H, uint16_t M, uint16_t S);

void    KnxTmr_SystemTickHandler(void);
void    KnxTmr_SecondCallback(void);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TIMER_H */
