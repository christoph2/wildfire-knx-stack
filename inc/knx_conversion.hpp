/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#if !defined(__KNXCONV_HPP)
#define __KNXCONV_HPP

#include "Wildfire_Config.hpp"
#include "knx_types.hpp"
#include "knx_macros.hpp"


namespace knx {

/*
** Global functions.
*/
uint16_t  LongToDPT9(int32_t value);
uint16_t  FloatToDPT9(float64 value);
float64 DPT9ToFloat(uint16_t value);
int32_t  DPT9ToLong(uint16_t value);
uint16_t  btohs(uint16_t w);


} // namespace knx

#endif  /* __KNXCONV_HPP */

