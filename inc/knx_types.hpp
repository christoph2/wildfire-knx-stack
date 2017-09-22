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
#if !defined(__KNX_TYPES_HPP)
#define __KNX_TYPES_HPP

#include <cstdint>

namespace knx {

/*
 *
 * Basic Types.
 *
 */
using float32 = float;
using float64 = double;

#define STD_ON      (1)
#define STD_OFF     (0)

/*
** Global types.
*/
using Knx_AddressType = uint16_t;
using Knx_SerialNumberType = uint8_t[6];

enum class Knx_StatusType {
    KNX_E_OK,
    KNX_E_NOT_OK
};

struct Knx_PollGroupSettingType {
    uint16_t group_addr;
    uint8_t slot_no;
};

struct Knx_CommObjDescriptorType {
    uint8_t dataPtr;
    uint8_t config;
    uint8_t type;
};

enum class Knx_DafType : uint8_t {
    KNX_ADDR_INDIVIDUAL = 0x00,
    KNX_ADDR_MULTICAST  = 0x80
};

enum class Knx_FrameType : uint8_t {
    KNX_FRAME_EXTENDED  = 0x00,
    KNX_FRAME_STANDARD  = 0x80,
    KNX_FRAME_POLLING   = 0xC0
};

enum class Knx_ObjectType : uint8_t {
    KNX_OT_UINT1,
    KNX_OT_UINT2,
    KNX_OT_UINT3,
    KNX_OT_UINT4,
    KNX_OT_UINT5,
    KNX_OT_UINT6,
    KNX_OT_UINT7,
    KNX_OT_UINT8,
    KNX_OT_UINT16,
    KNX_OT_BYTE3,
    KNX_OT_FLOAT,
    KNX_OT_DATA6,
    KNX_OT_DOUBLE,
    KNX_OT_DATA10,
    KNX_OT_MAXDATA,
    KNX_OT_VARDATA
};

using Knx_PriorityType = uint8_t;
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
constexpr uint8_t KNX_LL_SERVICES     = 0x10;
constexpr uint8_t KNX_NL_SERVICES     = 0x20;

constexpr uint8_t KNX_TLG_SERVICES    = 0x30;
constexpr uint8_t KNX_TLC_SERVICES    = 0x40;

constexpr uint8_t KNX_ALG_SERVICES    = 0x70;
constexpr uint8_t KNX_ALM_SERVICES    = 0x80;

/*
** Global types.
*/
enum class Knx_ServiceType {
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

};

/*
**  START: LAYER-Dispatching-Functions.
*/
using Knx_LayerServiceFunctionType  = void (*)();

struct Knx_ServiceEntryType {
    Knx_ServiceType Service;
    Knx_LayerServiceFunctionType Function;
};

struct Knx_LayerServicesType {
    uint8_t LayerOffset;
    uint8_t NumServices;
    const Knx_LayerServiceFunctionType * Functions;
};
/*
**  END: LAYER-Dispatching-Functions.
*/

using Knx_HopCountType = uint8_t;

/*
**  Global Types.
*/
enum class KnxModule_StateType {
    KNX_MODULE_UNINIT,
    KNX_MODULE_READY
};

} // namespace knx

#endif  /* __KNX_TYPES_HPP */

