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

/*
** cEMI
*/
#if !defined(__CEMI_HPP)
#define __CEMI_HPP

#include "target.hpp"

namespace knx {

constexpr uint8_t L_BUSMON_IND        = 0x2B;
constexpr uint8_t L_RAW_IND           = 0x2D;
constexpr uint8_t L_RAW_REQ           = 0x10;
constexpr uint8_t L_RAW_CON           = 0x2F;
constexpr uint8_t L_DATA_REQ          = 0x11;
constexpr uint8_t L_DATA_CON          = 0x2E;
constexpr uint8_t L_DATA_IND          = 0x29;
constexpr uint8_t L_POLL_DATA_REQ     = 0x13;
constexpr uint8_t L_POLL_DATA_CON     = 0x25;
constexpr uint8_t M_PROP_READ_REQ     = 0xFC;
constexpr uint8_t M_PROP_READ_CON     = 0xFB;
constexpr uint8_t M_PROP_WRITE_REQ    = 0xF6;
constexpr uint8_t M_PROP_WRITE_CON    = 0xF5;
constexpr uint8_t M_PROP_INFO_IND     = 0xF7;
constexpr uint8_t M_RESET_REQ         = 0xF1;
constexpr uint8_t M_RESET_IND         = 0xF0;

} // namespace knx

#endif  /* __CEMI_HPP  */

