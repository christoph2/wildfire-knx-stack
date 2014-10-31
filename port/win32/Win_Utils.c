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
#include "win\Win_Utils.h"
#include <malloc.h>
#include <stdio.h>

/**
 *  @file Win_Utils.c
 *  @brief Collection of unsorted functions related to Windows.
 *
 */

STATIC volatile BOOL exitApplication = FALSE;

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
        szBuf[idx] = (char)lpMsgBuf[idx];
    }
//_tprintf(szBuf, "%s failed with error %d: %s",  lpszFunction, err, szBuf);
    printf("%s failed with error %ld: %s", function, err, szBuf);

    _freea(szBuf);
    LocalFree(lpMsgBuf);
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
    return TRUE;
}

boolean Port_ExitRequest(void)
{
    return exitApplication;
}

