/*
 *   KONNEX/EIB-Protocol-Stack.
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
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#if !defined(__KNX_LSM_H)
#define __KNX_LSM_H

#include "k-ps/config.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

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
typedef uint8_t KNX_LSCType;

/*
** Global variables.
*/
extern KNX_LSCType KNX_SystemLSC[];

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)    LSM_Init(void);
FUNC(void, KSTACK_CODE)    LSM_Dispatch(
    P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) record,
    /*LSM_State*/ P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) ls_var
    );
FUNC(boolean, KSTACK_CODE) LSM_IsAppLoaded(void);
FUNC(boolean, KSTACK_CODE) LSM_IsGrATLoaded(void);
FUNC(boolean, KSTACK_CODE) LSM_IsGrOATLoaded(void);
FUNC(void, KSTACK_CODE)    LSM_Test(void);
#else
void    LSM_Init(void);
void    LSM_Dispatch(uint8_t * record, /*LSM_State*/ uint8_t * ls_var);
boolean LSM_IsAppLoaded(void);
boolean LSM_IsGrATLoaded(void);
boolean LSM_IsGrOATLoaded(void);
void    LSM_Test(void);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_LSM_H */

