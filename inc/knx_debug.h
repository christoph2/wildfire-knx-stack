
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
#if !defined(__KNX_DEBUG_H)
#define __KNX_DEBUG_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

#include "k-ps/config.h"

#if defined(_WIN32) || defined(_WIN64)

typedef struct tagDbg_TimerType {
    __int64 start;
    __int64 stop;
    boolean running;
} Dbg_TimerType;

void Dbg_DumpHex(uint8_t * frame, uint16_t length);

#define DBG_PRINT(msg)      printf("%s", (msg))
#define DBG_PRINTLN(msg)    printf("%s\n", (msg))

void Dbg_Init(void);
boolean Dbg_IsHRTAvailable(void);
void Dbg_TimerInit(Dbg_TimerType * timerContext);
void Dbg_TimerStart(Dbg_TimerType * timerContext);
void Dbg_TimerStop(Dbg_TimerType * timerContext);
__int64 Dbg_TimerElapsedTime(Dbg_TimerType const * timerContext);

#else
#define Dbg_DumpHex(frame, length)
#define DBG_PRINT(msg)
#define DBG_PRINTLN(msg)

#define Dbg_Init()
#define Dbg_IsHRTAvailable()    (FALSE)
#define Dbg_TimerInit(timerContext)
#define Dbg_TimerStart(timerContext)
#define Dbg_TimerStop(timerContext)
#define Dbg_TimerElapsedTime(timerContext)
#endif


#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_DEBUG_H */
