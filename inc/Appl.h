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
#if !defined(__APPL_H)
#define __APPL_H

#include "knx_address.h"
#include "knx_layer_network.h"
#include "knx_ios.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

#define APP_NUM_OF_COM_OBJS     ((uint8_t)3)
/* todo: NUM_ASSOCS */

#define APP_OBJECT_DATA_SIZE    ((uint8_t)8)

extern const uint8_t KNX_DEV_FIRMWARE_REV;

extern const uint8_t DEV_EEPROM_HEADER[0x16];

extern const Knx_SerialNumberType   KNX_DEV_SERIAL_NUM;
extern const uint8_t                  KNX_DEV_ORDER_INFO[10];

extern const Knx_PollGroupSettingType   KNX_DEV_POLLGROUP_SETTINGS;
extern const uint32                     KNX_DEV_ACCESS_KEYTABLE[3];

extern uint8_t const * const  __LOG_EEPROM_START;
extern uint8_t const * const  __PHYS_EEPROM_START;

extern const uint8_t                          Knx_UserInterfaceObjCount;
extern const Knx_InterfaceObjectType * *    Knx_UserInterfaceObjPtr;

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) DEV_Init(void);
#else
void DEV_Init(void);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __APPL_H */

