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
#include "knx_rsm.h"

/*
   State           Value   Remark
   =====           =====   ======
   Halted          0x00    the program is halted
   Running         0x01    the program is running
   Ready           0x02    the program is ready, but not yet running
   Terminate       0x03    the program is terminated
 */

/*
   Event   Value   Remark
   ====    =====   ======
   Ready   0x00    no operation
   Restart 0x01    request to restart the program
   Stop    0x02    request to stop the program
 */

/*
   // Format (reading run control).

   #define KNX_RSM_STATE_HALTED        0   // the program is halted.
   #define KNX_RSM_STATE_RUNNING       1   // the program is running.
   #define KNX_RSM_STATE_READY         2   // the program is ready, but not yet running.
   #define KNX_RSM_STATE_TERMINATED    3   // the program is terminated.

   // Format (writing run control).

   #define KNX_RSM_EVENT_READY     0   // no operation.
   #define KNX_RSM_EVENT_RESTART   1   // request to restart the program.
   #define KNX_RSM_EVENT_STOP      2   // request to stop the program.
 */

typedef enum tagRSM_State {
    RSM_STATE_HALTED,
    RSM_STATE_RUNNING,
    RSM_STATE_READY,
    RSM_STATE_TERMINATED,
    /* STATE_ERROR ??? */
    RSM_STATE_LAST
} RSM_State;

typedef enum tagRSM_Event {
    RSM_EVENT_READY,
    RSM_EVENT_RESTART,
    RSM_EVENT_STOP,
    RSM_EVENT_LAST
} RSM_Event;

