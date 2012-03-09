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
#if !defined(__TPUART_H)
#define __TPUART_H

#include "knx_disp.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


/*
** Global defines.
*/
#define BUF_LEN ((SizeType)0x1f)

/*
**    7 6 5 4 3  2  1 0
**    ===================================
**    1 0 r 1 c1 c0 0 0       L_DATA.ind
**    0 0 r 1 c1 c0 0 0       L_LONG_DATA.ind
**    1 1 1 1 0  0  0 0       L_Poll_Data.ind
*/

#define DATA_SERVICE_MASK       ((uint8)0x10)
#define L_DATA_IND_MASK         ((uint8)0x90)
#define L_LONG_DATA_IND_MASK    ((uint8)0x10)
#define L_POLL_DATA_IND_MASK    ((uint8)0xf0)
#define PRIO_MASK               ((uint8)0x0C)
#define REPEATED_MASK           ((uint8)0x20)

/*
**      Immediate Acknowledge Services
**      =========================================
**      1 1 0 0 1 1 0 0         Acknowledge frame
**      0 0 0 0 1 1 0 0         NotAcknowledge frame
**      1 1 0 0 0 0 0 0         Busy fram
*/

#define IACK_IND                ((uint8)0xCC)
#define INACK_IND               ((uint8)0x0C)
#define IBUSY_IND               ((uint8)0xC0)

/*
**      EIBUART-Control-Services
**      =================================================
**      0 0 0 0 0 0 1 1         Reset-Indication
**      x x x x x 1 1 1         State-Response/Indication
**      x 0 0 0 1 0 1 1         L_DATA.confirm x = 1 Positive
*/

#define RESET_IND               ((uint8)0x03)


/*
** Global types.
*/
typedef enum tagTPUART_RCV_STATE {
    TPSR_INIT,
    TPSR_WAIT_RESET_IND,
    TPSR_WAIT_IACK,
    TPSR_WAIT,
    TPSR_DATA_CONT1,
    TPSR_DATA_CONT2,
/*      TSR_DATA_END, */
    TPSR_LONG_DATA,
    TPSR_LONG_DATA_CONT1,
    TPSR_LONG_DATA_CONT2,
/*      TSR_LONG_DATA_END, */
    TPSR_POLL_DATA,
    TPSR_POLL_DATA_CONT,
/*      TSR_POLL_DATA_END, */
    TPSR_IACK_SRV,
    TPSR_CTRL_SRV
} TPUART_RCV_STATE;

typedef enum tagTPUART_RCV_SERVICE {
    SERVICE_NONE, SERVICE_DATA, SERVICE_IACK, SERVICE_CONTROL
} TPUART_RCV_SERVICE;


/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) TPTest(void);
FUNC(void, KSTACK_CODE) KnxLL_Init(void);
FUNC(void, KSTACK_CODE) KnxLL_Task(void);
#else
void    TPTest(void);
void    KnxLL_Init(void);
void    KnxLL_Task(void);
#endif /* KSTACK_MEMORY_MAPPING */



#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __TPUART_H */

