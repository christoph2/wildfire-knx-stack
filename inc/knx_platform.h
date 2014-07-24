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
#if !defined(__KNX_PLATFORM_H)
#define __KNX_PLATFORM_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

#include "k-ps/config.h"
#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#include "win\Win_Timer.h"
#include "Port_Serial.h"
#else

#endif

#define TMR_LOCK_MAIN_TIMER()       Port_TimerLockMainTimer()
#define TMR_UNLOCK_MAIN_TIMER()     Port_TimerUnlockMainTimer()
#define TMR_LOCK_DL_TIMER()         Port_TimerLockDLTimer()
#define TMR_UNLOCK_DL_TIMER()       Port_TimerUnlockDLTimer()

#define TMR_START_DL_TIMER()        Port_StartDLTimer()
#define TMR_STOP_DL_TIMER()         Port_StopDLTimer()

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_PLATFORM_H */

