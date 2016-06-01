/*
 *   KONNEX/EIB-Protocol-Stack.
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

#if !defined(__WILDFIRE_CONFIG_H)
#define __WILDFIRE_CONFIG_H

#include "port/port_sync.h"

#define DISABLE_ALL_INTERRUPTS()
#define ENABLE_ALL_INTERRUPTS()

#define KSTACK_MEMORY_MAPPING       STD_OFF
//#define KNX_BUILD_TYPE              KNX_RELEASE_BUILD
#define KNX_ENDIANESS               KNX_LITTLE_ENDIAN
#define TMR_NUM_TIMERS              (4)
#define TMR_TICK_RESOLUTION         (10)
#define KNX_TL_STATEMACHINE_STYLE   (1)
#define KNX_BUS_INTERFACE           KNX_BIF_TPUART_1
#define KNX_BUILD_TYPE              KNX_BUILD_DEBUG
#define KNX_TARGET_TYPE             KNX_TARGET_POSIX  //KNX_TARGET_ATMEL_ATMEGA

#define Port_Lock_TaskLevel()
#define Port_Unlock_TaskLevel()
#define Port_TimerLockMainTimer()
#define Port_TimerUnlockMainTimer()
#define Port_TimerLockDLTimer()
#define Port_TimerUnlockDLTimer()

#define TMR_LOCK_MAIN_TIMER()       Port_TimerLockMainTimer()
#define TMR_UNLOCK_MAIN_TIMER()     Port_TimerUnlockMainTimer()
#define TMR_LOCK_DL_TIMER()         Port_TimerLockDLTimer()
#define TMR_UNLOCK_DL_TIMER()       Port_TimerUnlockDLTimer()

#define TMR_START_DL_TIMER()        KnxTmr_DataLinkTimerStart()
#define TMR_STOP_DL_TIMER()         KnxTmr_DataLinkTimerStop()

#define PORT_LOCK_TASK_LEVEL()      Sync_Lock_TaskLevel()
#define PORT_UNLOCK_TASK_LEVEL()    Sync_Unlock_TaskLevel()

#endif /* __WILDFIRE_CONFIG_H */

// Für interne Zwecke (PRE_COMPILE config) benötigen wir 'config_internal.h'.

