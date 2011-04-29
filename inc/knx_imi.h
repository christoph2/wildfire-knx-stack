/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__KNX_IMI_H)
#define __KNX_IMI_H

/* knx_imi.h */

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
**
**  Service Codes for Konnex/EIB Internal-Message-Interface.
**
**  Own coding of Services permits usage of Jump-Tables.
**
*/

#define KNX_LL_SERVICES     ((uint8)0x10)
#define KNX_NL_SERVICES     ((uint8)0x20)

#define KNX_TLG_SERVICES    ((uint8)0x30)
#define KNX_TLC_SERVICES    ((uint8)0x40)

#define KNX_ALG_SERVICES    ((uint8)0x70)
#define KNX_ALM_SERVICES    ((uint8)0x80)

typedef enum tagKnx_ServiceTypeType {
/*
**      Link-Layer.
*/
    L_DATA_REQ =   KNX_LL_SERVICES,
    L_POLL_DATA_REQ,

/*
**      Network-Layer.
*/
    L_DATA_IND =   KNX_NL_SERVICES,
    L_DATA_CON,
    L_POLL_DATA_CON,
    L_BUSMON_IND,
    N_DATA_INDIVIDUAL_REQ,
    N_DATA_GROUP_REQ,
    N_DATA_BROADCAST_REQ,
    N_POLL_DATA_REQ,

/*
**      Transport-Layer/group-oriented.
*/
    N_DATA_GROUP_IND =   KNX_TLG_SERVICES,
    N_DATA_GROUP_CON,
    N_POLL_DATA_CON,
    T_DATA_GROUP_REQ,
    T_POLL_DATA_REQ,

/*
**      Transport-Layer/connection-oriented.
*/
    N_DATA_INDIVIDUAL_IND =   KNX_TLC_SERVICES,
    N_DATA_INDIVIDUAL_CON,
    N_DATA_BROADCAST_IND,
    N_DATA_BROADCAST_CON,
    T_CONNECT_REQ,
    T_DISCONNECT_REQ,
    T_DATA_CONNECTED_REQ,
    T_DATA_INDIVIDUAL_REQ,
    T_DATA_BROADCAST_REQ,

/*
**      Application-Layer/group-oriented.
*/
    T_DATA_GROUP_IND =   KNX_ALG_SERVICES,
    T_DATA_GROUP_CON,
    T_POLL_DATA_CON,
    A_DATA_GROUP_REQ,
    A_POLL_DATA_REQ,

/*
**      Application-Layer/managment-part.
*/
    T_CONNECT_IND =   KNX_ALM_SERVICES,
    T_CONNECT_CON,
    T_DISCONNECT_IND,
    T_DISCONNECT_CON,
    T_DATA_CONNECTED_IND,
    T_DATA_CONNECTED_CON,
    T_DATA_INDIVIDUAL_IND,
    T_DATA_INDIVIDUAL_CON,
    T_DATA_BROADCAST_IND,
    T_DATA_BROADCAST_CON

} Knx_ServiceTypeType;

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_IMI_H */
