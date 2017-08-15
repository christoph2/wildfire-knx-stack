/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
*                                       cpu12.gems@googlemail.com>
*
*   All Rights Reserved
*
*  This program is free softwKNXe; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free SoftwKNXe Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#if !defined(__APPL_HPP)
#define __APPL_HPP

#include "knx_address.hpp"
#include "knx_nl.hpp"
#include "knx_ios.hpp"

namespace knx {

#define APP_NUM_OF_COM_OBJS     ((uint8_t)3)
/* todo: NUM_ASSOCS */

#define APP_OBJECT_DATA_SIZE    ((uint8_t)8)

extern const uint8_t KNX_DEV_FIRMWARE_REV;

extern const uint8_t DEV_EEPROM_HEADER[0x16];

extern const Knx_SerialNumberType   KNX_DEV_SERIAL_NUM;
extern const uint8_t                  KNX_DEV_ORDER_INFO[10];

extern const Knx_PollGroupSettingType   KNX_DEV_POLLGROUP_SETTINGS;
extern const uint32_t                     KNX_DEV_ACCESS_KEYTABLE[3];

extern uint8_t const * const  __LOG_EEPROM_START;
extern uint8_t const * const  __PHYS_EEPROM_START;

extern const uint8_t                          Knx_UserInterfaceObjCount;
extern const Knx_InterfaceObjectType * *    Knx_UserInterfaceObjPtr;

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) DEV_Init();
#else
void DEV_Init();


#endif /* KSTACK_MEMORY_MAPPING */

} // namespace knx

#endif  /* __APPL_HPP */

