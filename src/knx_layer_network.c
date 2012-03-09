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
#include "knx_layer_network.h"


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) NL_CheckRoutingCount(KnxMSG_BufferPtr pBuffer);
#else
void NL_CheckRoutingCount(KnxMSG_BufferPtr pBuffer);
#endif /* KSTACK_MEMORY_MAPPING */


/*
**  Flags im Control-Field:
**  =======================
**  "The L_Data.con message exists in a positive and a negative version. Both versions differ in the
**  value of error flag contained in the control field octet: the positive L_Data.con message has the
**  error flag reset, the negative one set."
**  ERROR-Flag ist Bit #0.
*/

#define CONF_OK     ((uint8)0x00)
#define CONF_ERROR  ((uint8)0x01)

#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataIndividualReq(void), Disp_N_DataGroupReq(void), Disp_N_PollDataReq(void);
STATIC FUNC(void, KSTACK_CODE) Disp_L_PollDataCon(void), Disp_L_DataInd(void), Disp_L_BusmonInd(void);
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataBroadcastReq(void), Disp_L_DataCon(void);
#else
static void Disp_N_DataIndividualReq(void), Disp_N_DataGroupReq(void), Disp_N_PollDataReq(void);
static void Disp_L_PollDataCon(void), Disp_L_DataInd(void), Disp_L_BusmonInd(void);
static void Disp_N_DataBroadcastReq(void), Disp_L_DataCon(void);
#endif /* KSTACK_MEMORY_MAPPING */

static const Knx_LayerServiceFunctionType NL_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      L_DATA_IND              */ Disp_L_DataInd,
/*      L_DATA_CON              */ Disp_L_DataCon,
/*      L_POLL_DATA_CON         */ Disp_L_PollDataCon,
/*      L_BUSMON_IND            */ Disp_L_BusmonInd,
/*      N_DATA_INDIVIDUAL_REQ   */ Disp_N_DataIndividualReq,
/*      N_DATA_GROUP_REQ        */ Disp_N_DataGroupReq,
/*      N_DATA_BROADCAST_REQ    */ Disp_N_DataBroadcastReq,
/*      N_POLL_DATA_REQ         */ Disp_N_PollDataReq
/*      ====================================================*/
};

static const Knx_LayerServicesType NL_ServiceTable[] = {
    {KNX_NL_SERVICES, (uint8)8, NL_Services}
};

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)  KnxNL_Task(void)
#else
void KnxNL_Task(void)
{
    KnxDisp_DispatchLayer(TASK_NL_ID, NL_ServiceTable);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxNL_Init(void)
#else
void KnxNL_Init(void)
{

}


/*
**
**  Services from Link-Layer.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_L_DataInd(void)
#else
STATIC void Disp_L_DataInd(void)
{
    if ((KnxMSG_GetMessagePtr(KnxMSG_ScratchBufferPtr)->ncpi & (uint8)0x80) == (uint8)0x80) {
        if (KnxADR_IsBroadcastAddress(KnxMSG_GetMessagePtr(KnxMSG_ScratchBufferPtr)->dest)) {
            /* Broadcast-Communication. */
            KnxMSG_ScratchBufferPtr->service = N_DATA_BROADCAST_IND;
            (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
        } else {
            /* Multicast-Communication. */
            KnxMSG_ScratchBufferPtr->service = N_DATA_GROUP_IND;
            (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
        }
    } else {
        /* Individual-Adressing. */
        KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_IND;
        (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_L_DataCon(void)
#else
STATIC void Disp_L_DataCon(void)
{
    /* todo: Implement!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_L_BusmonInd(void)
#else
STATIC void Disp_L_BusmonInd(void)
{
    /* todo: Implement!!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_L_PollDataCon(void)
#else
STATIC void Disp_L_PollDataCon(void)
{
    /* todo: Implement!!! */
}


/*
**
**  Services from Transport-Layer.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataBroadcastReq(void)
#else
STATIC void Disp_N_DataBroadcastReq(void)
{
    KnxMSG_SetAddressType(KnxMSG_ScratchBufferPtr, atMULTICAST);
    KnxMSG_SetRoutingCount(KnxMSG_ScratchBufferPtr);
    KnxMSG_ScratchBufferPtr->service = L_DATA_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataIndividualReq(void)
#else
STATIC void Disp_N_DataIndividualReq(void)
{
    KnxMSG_SetAddressType(KnxMSG_ScratchBufferPtr, atINDIVIDUAL);
    KnxMSG_SetRoutingCount(KnxMSG_ScratchBufferPtr);
    KnxMSG_ScratchBufferPtr->service = L_DATA_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_DataGroupReq(void)
#else
STATIC void Disp_N_DataGroupReq(void)
{
    KnxMSG_SetAddressType(KnxMSG_ScratchBufferPtr, atMULTICAST);
    KnxMSG_SetRoutingCount(KnxMSG_ScratchBufferPtr);
    KnxMSG_ScratchBufferPtr->service = L_DATA_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_N_PollDataReq(void)
#else
STATIC void Disp_N_PollDataReq(void)
{
    /* todo: Implement!!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) NL_CheckRoutingCount(KnxMSG_BufferPtr pBuffer)
#else
void NL_CheckRoutingCount(KnxMSG_BufferPtr pBuffer)
{
    if (KnxMSG_GetRoutingCount(pBuffer) == MSG_NO_ROUTING_CTRL) {
        KnxMSG_SetRoutingCtrl(pBuffer, TRUE);
    } else {
        KnxMSG_SetRoutingCtrl(pBuffer, FALSE);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
