/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__KNX_LAYER_TRANSPORT_H)
#define __KNX_LAYER_TRANSPORT_H

#include "knx_address.h"
#include "knx_messaging.h"
#include "StateMachineKNX.h"
#include "knx_timer.h"
#include "knx_disp.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
**  TPCI-Codings.
*/
#define TPCI_UDT                ((uint8)0x00)       /* Unnumbered Data. */
#define TPCI_NDT                ((uint8)0x40)       /* Numbered Data (T_DATA_CONNECTED_REQ_PDU). */
#define TPCI_UCD                ((uint8)0x80)       /* Unnumbered Control. */
#define TPCI_NCD                ((uint8)0xC0)       /* Numbered Control (TACK/TNACK). */

#define T_ACK_PDU               ((uint8)0xC2)
#define T_NAK_PDU               ((uint8)0xC3)

#define T_CONNECT_REQ_PDU       ((uint8)0x80)
#define T_DISCONNECT_REQ_PDU    ((uint8)0x81)

#define T_DATA_TAG_GROUP_PDU    ((uint8)0x04)    /* Interface-Objects using Group-Addressing (LTE-HEE). */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_LAYER_TRANSPORT_H */

