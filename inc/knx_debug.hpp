/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__KNX_DEBUG_HPP)
#define __KNX_DEBUG_HPP

namespace knx {

#include "Wildfire_Config.hpp"


#if (KNX_BUILD_TYPE == KNX_BUILD_DEBUG) && (defined(_MSC_VER) || defined(WIN32) || defined(WIN64) || \
                                            defined(__CYGWIN32__) || defined(__CYGWIN64__) || defined(__GNUC__))

#include "knx_types.hpp"

void KnxEt_DumpHex(uint8_t const * frame, uint16_t length);

#define DBG_PRINT(msg)      printf("%s", (msg))
#define DBG_PRINTLN(msg)    printf("%s\n", (msg))

void Dbg_Init(void);

#else
#define KnxEt_DumpHex(frame, length)
#define DBG_PRINT(msg)
#define DBG_PRINTLN(msg)

#define Dbg_Init()
#define Dbg_TraceFunctionEntry(ModuleId, ApiId)
#define Dbg_TraceFunctionExit(ModuleId, ApiId)

#endif

} // namespace knx

#endif  /* __KNX_DEBUG_HPP */

