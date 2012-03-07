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

#include "knx_sched.h"
#include "knx_layer_link.h"
#include "knx_layer_network.h"
#include "knx_tlg.h"
#include "knx_layer_application.h"

static boolean  KnxSched_PreLinkLayerTest(void);
static boolean  KnxSched_PostLinkLayerTest(void);
static boolean  KnxSched_PreUserApplicationTest(void);


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

void KnxSched_Init(void)
{
    /*
       Initialisation on power on:
       ===========================
       Link-Layer, Management, Hardware...

       1. check if each system MCB is established in EEPROM
        on error set bit #6 in RunError.

       2. for each owner(addr, asso, new user, old user) {

       }
     */
    DEV_Init();
    KnxLL_Init();
    LSM_Init();
    KnxNL_Init();
    KnxTLC_Init();
    KnxMSG_Init();
    KnxALG_Init();
    KnxTMR_Init();
}


void KnxSched_Task(void)
{
    if (KnxSched_PreLinkLayerTest()) {

    }

    KnxLL_Task();

    if (KnxSched_PostLinkLayerTest()) {

    }

    KnxNL_Task();
    KnxTLG_Task();
    KnxTLC_Task();
    KnxALG_Task();
    KnxALM_Task();

    if (KnxSched_PreUserApplicationTest()) {

    }

    KnxALG_PollCycle();

    KnxALM_Task();
    KnxALG_Task();
    KnxTLC_Task();
    KnxTLG_Task();
    KnxNL_Task();

    if (KnxSched_PreLinkLayerTest()) {

    }

    KnxLL_Task();

    if (KnxSched_PostLinkLayerTest()) {

    }
}


static boolean KnxSched_PreLinkLayerTest(void)
{
/*
    check stack overflow
    check parity of system state (60h)
    check program button (handle programming / normal mode)
    trigger watchdog
    trigger timer-system (cyclic refresh of system timer)
 */
}


static boolean KnxSched_PostLinkLayerTest(void)
{
/*
TODO: check if system-state is changed (busmonitor mode).
 */
}


static boolean KnxSched_PreUserApplicationTest(void)
{
/*
TODO: check entry conditions for application
 */
}

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
