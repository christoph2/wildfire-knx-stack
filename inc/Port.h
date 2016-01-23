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
#if !defined(__PORT_H)
#define __PORT_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

//#include "knx_defs.h"
#include "knx_platform.h"
#if _WIN32_WINNT <= 0x0601
#define _WIN32_WINNT 0x0601
#endif
#include <windows.h>

void Port_Init(void);
void Port_SetThreadAffinity(HANDLE thread, DWORD_PTR mask);
uint32_t Port_GetProcessID(void);
boolean Port_InitializeCriticalSection(CRITICAL_SECTION * criticalSection);
void Port_DeleteCriticalSection(CRITICAL_SECTION * criticalSection);
void Port_EnterCriticalSection(CRITICAL_SECTION * criticalSection);
void Port_LeaveCriticalSection(CRITICAL_SECTION * criticalSection);
boolean Port_InCriticalSection(CRITICAL_SECTION * criticalSection);
boolean Port_InstallExitHandler();

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __PORT_H */
