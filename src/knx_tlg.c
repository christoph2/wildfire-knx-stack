/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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
#include "knx_tlg.h"

/*
** Local function prototypes.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, KSTACK_CODE) Disp_T_DataGroupReq(void), Disp_T_PollDataReq(void), Disp_N_PollDataCon(void);
STATIC  FUNC(void, KSTACK_CODE) Disp_N_DataGroupInd(void), Disp_N_DataGroupCon(void);


#else
STATIC void Disp_T_DataGroupReq(void), Disp_T_PollDataReq(void), Disp_N_PollDataCon(void);
STATIC void Disp_N_DataGroupInd(void), Disp_N_DataGroupCon(void);


#endif /* KSTACK_MEMORY_MAPPING */

/*
** Local constants.
*/
STATIC const Knx_LayerServiceFunctionType TLG_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      N_DATA_GROUP_IND        */ Disp_N_DataGroupInd,
/*      N_DATA_GROUP_CON        */ Disp_N_DataGroupCon,
/*      N_POLL_DATA_CON         */ Disp_N_PollDataCon,
/*      T_DATA_GROUP_REQ        */ Disp_T_DataGroupReq,
/*      T_POLL_DATA_REQ         */ Disp_T_PollDataReq,
/*      ====================================================*/
};

STATIC const Knx_LayerServicesType TLG_ServiceTable[] = {
    {KNX_TLG_SERVICES, 5, TLG_Services}
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
FUNC(void, KSTACK_CODE) KnxTLG_Task(void)
#else
void KnxTLG_Task(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxDisp_DispatchLayer(TASK_TL_ID, TLG_ServiceTable);
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
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataGroupInd(void)
#else
STATIC void Disp_N_DataGroupInd(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ScratchBufferPtr->service = T_DATA_GROUP_IND;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataGroupCon(void)
#else
STATIC void Disp_N_DataGroupCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ScratchBufferPtr->service = T_DATA_GROUP_CON;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_PollDataCon(void)
#else
STATIC void Disp_N_PollDataCon(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ScratchBufferPtr->service = T_POLL_DATA_CON;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


/*
**  Services from Application-Layer.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_DataGroupReq(void)
#else
STATIC void Disp_T_DataGroupReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UDT);
    KnxMSG_ScratchBufferPtr->service = N_DATA_GROUP_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_T_PollDataReq(void)
#else
STATIC void Disp_T_PollDataReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
