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
*  but WITHOUT ANY WKNXRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#include "Port.h"
#include "win\Win_Utils.h"
#include <stdio.h>
#include <process.h>

#if !defined(STATUS_POSSIBLE_DEADLOCK)
#define STATUS_POSSIBLE_DEADLOCK (0xC0000194)
#endif

#if !defined(RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN)
#define RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN  0x02000000
#endif

STATIC HANDLE hMainThread;
STATIC CRITICAL_SECTION isrCS;
STATIC uint32_t processID;

void Port_Init(void)
{
    DWORD pid;
    hMainThread = GetCurrentThread();

    Port_SetThreadAffinity(hMainThread, 0x00000001);

    Port_InitializeCriticalSection(&isrCS);
    //Dbg_Init();
    //Port_InstallExitHandler();
    Port_TimerInit();

    processID = GetCurrentProcessId();
}

void Port_SetThreadAffinity(HANDLE thread, DWORD_PTR mask)
{
    DWORD_PTR resultMask;

    if (SetThreadAffinityMask(hMainThread, mask) == 0) {
        KnxEt_Error("Port_SetThreadAffinity");
    }
}

uint32_t Port_GetProcessID(void)
{
    return processID;
}

//#if 0
void Port_Lock_TaskLevel(void)
{
    Port_EnterCriticalSection(&isrCS);
#if 0
    if (SuspendThread(hMainThread) == -1) {
        KnxEt_Error("Port_Lock_TaskLevel");
    }
#endif
}

void Port_Unlock_TaskLevel(void)
{
    Port_LeaveCriticalSection(&isrCS);
#if 0
    ResumeThread(hMainThread);
#endif
}

boolean Port_InitializeCriticalSection(CRITICAL_SECTION * criticalSection)
{
    if (!InitializeCriticalSectionEx(criticalSection, 4000UL, RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN))  {
        KnxEt_Error("InitializeCriticalSectionAndSpinCount");
        return FALSE;
    }
    return TRUE;
}


void Port_DeleteCriticalSection(CRITICAL_SECTION * criticalSection)
{
    DeleteCriticalSection(criticalSection);
}

static unsigned critCounter = 0;

void Port_EnterCriticalSection(CRITICAL_SECTION * criticalSection)
{
#if defined(_MSC_VER)
    __try {
        EnterCriticalSection(criticalSection);
    }
    __except (GetExceptionCode() == EXCEPTION_POSSIBLE_DEADLOCK ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
        printf("POSSIBLE_DEADLOCK exception.\nTerminating.\n");
        ExitProcess(1);
    }
#else
    EnterCriticalSection(criticalSection);
#endif
}


void Port_LeaveCriticalSection(CRITICAL_SECTION * criticalSection)
{
    LeaveCriticalSection(criticalSection);
}


boolean Port_InCriticalSection(CRITICAL_SECTION * criticalSection)
{
    if (TryEnterCriticalSection(criticalSection)) {
        LeaveCriticalSection(criticalSection);
        return FALSE;
    }
    else {
        return TRUE;
    }
    //return criticalSection.LockCount > 0UL;
}
