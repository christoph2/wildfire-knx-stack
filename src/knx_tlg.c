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
#include "knx_tlg.h"

static void Disp_T_DataGroupReq(void), Disp_T_PollDataReq(void), Disp_N_PollDataCon(void);
static void Disp_N_DataGroupInd(void), Disp_N_DataGroupCon(void);


static const Knx_LayerServiceFunctionType TLG_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      N_DATA_GROUP_IND        */ Disp_N_DataGroupInd,
/*      N_DATA_GROUP_CON        */ Disp_N_DataGroupCon,
/*      N_POLL_DATA_CON         */ Disp_N_PollDataCon,
/*      T_DATA_GROUP_REQ        */ Disp_T_DataGroupReq,
/*      T_POLL_DATA_REQ         */ Disp_T_PollDataReq,
/*      ====================================================*/
};

static const Knx_LayerServicesType TLG_ServiceTable[] = {
    {KNX_TLG_SERVICES, 5, TLG_Services}
};

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

void KnxTLG_Task(void)
{
    KnxDisp_DispatchLayer(TASK_TL_ID, TLG_ServiceTable);
}


/*
**
**  Services from Network-Layer.
**
*/
static void Disp_N_DataGroupInd(void)
{
    KnxMSG_ScratchBufferPtr->service = T_DATA_GROUP_IND;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


static void Disp_N_DataGroupCon(void)
{
    KnxMSG_ScratchBufferPtr->service = T_DATA_GROUP_CON;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


static void Disp_N_PollDataCon(void)
{
    KnxMSG_ScratchBufferPtr->service = T_POLL_DATA_CON;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


/*
**
**  Services from Application-Layer.
**
*/
static void Disp_T_DataGroupReq(void)
{
    KnxMSG_SetTPCI(KnxMSG_ScratchBufferPtr, TPCI_UDT);
    KnxMSG_ScratchBufferPtr->service = N_DATA_GROUP_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


static void Disp_T_PollDataReq(void)
{
    /* todo: Implement!!! */
}

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
