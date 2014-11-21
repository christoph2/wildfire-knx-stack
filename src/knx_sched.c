/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
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
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#include "knx_sched.h"
#include "knx_layer_link.h"
#include "knx_nl.h"
#include "knx_tlg.h"
#include "knx_layer_application.h"
#include "Port.h"


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, KSTACK_CODE)  KnxSched_PreLinkLayerTest(void);
STATIC FUNC(boolean, KSTACK_CODE)  KnxSched_PostLinkLayerTest(void);
STATIC FUNC(boolean, KSTACK_CODE)  KnxSched_PreUserApplicationTest(void);
#else
STATIC boolean  KnxSched_PreLinkLayerTest(void);
STATIC boolean  KnxSched_PostLinkLayerTest(void);
STATIC boolean  KnxSched_PreUserApplicationTest(void);
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxSched_Init(void)
#else
void KnxSched_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
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
    Port_Init();

    //printf("Dev_Init\n");
    DEV_Init();
    //printf("KnxLL_Init\n");
    KnxLL_Init();
    //printf("LSM_Init\n");
    LSM_Init();
    //printf("KnxNL_Init\n");
    KnxNl_Init();
    //printf("KnxTlc_Init\n");
    KnxTlc_Init();
    //printf("KnxMsg_Init\n");
    KnxMsg_Init();
    //printf("KnxALG_Init\n");
    KnxALG_Init();
    //printf("KnxTmr_Init\n");
    KnxTmr_Init();
    //printf("KnxUser_Init\n");
    KnxUser_Init(); /* TODO: Check preconditions. */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxSched_Task(void)
#else
void KnxSched_Task(void)
#endif /* KSTACK_MEMORY_MAPPING */
{    
    PORT_LOCK_TASK_LEVEL();
    if (KnxSched_PreLinkLayerTest()) {

    }

    KnxLL_Task();

    if (KnxSched_PostLinkLayerTest()) {

    }

    KnxNl_Task();
    KnxTlg_Task();
    KnxTlc_Task();
    KnxALG_Task();
    KnxALM_Task();

    if (KnxSched_PreUserApplicationTest()) {
        KnxUser_Main();
    }

    KnxALG_PollCycle(); // TODO: #if POLLING!!!

    KnxALM_Task();
    KnxALG_Task();
    KnxTlc_Task();
    KnxTlg_Task();
    KnxNl_Task();

    if (KnxSched_PreLinkLayerTest()) {

    }

    KnxLL_Task();

    if (KnxSched_PostLinkLayerTest()) {

    }
    PORT_UNLOCK_TASK_LEVEL();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, KSTACK_CODE) KnxSched_PreLinkLayerTest(void)
#else
STATIC boolean KnxSched_PreLinkLayerTest(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
/*
    check stack overflow
    check parity of system state (60h)
    check program button (handle programming / normal mode)
    trigger watchdog
    trigger timer-system (cyclic refresh of system timer)
 */
    return TRUE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, KSTACK_CODE) KnxSched_PostLinkLayerTest(void)
#else
STATIC boolean KnxSched_PostLinkLayerTest(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
/*
TODO: check if system-state is changed (busmonitor mode).
 */
    return TRUE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, KSTACK_CODE) KnxSched_PreUserApplicationTest(void)
#else
STATIC boolean KnxSched_PreUserApplicationTest(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
/*
TODO: check entry conditions for application
 */
    return TRUE;
}

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
