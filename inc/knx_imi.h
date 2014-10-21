/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
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
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#if !defined(__KNX_IMI_H)
#define __KNX_IMI_H

#include "knx_types.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
**
**  Service Codes for Konnex/EIB Internal-Message-Interface.
**
**  Own coding of services permits usage of jump-tables.
**
*/

/*
** Global defines.
*/
#define KNX_LL_SERVICES     ((uint8_t)0x10)
#define KNX_NL_SERVICES     ((uint8_t)0x20)

#define KNX_TLG_SERVICES    ((uint8_t)0x30)
#define KNX_TLC_SERVICES    ((uint8_t)0x40)

#define KNX_ALG_SERVICES    ((uint8_t)0x70)
#define KNX_ALM_SERVICES    ((uint8_t)0x80)

/*
** Global types.
*/
typedef enum tagKnx_ServiceTypeType {
/*
**      Link-Layer.
*/

    KNX_SERVICE_L_DATA_REQ = KNX_LL_SERVICES,
    KNX_SERVICE_L_POLL_DATA_REQ,

/*
**      Network-Layer.
*/
    KNX_SERVICE_L_DATA_IND = KNX_NL_SERVICES,
    KNX_SERVICE_L_DATA_CON,
    KNX_SERVICE_L_POLL_DATA_CON,
    KNX_SERVICE_L_BUSMON_IND,
    KNX_SERVICE_N_DATA_INDIVIDUAL_REQ,
    KNX_SERVICE_N_DATA_GROUP_REQ,
    KNX_SERVICE_N_DATA_BROADCAST_REQ,
    KNX_SERVICE_N_POLL_DATA_REQ,

/*
**      Transport-Layer/group-oriented.
*/
    KNX_SERVICE_N_DATA_GROUP_IND = KNX_TLG_SERVICES,
    KNX_SERVICE_N_DATA_GROUP_CON,
    KNX_SERVICE_N_POLL_DATA_CON,
    KNX_SERVICE_T_DATA_GROUP_REQ,
    KNX_SERVICE_T_POLL_DATA_REQ,

/*
**      Transport-Layer/connection-oriented.
*/
    KNX_SERVICE_N_DATA_INDIVIDUAL_IND = KNX_TLC_SERVICES,
    KNX_SERVICE_N_DATA_INDIVIDUAL_CON,
    KNX_SERVICE_N_DATA_BROADCAST_IND,
    KNX_SERVICE_N_DATA_BROADCAST_CON,
    KNX_SERVICE_T_CONNECT_REQ,
    KNX_SERVICE_T_DISCONNECT_REQ,
    KNX_SERVICE_T_DATA_CONNECTED_REQ,
    KNX_SERVICE_T_DATA_INDIVIDUAL_REQ,
    KNX_SERVICE_T_DATA_BROADCAST_REQ,

/*
**      Application-Layer/group-oriented.
*/
    KNX_SERVICE_T_DATA_GROUP_IND = KNX_ALG_SERVICES,
    KNX_SERVICE_T_DATA_GROUP_CON,
    KNX_SERVICE_T_POLL_DATA_CON,
    KNX_SERVICE_A_DATA_GROUP_REQ,
    KNX_SERVICE_A_POLL_DATA_REQ,

/*
**      Application-Layer/managment-part.
*/
    KNX_SERVICE_T_CONNECT_IND = KNX_ALM_SERVICES,
    KNX_SERVICE_T_CONNECT_CON,
    KNX_SERVICE_T_DISCONNECT_IND,
    KNX_SERVICE_T_DISCONNECT_CON,
    KNX_SERVICE_T_DATA_CONNECTED_IND,
    KNX_SERVICE_T_DATA_CONNECTED_CON,
    KNX_SERVICE_T_DATA_INDIVIDUAL_IND,
    KNX_SERVICE_T_DATA_INDIVIDUAL_CON,
    KNX_SERVICE_T_DATA_BROADCAST_IND,
    KNX_SERVICE_T_DATA_BROADCAST_CON

} Knx_ServiceTypeType;

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_IMI_H */

