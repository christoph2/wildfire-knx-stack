
/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
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


#include <Windows.h>

static LARGE_INTEGER timerFrequency;
static boolean hrcAvailable;

typedef struct tagHrt_TimerType {
    long long start;
    long long stop;
    BOOL running;
} Hrt_TimerType;


void Hrt_Init(void)
{
    if (QueryPerformanceFrequency(&timerFrequency) == 0) {
        hrcAvailable = FALSE;
    }
    else {
        hrcAvailable = TRUE;
    }
}

boolean Hrt_IsHRTAvailable(void)
{
    return hrcAvailable;
}

void Hrt_TimerInit(Hrt_TimerType * timerContext)
{
    if (!hrcAvailable) {
        return;
    }
    timerContext->running = FALSE;
}

void Hrt_TimerStart(Hrt_TimerType * timerContext)
{
    LARGE_INTEGER timerValue;
    if (!hrcAvailable || timerContext->running) {
        return;
    }
    timerContext->running = TRUE;
    QueryPerformanceCounter(&timerValue);
    timerContext->start = timerValue.QuadPart;
}

void Hrt_TimerStop(Hrt_TimerType * timerContext)
{
    LARGE_INTEGER timerValue;
    if (!hrcAvailable || !timerContext->running) {
        return;
    }
    QueryPerformanceCounter(&timerValue);
    timerContext->stop = timerValue.QuadPart;
}

long long Hrt_TimerElapsedTime(Hrt_TimerType const * timerContext)
{
    if (!hrcAvailable || !timerContext->running) {
        return (long long)0LL;
    }
    return (timerContext->stop - timerContext->start) / (timerFrequency.QuadPart / 1000);
}

