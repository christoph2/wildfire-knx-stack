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
#include "knx_tlg.h"

/*
** Local function prototypes.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, KSTACK_CODE) Disp_T_DataGroup_Req(void), Disp_T_PollData_Req(void), Disp_N_PollData_Con(void);
STATIC  FUNC(void, KSTACK_CODE) Disp_N_DataGroup_Ind(void), Disp_N_DataGroup_Con(void);


#else
STATIC void Disp_T_DataGroup_Req(void), Disp_T_PollData_Req(void), Disp_N_PollData_Con(void);
STATIC void Disp_N_DataGroup_Ind(void), Disp_N_DataGroup_Con(void);


#endif /* KSTACK_MEMORY_MAPPING */

/*
** Local constants.
*/
STATIC const Knx_LayerServiceFunctionType KnxTlg_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      N_DATA_GROUP_IND        */ Disp_N_DataGroup_Ind,
/*      N_DATA_GROUP_CON        */ Disp_N_DataGroup_Con,
/*      N_POLL_DATA_CON         */ Disp_N_PollData_Con,
/*      T_DATA_GROUP_REQ        */ Disp_T_DataGroup_Req,
/*      T_POLL_DATA_REQ         */ Disp_T_PollData_Req,
/*      ====================================================*/
};

STATIC const Knx_LayerServicesType KnxTlg_ServiceTable[] = {
    { KNX_TLG_SERVICES, SIZEOF_ARRAY(KnxTlg_Services), KnxTlg_Services }
};

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

/*
**
** Global functions.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlg_Task(void)
#else
void KnxTlg_Task(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxDisp_DispatchLayer(TASK_TL_ID, KnxTlg_ServiceTable);
}


/*
**
** Local functions.
**
*/

/*
**  Services from Network-Layer.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataGroup_Ind(void)
#else
STATIC void Disp_N_DataGroup_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_T_DATA_GROUP_IND;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataGroup_Con(void)
#else
STATIC void Disp_N_DataGroup_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_T_DATA_GROUP_CON;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_PollData_Con(void)
#else
STATIC void Disp_N_PollData_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_T_POLL_DATA_CON;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


/*
**  Services from Application-Layer.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_DataGroup_Req(void)
#else
STATIC void Disp_T_DataGroup_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetTPCI(KnxMsg_ScratchBufferPtr, TPCI_UDT);
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_N_DATA_GROUP_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_PollData_Req(void)
#else
STATIC void Disp_T_PollData_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
