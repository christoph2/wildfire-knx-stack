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
#if !defined(__KNX_SYS_OBJS_HPP)
#define __KNX_SYS_OBJS_HPP

#include "knx_lsm.hpp"
#include "Appl.hpp"

namespace knx {

/*
** Global variables.
*/
extern const uint8_t Knx_SystemInterfaceObjCount;
extern Knx_InterfaceObjectType const * const Knx_SystemInterfaceObjs[];

} // namespace knx

#endif  /* __KNX_SYS_OBJS_HPP */

