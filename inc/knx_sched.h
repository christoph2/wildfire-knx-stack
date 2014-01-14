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
#if !defined(__KNX_SCHED_H)
#define __KNX_SCHED_H

#include "knx_messaging.h"
#include "knx_disp.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


/*
** Global defines.
*/
#define KNX_DEFAULT_HOP_COUNT ((uint8)6)


/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)	KnxSched_Task(void);
FUNC(void, KSTACK_CODE)	KnxSched_Init(void);
#else	
void KnxSched_Task(void);
void KnxSched_Init(void);

void KnxUser_Main(void);    /* CHECK: Separate module? */
void KnxUser_Init(void);
void KnxUser_Save(void);

#endif /* KSTACK_MEMORY_MAPPING */


#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /*__KNX_SCHED_H*/

