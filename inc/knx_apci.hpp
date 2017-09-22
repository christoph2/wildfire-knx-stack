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
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#if !defined(__KNX_APCI_HPP)
#define __KNX_APCI_HPP

namespace knx {

/*
**  APCI types.
*/
enum class Knx_APCITypeType {
    APCI_GROUP_VALUE_READ,          /* Multicast.               */
    APCI_GROUP_VALUE_RESP,          /* "       ".               */
    APCI_GROUP_VALUE_WRITE,         /* "       ".               */
    /*                                                          */
    APCI_INDIVIDUAL_ADDRESS_WRITE,  /* Broadcast.               */
    APCI_INDIVIDUAL_ADDRESS_READ,   /* "       ".               */
    APCI_INDIVIDUAL_ADDRESS_RESP,   /* "       ".               */
    /*                                                          */
    APCI_ADC_READ,                  /* P2P-Connection-Oriented. */
    APCI_ADC_RESP,                  /* "                     ". */
    APCI_MEMORY_READ,               /* "                     ". */
    APCI_MEMORY_RESP,               /* "                     ". */
    APCI_MEMORY_WRITE,              /* "                     ". */
    /*                                                          */
    APCI_USER_MSG,                  /* User-defined Messages.   */
    /*                                                          */
    APCI_DEVICE_DESCRIPTOR_READ,    /* P2P-Conection-Less.      */
    APCI_DEVICE_DESCRIPTOR_RESP,    /* "                ".      */
    /*                                                          */
    APCI_RESTART,                   /* P2P-Connection-Oriented. */
        /*                                                      */
    APCI_ESCAPE                     /* Others, escape.          */

};

/*
**  Group services.
*/
constexpr uint16_t A_GROUPVALUE_READ                       = 0x0000;
constexpr uint16_t A_GROUPVALUE_RESPONSE                   = 0x0040;
constexpr uint16_t A_GROUPVALUE_WRITE                      = 0x0080;

/*
**  Broadcast.
*/
constexpr uint16_t A_PHYSICALADDRESS_WRITE                 = 0x00C0;    /* Phys.AddrSet                 */
constexpr uint16_t A_PHYSICALADDRESS_READ                  = 0x0100;    /* Phys.AddrRead                */
constexpr uint16_t A_PHYSICALADDRESS_RESPONSE              = 0x0140;    /* Phys.AddrResponse            */

/*
**  Point-to-Point.
*/
constexpr uint16_t A_ADC_READ                              = 0x0180;    /* ADCRead                      */
constexpr uint16_t A_ADC_RESPONSE                          = 0x01C0;    /* ADCResponse                  */
constexpr uint16_t A_MEMORY_READ                           = 0x0200;    /* MemoryRead                   */
constexpr uint16_t A_MEMORY_RESPONSE                       = 0x0240;    /* MemoryResponse               */
constexpr uint16_t A_MEMORY_WRITE                          = 0x0280;    /* MemoryWrite                  */

constexpr uint16_t A_USERMEMORY_READ                       = 0x02C0;    /* UserDataRead                 */
constexpr uint16_t A_USERMEMORY_RESPONSE                   = 0x02C1;    /* UserDataResponse             */
constexpr uint16_t A_USERMEMORY_WRITE                      = 0x02C2;    /* UserDataWrite                */
constexpr uint16_t A_USERMEMORYBIT_WRITE                   = 0x02C4;    /* UserDataBitWrite             */
constexpr uint16_t A_USERMANUFACTURERINFO_READ             = 0x02C5;    /* UserMgmtTypeRead             */
constexpr uint16_t A_USERMANUFACTURERINFO_RESPONSE         = 0x02C6;    /* UserMgmtTypeResponse         */

/*
** Reserved for USERMSGs: 0x2C7 - 2F7 (48 Codes).
** Reserved for manufacturer specific msgs: 0x2F8 - 2FE (6 Codes).
*/

constexpr uint16_t A_DEVICEDESCRIPTOR_READ                 = 0x0300;    /* MaskVersionRead              */
constexpr uint16_t A_DEVICEDESCRIPTOR_RESPONSE             = 0x0340;    /* MaskVersionResponse          */
constexpr uint16_t A_RESTART                               = 0x0380;    /* Restart                      */

/*
**  Coupler-/Router-specific.
*/
constexpr uint16_t A_OPEN_ROUTING_TABLE_REQ                = 0x03C0;    /* LcTabMemEnable               */
constexpr uint16_t A_READ_ROUTING_TABLE_REQ                = 0x03C1;    /* LcTabMemRead                 */
constexpr uint16_t A_READ_ROUTING_TABLE_RES                = 0x03C2;    /* LcTabMemResponse             */
constexpr uint16_t A_WRITE_ROUTING_TABLE_REQ               = 0x03C3;    /* LcTabMemWrite                */
constexpr uint16_t A_READ_ROUTER_MEMORY_REQ                = 0x03C8;    /* LcSlaveRead                  */
constexpr uint16_t A_READ_ROUTER_MEMORY_RES                = 0x03C9;    /* LcSlaveResponse              */
constexpr uint16_t A_WRITE_ROUTER_MEMORY_REQ               = 0x03CA;    /* LcSlaveWrite                 */
constexpr uint16_t A_READ_ROUTER_STATUS_REQ                = 0x03CD;    /* LcGroupRead                  */
constexpr uint16_t A_READ_ROUTER_STATUS_RES                = 0x03CE;    /* LcGroupResponse              */
constexpr uint16_t A_WRITE_ROUTER_STATUS_REQ               = 0x03CF;    /* LcGroupWrite                 */

constexpr uint16_t A_MEMORYBIT_WRITE                       = 0x03D0;    /* BitWrite                     */
constexpr uint16_t A_AUTHORIZE_REQUEST                     = 0x03D1;    /* AuthorizeRequest             */
constexpr uint16_t A_AUTHORIZE_RESPONSE                    = 0x03D2;    /* AuthorizeResponse            */
constexpr uint16_t A_KEY_WRITE                             = 0x03D3;    /* SetKeyRequest                */
constexpr uint16_t A_KEY_RESPONSE                          = 0x03D4;    /* SetKeyResponse               */

constexpr uint16_t A_PROPERTYVALUE_READ                    = 0x03D5;    /* PropertyRead                 */
constexpr uint16_t A_PROPERTYVALUE_RESPONSE                = 0x03D6;    /* PropertyResponse             */
constexpr uint16_t A_PROPERTYVALUE_WRITE                   = 0x03D7;    /* PropertyWrite                */
constexpr uint16_t A_PROPERTYDESCRIPTION_READ              = 0x03D8;    /* PropertyDescriptionRead      */
constexpr uint16_t A_PROPERTYDESCRIPTION_RESPONSE          = 0x03D9;    /* PropertyDescriptionResponse  */

/*
** Broadcast.
**
*/
constexpr uint16_t A_PHYSICALADDRESSSERIALNUMBER_READ      = 0x03DC;    /* PhysAddrSerNoRead            */
constexpr uint16_t A_PHYSICALADDRESSSERIALNUMBER_RESPONSE  = 0x03DD;    /* PhysAddrSerNoResponse        */
constexpr uint16_t A_PHYSICALADDRESSSERIALNUMBER_WRITE     = 0x03DE;    /* PhysAddrSerNoWrite           */
constexpr uint16_t A_SERVICEINFORMATION_INDICATION_WRITE   = 0x03DF;    /* ServiceInfo                  */

constexpr uint16_t A_DOMAINADDRESS_WRITE                   = 0x03E0;    /* SysIdWrite                   */
constexpr uint16_t A_DOMAINADDRESS_READ                    = 0x03E1;    /* SysIdRead                    */
constexpr uint16_t A_DOMAINADDRESS_RESPONSE                = 0x03E2;    /* SysIdResponse                */
constexpr uint16_t A_DOMAINADDRESSSELECTIVE_READ           = 0x03E3;    /* SysIdSelectiveRead           */

constexpr uint16_t A_NETWORKPARAMETER_WRITE                = 0x03E4;    /* A_NetworkParameter_Write     */
constexpr uint16_t A_NETWORKPARAMETER_READ                 = 0x03DA;    /* (s. Supplement S09, S08)     */
constexpr uint16_t A_NETWORKPARAMETER_RESPONSE             = 0x03DB;    /* (s. Supplement S03)          */

/*
**  Point to Point.
*/
constexpr uint16_t A_LINK_READ                             = 0x03E5;
constexpr uint16_t A_LINK_RESPONSE                         = 0x03E6;
constexpr uint16_t A_LINK_WRITE                            = 0x03E7;

/*
**  Multicast.
*/
constexpr uint16_t A_GROUPPROPVALUE_READ                   = 0x03E8;    /* s. KNX 10_01 LTE             */
constexpr uint16_t A_GROUPPROPVALUE_RESPONSE               = 0x03E9;
constexpr uint16_t A_GROUPPROPVALUE_WRITE                  = 0x03EA;
constexpr uint16_t A_GROUPPROPVALUE_INFOREPORT             = 0x03EB;

} // namespace knx

#endif  /* __KNX_APCI_HPP */

