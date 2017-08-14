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
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#if !defined(__KNX_LAYER_TRANSPORT_HPP)
#define __KNX_LAYER_TRANSPORT_HPP

#include "knx_address.hpp"
#include "knx_messaging.hpp"
#include "StateMachineKNX.hpp"
#include "knx_timer.hpp"
#include "knx_disp.hpp"

namespace knx {

/*
**  TPCI-Codings.
*/
#define KNX_TPCI_UDT                    ((uint8_t)0x00)       /* Unnumbered Data. */
#define KNX_TPCI_NDT                    ((uint8_t)0x40)       /* Numbered Data (T_DATA_CONNECTED_REQ_PDU). */
#define KNX_TPCI_UCD                    ((uint8_t)0x80)       /* Unnumbered Control. */
#define KNX_TPCI_NCD                    ((uint8_t)0xC0)       /* Numbered Control (TACK/TNACK). */

#define KNX_TPCI_ACK_PDU                ((uint8_t)0xC2)
#define KNX_TPCI_NAK_PDU                ((uint8_t)0xC3)

#define KNX_TPCI_CONNECT_REQ_PDU        ((uint8_t)0x80)
#define KNX_TPCI_DISCONNECT_REQ_PDU     ((uint8_t)0x81)

#define KNX_TPCI_DATA_TAG_GROUP_PDU     ((uint8_t)0x04)    /* Interface-Objects using Group-Addressing (LTE-HEE). */

} // namespace knx

#endif  /* __KNX_LAYER_TRANSPORT_HPP */
