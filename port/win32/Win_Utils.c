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

#include "win\Win_Utils.h"
#include <malloc.h>
#include <stdio.h>

/**
 *  @file Win_Utils.c
 *  @brief Collection of unsorted functions related to Windows.
 *
 */

#if !defined(STATUS_POSSIBLE_DEADLOCK)
    #define STATUS_POSSIBLE_DEADLOCK (0xC0000194)
#endif

static volatile BOOL exitApplication = FALSE;

void Win_Error(char * function)
{
    char * szBuf;
    LPWSTR  lpMsgBuf;
    WORD len, idx;
    DWORD err = GetLastError();

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );

    len = lstrlen(lpMsgBuf) + 1;
    szBuf = (char *)_alloca((len * sizeof(lpMsgBuf[0])));
    for (idx = 0; idx < len; ++idx) {
        szBuf[idx] = lpMsgBuf[idx];
    }
//_tprintf(szBuf, "%s failed with error %d: %s",  lpszFunction, err, szBuf);
    printf("%s failed with error %d: %s", function, err, szBuf);

    _freea(szBuf);
    LocalFree(lpMsgBuf);
}


boolean Port_InitializeCriticalSection(CRITICAL_SECTION * criticalSection)
{    
    if (!InitializeCriticalSectionEx(criticalSection, 4000UL, RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN))  {
        Win_Error("InitializeCriticalSectionAndSpinCount");
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
    __try {
        EnterCriticalSection(criticalSection);
    } __except (GetExceptionCode() == EXCEPTION_POSSIBLE_DEADLOCK ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
        printf("POSSIBLE_DEADLOCK exception.\nTerminating.\n");
        ExitProcess(1);
    }
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
    } else {
        return TRUE;
    }
    //return criticalSection.LockCount > 0UL;
}


BOOL WINAPI ConsoleCtrlHandler(DWORD signal)
{
    switch (signal) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
        exitApplication = TRUE;
        return TRUE;
    default:
        /* Pass signal on to the next handler. */
        return FALSE;
    }
    return TRUE;
}

boolean Port_InstallExitHandler(void)
{
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleCtrlHandler, TRUE)) {
        Win_Error("SetConsoleCtrlHandler");
        return FALSE;
    }
}

boolean Port_ExitRequest(void)
{
    return exitApplication;
}

