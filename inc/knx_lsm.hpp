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
#if !defined(__KNX_LSM_HPP)
#define __KNX_LSM_HPP

#include "Wildfire_Config.hpp"

namespace knx {

/*
** Global defines.
*/
#define KNX_LSC_APP         ((uint8_t)0)
#define KNX_LSC_GRAT        ((uint8_t)1)
#define KNX_LSC_GROAT       ((uint8_t)2)

#define KNX_NUM_SYS_LSCS    ((uint8_t)3)

/*
** Global types.
*/
using KNX_LSCType = uint8_t;

/*
** Global variables.
*/
extern KNX_LSCType KNX_SystemLSC[];

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) LSM_Init();
FUNC(void, KSTACK_CODE) LSM_Dispatch(
    P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) record,
    /*LSM_State*/ P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) ls_var
    );
FUNC(bool, KSTACK_CODE) LSM_IsAppLoaded();
FUNC(bool, KSTACK_CODE) LSM_IsGrATLoaded();
FUNC(bool, KSTACK_CODE) LSM_IsGrOATLoaded();
FUNC(void, KSTACK_CODE) LSM_Test();
#else
void LSM_Init();
void LSM_Dispatch(uint8_t * record, /*LSM_State*/ uint8_t * ls_var);
bool LSM_IsAppLoaded();
bool LSM_IsGrATLoaded();
bool LSM_IsGrOATLoaded();
void LSM_Test();


#endif /* KSTACK_MEMORY_MAPPING */

} // namespace knx

#endif  /* __KNX_LSM_HPP */

