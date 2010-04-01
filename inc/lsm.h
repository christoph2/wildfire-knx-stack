/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
#if !defined(__LSM_H)
#define __LSM_H

#include "KNXDefs.h"

#define KNX_NUM_SYS_LSCS    ((uint8)3)

#define KNX_LSC_APP         ((uint8)0)
#define KNX_LSC_GRAT        ((uint8)1)
#define KNX_LSC_GROAT       ((uint8)2)

typedef uint8 KNX_LSCType;

extern KNX_LSCType KNX_SystemLSC[];

void LSM_Init(void);
void LSM_Dispatch(uint8 *record,/*LSM_State*/uint8 *ls_var);
boolean LSM_IsAppLoaded(void);
boolean LSM_IsGrATLoaded(void);
boolean LSM_IsGrOATLoaded(void);

void LSM_Test(void);

#endif  /* __LSM_H */

