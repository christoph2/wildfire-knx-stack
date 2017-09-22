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
*  GNU General Public License for more Details.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#if !defined(__KNX_DEFS_HPP)
#define __KNX_DEFS_HPP

#include "knx_types.hpp"
#include "knx_macros.hpp"
#include "knx_utl.hpp"
#include "knx_tracelog.hpp"
#include "knx_moduleids.hpp"

namespace knx {

/*
** Global defines.
*/
constexpr uint8_t IAK_OK                   = 0x00;
constexpr uint8_t IAK_NOT_OK               = 0x01;

/*
   #define MSG_NO_ROUTING_CTRL ((BYTE)7)
   NoRouting,Parameter,Force,
 */

/* RunError-Codes. */
constexpr uint8_t KNX_RUNERROR_SYS3_ERR    = 0x40;   /* internal system error (not for each default MCB [ROM] an. */
                                                     /* MCB [EEPROM] exists with the same start address). */
constexpr uint8_t KNX_RUNERROR_SYS2_ERR    = 0x20;   /* internal system error (stuck/ overheat detection). */
constexpr uint8_t KNX_RUNERROR_OBJ_ERR     = 0x10;   /* RAM flag table error (*PTR not in ZPAGE range). */
constexpr uint8_t KNX_RUNERROR_STK_OVL     = 0x08;   /* stack overflow was detected. */
constexpr uint8_t KNX_RUNERROR_EEPROM_ERR  = 0x04;   /* EEPROM check detected an CRC error. */
constexpr uint8_t KNX_RUNERROR_SYS1_ERR    = 0x02;   /* internal system error (parity bit in SystemState is corrupt). */
constexpr uint8_t KNX_RUNERROR_SYS0_ERR    = 0x01;   /* internal system error (msg buffer offset corrupt). */

/*
** Config-Byte / Object-Descriptor.
*/
constexpr uint8_t KNX_OBJ_UPDATE_ENABLE    = 0x80;  /* GroupValueRead()_Res wird verarbeitet - nur Mask 0020h, 0021h. */
constexpr uint8_t KNX_OBJ_TRANSMIT_ENABLE  = 0x40;  /* Lese- und Schreib-Anforderungen (Requests) vom App.-Layer werden verarbeitet. */
constexpr uint8_t KNX_OBJ_EEPROM_VALUE     = 0x20;  /* Objektwert steht im EEPROM (statt im RAM). */
constexpr uint8_t KNX_OBJ_WRITE_ENABLE     = 0x10;  /* Objektwert kann über den Bus geschrieben werden. */
constexpr uint8_t KNX_OBJ_READ_ENABLE      = 0x08;  /* Objektwert kann über den Bus gelesen werden. */
constexpr uint8_t KNX_OBJ_COMM_ENABLE      = 0x04;  /* "Mainswitch" for Communikation. */

/*
**  Config-Byte / Priorities.
*/
constexpr uint8_t KNX_OBJ_PRIO_SYSTEM      = 0;
constexpr uint8_t KNX_OBJ_PRIO_URGENT      = 2;
constexpr uint8_t KNX_OBJ_PRIO_NORMAL      = 1;
constexpr uint8_t KNX_OBJ_PRIO_LOW         = 3;

/*
    Transmission Priorities
    =======================
    11 = low operational priority
    10 = high operational priority
    01 = alarm priority
    00 = system priority
 */

/*
** Comm-Flags.
*/

constexpr uint8_t KNX_OBJ_UPDATED                  = 0x08;
constexpr uint8_t KNX_OBJ_DATA_REQUEST             = 0x04;

/*
** Transmission-Status
*/
constexpr uint8_t KNX_OBJ_IDLE_OK                  = 0x00;
constexpr uint8_t KNX_OBJ_IDLE_ERROR               = 0x01;
constexpr uint8_t KNX_OBJ_TRANSMITTING             = 0x02;
constexpr uint8_t KNX_OBJ_TRANSMIT_REQ             = 0x03;

constexpr uint8_t BCU20_PRIVILEGE_CONFIGURATION    = 0;
constexpr uint8_t BCU20_PRIVILEGE_SERVICE          = 1;
constexpr uint8_t BCU20_PRIVILEGE_USER             = 2;
constexpr uint8_t BCU20_PRIVILEGE_NO               = 3;

constexpr uint8_t KNX_UNUSED_TSAP                  = 0xfe;
constexpr uint8_t KNX_INVALID_TSAP                 = 0x00;


/**
 * Supported Bus Interfaces.
 */
#define KNX_BIF_TPUART_1                  (0)
#define KNX_BIF_TPUART_2                  (1)
#define KNX_BIF_NCN5120                   (2)


/** Power supply of controller module.
 *
 */
#define KNX_MODULE_POWER_BY_BUS               (0)
#define KNX_MODULE_POWER_BY_AUXILIARY_SUPPLY  (1)

/** Endianess
 *
 */
#define KNX_LITTLE_ENDIAN                     (0)
#define KNX_BIG_ENDIAN                        (1)


/** Build types.
 *
 */
#define KNX_BUILD_DEBUG                       (0)
#define KNX_BUILD_RELEASE                     (1)

/** Target Types.
 *
 */
#define KNX_TARGET_WINDOWS                    (1)
#define KNX_TARGET_POSIX                      (2)
#define KNX_TARGET_S12                        (3)
#define KNX_TARGET_ATMEL_ATSAM4               (4)
#define KNX_TARGET_ATMEL_ATMEGA               (5)

/** Stack Types.
 *
 */
#define KNX_LINK_LAYER_ONLY                   (1)
#define KNX_FULL_STACK                        (2)

} // namespace knx

#endif  /* __KNX_DEFS_HPP */

