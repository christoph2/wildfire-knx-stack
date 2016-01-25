/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__KNX_DEFS_H)
#define __KNX_DEFS_H

#include "knx_types.h"
#include "knx_macros.h"
#include "knx_utl.h"
#include "knx_tracelog.h"
#include "knx_moduleids.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


/*
** Global defines.
*/
#define IAK_OK                  ((uint8_t)0x00)
#define IAK_NOT_OK              ((uint8_t)0x01)

/*
   #define MSG_NO_ROUTING_CTRL ((BYTE)7)
   NoRouting,Parameter,Force,
 */

/* RunError-Codes. */
#define KNX_RUNERROR_SYS3_ERR   ((uint8_t)0x40) /* internal system error (not for each default MCB [ROM] an. */
                                                /* MCB [EEPROM] exists with the same start address). */
#define KNX_RUNERROR_SYS2_ERR   ((uint8_t)0x20) /* internal system error (stuck/ overheat detection). */
#define KNX_RUNERROR_OBJ_ERR    ((uint8_t)0x10) /* RAM flag table error (*PTR not in ZPAGE range). */
#define KNX_RUNERROR_STK_OVL    ((uint8_t)0x08) /* stack overflow was detected. */
#define KNX_RUNERROR_EEPROM_ERR ((uint8_t)0x04) /* EEPROM check detected an CRC error. */
#define KNX_RUNERROR_SYS1_ERR   ((uint8_t)0x02) /* internal system error (parity bit in SystemState is corrupt). */
#define KNX_RUNERROR_SYS0_ERR   ((uint8_t)0x01) /* internal system error (msg buffer offset corrupt). */

/*
** Config-Byte / Object-Descriptor.
*/
#define KNX_OBJ_UPDATE_ENABLE   ((uint8_t)0x80) /* GroupValueRead()_Res wird verarbeitet - nur Mask 0020h, 0021h. */
#define KNX_OBJ_TRANSMIT_ENABLE ((uint8_t)0x40) /* Lese- und Schreib-Anforderungen (Requests) vom App.-Layer werden verarbeitet. */
#define KNX_OBJ_EEPROM_VALUE    ((uint8_t)0x20) /* Objektwert steht im EEPROM (statt im RAM). */
#define KNX_OBJ_WRITE_ENABLE    ((uint8_t)0x10) /* Objektwert kann über den Bus geschrieben werden. */
#define KNX_OBJ_READ_ENABLE     ((uint8_t)0x08) /* Objektwert kann über den Bus gelesen werden. */
#define KNX_OBJ_COMM_ENABLE     ((uint8_t)0x04) /* "Mainswitch" for Communikation. */

/*
**  Config-Byte / Priorities.
*/
#define KNX_OBJ_PRIO_SYSTEM     ((uint8_t)0)
#define KNX_OBJ_PRIO_URGENT     ((uint8_t)2)
#define KNX_OBJ_PRIO_NORMAL     ((uint8_t)1)
#define KNX_OBJ_PRIO_LOW        ((uint8_t)3)

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

#define KNX_OBJ_UPDATED                 ((uint8_t)0x08)
#define KNX_OBJ_DATA_REQUEST            ((uint8_t)0x04)

/*
** Transmission-Status
*/
#define KNX_OBJ_IDLE_OK                 ((uint8_t)0x00)
#define KNX_OBJ_IDLE_ERROR              ((uint8_t)0x01)
#define KNX_OBJ_TRANSMITTING            ((uint8_t)0x02)
#define KNX_OBJ_TRANSMIT_REQ            ((uint8_t)0x03)

#define BCU20_PRIVILEGE_CONFIGURATION   ((uint8_t)0)
#define BCU20_PRIVILEGE_SERVICE         ((uint8_t)1)
#define BCU20_PRIVILEGE_USER            ((uint8_t)2)
#define BCU20_PRIVILEGE_NO              ((uint8_t)3)

#define KNX_UNUSED_TSAP                 ((uint8_t)0xfe)
#define KNX_INVALID_TSAP                ((uint8_t)0x00)


/**
 * Supported Bus Interfaces.
 */
#define KNX_BIF_TPUART_1                        (0)
#define KNX_BIF_TPUART_2                        (1)
#define KNX_BIF_NCN5120                         (2)


/** Power supply of controller module.
 *
 */
#define KNX_MODULE_POWER_BY_BUS                 (0)
#define KNX_MODULE_POWER_BY_AUXILIARY_SUPPLY    (1)

/** Endianess
 *
 */
#define KNX_LITTLE_ENDIAN                       (0)
#define KNX_BIG_ENDIAN                          (1)


/** Build types.
 *
 */
#define KNX_BUILD_DEBUG                         (0)
#define KNX_BUILD_RELEASE                       (1)

/** Target Types.
 *
 */
#define KNX_TARGET_WINDOWS                      (1)
#define KNX_TARGET_POSIX                        (2)
#define KNX_TARGET_S12                          (3)
#define KNX_TARGET_ATMEL_ATSAM4                 (4)


#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_DEFS_H */

