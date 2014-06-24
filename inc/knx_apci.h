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
#if !defined(__KNX_APCI_H)
#define __KNX_APCI_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
**  APCI types.
*/
typedef enum tagKNX_APCITypeType {
    apciGROUP_VALUE_READ,           /* Multicast.                   */
    apciGROUP_VALUE_RESP,           /* "       ".                   */
    apciGROUP_VALUE_WRITE,          /* "       ".                   */
    /*                                                              */
    apciINDIVIDUAL_ADDRESS_WRITE,   /* Broadcast.                   */
    apciINDIVIDUAL_ADDRESS_READ,    /* "       ".                   */
    apciINDIVIDUAL_ADDRESS_RESP,    /* "       ".                   */
    /*                                                              */
    apciADC_READ,                   /* P2P-Connection-Oriented.     */
    apciADC_RESP,                   /* "                     ".     */
    apciMEMORY_READ,                /* "                     ".     */
    apciMEMORY_RESP,                /* "                     ".     */
    apciMEMORY_WRITE,               /* "                     ".     */
    /*                                                              */
    apciUSER_MSG,                   /* User-defined Messages.       */
    /*                                                              */
    apciDEVICE_DESCRIPTOR_READ,     /* P2P-Conection-Less.          */
    apciDEVICE_DESCRIPTOR_RESP,     /* "                ".          */
    /*                                                              */
    apciRESTART,                    /* P2P-Connection-Oriented.     */
    /*                                                              */
    apciESCAPE                      /* Others, escape.              */

} KNX_APCITypeType;

/*
**  Group services.
*/
#define A_GROUPVALUE_READ                       ((uint16_t)0x0000)
#define A_GROUPVALUE_RESPONSE                   ((uint16_t)0x0040)
#define A_GROUPVALUE_WRITE                      ((uint16_t)0x0080)

/*
**  Broadcast.
*/
#define A_PHYSICALADDRESS_WRITE                 ((uint16_t)0x00C0)    /* Phys.AddrSet                 */
#define A_PHYSICALADDRESS_READ                  ((uint16_t)0x0100)    /* Phys.AddrRead                */
#define A_PHYSICALADDRESS_RESPONSE              ((uint16_t)0x0140)    /* Phys.AddrResponse            */

/*
**  Point-to-Point.
*/
#define A_ADC_READ                              ((uint16_t)0x0180)    /* ADCRead                      */
#define A_ADC_RESPONSE                          ((uint16_t)0x01C0)    /* ADCResponse                  */
#define A_MEMORY_READ                           ((uint16_t)0x0200)    /* MemoryRead                   */
#define A_MEMORY_RESPONSE                       ((uint16_t)0x0240)    /* MemoryResponse               */
#define A_MEMORY_WRITE                          ((uint16_t)0x0280)    /* MemoryWrite                  */

#define A_USERMEMORY_READ                       ((uint16_t)0x02C0)    /* UserDataRead                 */
#define A_USERMEMORY_RESPONSE                   ((uint16_t)0x02C1)    /* UserDataResponse             */
#define A_USERMEMORY_WRITE                      ((uint16_t)0x02C2)    /* UserDataWrite                */
#define A_USERMEMORYBIT_WRITE                   ((uint16_t)0x02C4)    /* UserDataBitWrite             */
#define A_USERMANUFACTURERINFO_READ             ((uint16_t)0x02C5)    /* UserMgmtTypeRead             */
#define A_USERMANUFACTURERINFO_RESPONSE         ((uint16_t)0x02C6)    /* UserMgmtTypeResponse         */

/*
** Reserved for USERMSGs: 0x2C7 - 2F7 (48 Codes).
** Reserved for manufacturer specific msgs: 0x2F8 - 2FE (6 Codes).
*/

#define A_DEVICEDESCRIPTOR_READ                 ((uint16_t)0x0300)    /* MaskVersionRead              */
#define A_DEVICEDESCRIPTOR_RESPONSE             ((uint16_t)0x0340)    /* MaskVersionResponse          */
#define A_RESTART                               ((uint16_t)0x0380)    /* Restart                      */

/*
**  Coupler-/Router-specific.
*/
#define A_OPEN_ROUTING_TABLE_REQ                ((uint16_t)0x03C0)    /* LcTabMemEnable               */
#define A_READ_ROUTING_TABLE_REQ                ((uint16_t)0x03C1)    /* LcTabMemRead                 */
#define A_READ_ROUTING_TABLE_RES                ((uint16_t)0x03C2)    /* LcTabMemResponse             */
#define A_WRITE_ROUTING_TABLE_REQ               ((uint16_t)0x03C3)    /* LcTabMemWrite                */
#define A_READ_ROUTER_MEMORY_REQ                ((uint16_t)0x03C8)    /* LcSlaveRead                  */
#define A_READ_ROUTER_MEMORY_RES                ((uint16_t)0x03C9)    /* LcSlaveResponse              */
#define A_WRITE_ROUTER_MEMORY_REQ               ((uint16_t)0x03CA)    /* LcSlaveWrite                 */
#define A_READ_ROUTER_STATUS_REQ                ((uint16_t)0x03CD)    /* LcGroupRead                  */
#define A_READ_ROUTER_STATUS_RES                ((uint16_t)0x03CE)    /* LcGroupResponse              */
#define A_WRITE_ROUTER_STATUS_REQ               ((uint16_t)0x03CF)    /* LcGroupWrite                 */

#define A_MEMORYBIT_WRITE                       ((uint16_t)0x03D0)    /* BitWrite                     */
#define A_AUTHORIZE_REQUEST                     ((uint16_t)0x03D1)    /* AuthorizeRequest             */
#define A_AUTHORIZE_RESPONSE                    ((uint16_t)0x03D2)    /* AuthorizeResponse            */
#define A_KEY_WRITE                             ((uint16_t)0x03D3)    /* SetKeyRequest                */
#define A_KEY_RESPONSE                          ((uint16_t)0x03D4)    /* SetKeyResponse               */

#define A_PROPERTYVALUE_READ                    ((uint16_t)0x03D5)    /* PropertyRead                 */
#define A_PROPERTYVALUE_RESPONSE                ((uint16_t)0x03D6)    /* PropertyResponse             */
#define A_PROPERTYVALUE_WRITE                   ((uint16_t)0x03D7)    /* PropertyWrite                */
#define A_PROPERTYDESCRIPTION_READ              ((uint16_t)0x03D8)    /* PropertyDescriptionRead      */
#define A_PROPERTYDESCRIPTION_RESPONSE          ((uint16_t)0x03D9)    /* PropertyDescriptionResponse  */

/*
** Broadcast.
** Hinweis: 'C'-Identifier dürfen höchstens 31 Zeichen lang sein!!!
*/
#define A_PHYSICALADDRESSSERIALNUMBER_READ      ((uint16_t)0x03DC)    /* PhysAddrSerNoRead            */
#define A_PHYSICALADDRESSSERIALNUMBER_RESPONSE  ((uint16_t)0x03DD)    /* PhysAddrSerNoResponse        */
#define A_PHYSICALADDRESSSERIALNUMBER_WRITE     ((uint16_t)0x03DE)    /* PhysAddrSerNoWrite           */
#define A_SERVICEINFORMATION_INDICATION_WRITE   ((uint16_t)0x03DF)    /* ServiceInfo                  */

#define A_DOMAINADDRESS_WRITE                   ((uint16_t)0x03E0)    /* SysIdWrite                   */
#define A_DOMAINADDRESS_READ                    ((uint16_t)0x03E1)    /* SysIdRead                    */
#define A_DOMAINADDRESS_RESPONSE                ((uint16_t)0x03E2)    /* SysIdResponse                */
#define A_DOMAINADDRESSSELECTIVE_READ           ((uint16_t)0x03E3)    /* SysIdSelectiveRead           */

#define A_NETWORKPARAMETER_WRITE                ((uint16_t)0x03E4)    /* A_NetworkParameter_Write     */
#define A_NETWORKPARAMETER_READ                 ((uint16_t)0x03DA)    /* (s. Supplement S09, S08)     */
#define A_NETWORKPARAMETER_RESPONSE             ((uint16_t)0x03DB)    /* (s. Supplement S03)          */

/*
**  Point to Point.
*/
#define A_LINK_READ                             ((uint16_t)0x03E5)
#define A_LINK_RESPONSE                         ((uint16_t)0x03E6)
#define A_LINK_WRITE                            ((uint16_t)0x03E7)

/*
**  Multicast.
*/
#define A_GROUPPROPVALUE_READ                   ((uint16_t)0x03E8)    /* s. KNX 10_01 LTE             */
#define A_GROUPPROPVALUE_RESPONSE               ((uint16_t)0x03E9)
#define A_GROUPPROPVALUE_WRITE                  ((uint16_t)0x03EA)
#define A_GROUPPROPVALUE_INFOREPORT             ((uint16_t)0x03EB)

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_APCI_H */

