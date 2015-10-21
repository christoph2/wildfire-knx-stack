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
#if !defined(__WIN_TIMER_H)
#define __WIN_TIMER_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

#include "Wildfire_Config.h"
#include <stdint.h>

void Port_TimerInit(void);
void Port_TimerDeinit(void);

void Port_TimerLockMainTimer(void);
void Port_TimerUnlockMainTimer(void);
void Port_TimerLockDLTimer(void);
void Port_TimerUnlockDLTimer(void);

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __WIN_TIMER_H */
