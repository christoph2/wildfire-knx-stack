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
#include "knx_nl.h"
#include "knx_debug.h"

#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) NL_CheckRoutingCount(KnxMSG_BufferPtr pBuffer);
#else
static void KnxNl_CheckRoutingCount(KnxMSG_BufferPtr pBuffer);


#endif /* KSTACK_MEMORY_MAPPING */

/*
**  Flags im Control-Field:
**  =======================
**  "The L_Data.con message exists in a positive and a negative version. Both versions differ in the
**  value of error flag contained in the control field octet: the positive L_Data.con message has the
**  error flag reset, the negative one set."
**  ERROR-Flag ist Bit #0.
*/

#define CONF_OK     ((uint8_t)0x00)
#define CONF_ERROR  ((uint8_t)0x01)

#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, KSTACK_CODE) N_DataIndividual_Req(void), N_DataGroup_Req(void), N_PollData_Req(void);
STATIC  FUNC(void, KSTACK_CODE) L_PollData_Con(void), L_Data_Ind(void), L_Busmon_Ind(void);
STATIC  FUNC(void, KSTACK_CODE) N_DataBroadcast_Req(void), L_Data_Con(void);


#else
static void N_DataIndividual_Req(void), N_DataGroup_Req(void), N_PollData_Req(void);
static void L_PollData_Con(void), L_Data_Ind(void), L_Busmon_Ind(void);
static void N_DataBroadcast_Req(void), L_Data_Con(void);


#endif /* KSTACK_MEMORY_MAPPING */

static const Knx_LayerServiceFunctionType KnxNl_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      L_DATA_IND              */ L_Data_Ind,
/*      L_DATA_CON              */ L_Data_Con,
/*      L_POLL_DATA_CON         */ L_PollData_Con,
/*      L_BUSMON_IND            */ L_Busmon_Ind,
/*      N_DATA_INDIVIDUAL_REQ   */ N_DataIndividual_Req,
/*      N_DATA_GROUP_REQ        */ N_DataGroup_Req,
/*      N_DATA_BROADCAST_REQ    */ N_DataBroadcast_Req,
/*      N_POLL_DATA_REQ         */ N_PollData_Req
/*      ====================================================*/
};

static const Knx_LayerServicesType KnxNl_ServiceTable[] = {
    { KNX_NL_SERVICES, SIZEOF_ARRAY(KnxNl_Services), KnxNl_Services }
};

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)  KnxNl_Task(void)
#else
void KnxNl_Task(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxDisp_DispatchLayer(TASK_NL_ID, KnxNl_ServiceTable);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxNl_Init(void)
#else
void KnxNl_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{

}


/*
**
**  Services from Link-Layer.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) L_Data_Ind(void)
#else
STATIC void L_Data_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if ((KnxMSG_GetMessagePtr(KnxMSG_ScratchBufferPtr)->npci & (uint8_t)0x80) == (uint8_t)0x80) {
        if (KnxADR_IsBroadcastAddress(*KnxMSG_GetMessagePtr(KnxMSG_ScratchBufferPtr)->dest)) {
            /* Broadcast-Communication. */
            //DBG_PRINTLN("Broadcast-Communication");
            KnxMSG_ScratchBufferPtr->service = N_DATA_BROADCAST_IND;
            (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
        } else {
            //DBG_PRINTLN("Multicast-Communication");
            /* Multicast-Communication. */
            KnxMSG_ScratchBufferPtr->service = N_DATA_GROUP_IND;
            (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
        }
    } else {
        //DBG_PRINTLN("Individual-Adressing");
        /* Individual-Adressing. */
        KnxMSG_ScratchBufferPtr->service = N_DATA_INDIVIDUAL_IND;
        (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) L_Data_Con(void)
#else
STATIC void L_Data_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) L_Busmon_Ind(void)
#else
STATIC void L_Busmon_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) L_PollData_Con(void)
#else
STATIC void L_PollData_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!!! */
}


/*
**
**  Services from Transport-Layer.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) N_DataBroadcast_Req(void)
#else
STATIC void N_DataBroadcast_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetAddressType(KnxMSG_ScratchBufferPtr, atMULTICAST);
    KnxMSG_SetRoutingCount(KnxMSG_ScratchBufferPtr);
    KnxMSG_ScratchBufferPtr->service = L_DATA_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) N_DataIndividual_Req(void)
#else
STATIC void N_DataIndividual_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    DBG_PRINTLN("N_DataIndividualReq");

    KnxMSG_SetAddressType(KnxMSG_ScratchBufferPtr, atINDIVIDUAL);
    KnxMSG_SetRoutingCount(KnxMSG_ScratchBufferPtr);
    KnxMSG_ScratchBufferPtr->service = L_DATA_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) N_DataGroup_Req(void)
#else
STATIC void N_DataGroup_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_SetAddressType(KnxMSG_ScratchBufferPtr, atMULTICAST);
    KnxMSG_SetRoutingCount(KnxMSG_ScratchBufferPtr);
    KnxMSG_ScratchBufferPtr->service = L_DATA_REQ;
    (void)KnxMSG_Post(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) N_PollData_Req(void)
#else
STATIC void N_PollData_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!!! */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) KnxNl_CheckRoutingCount(KnxMSG_BufferPtr pBuffer)
#else
static void KnxNl_CheckRoutingCount(KnxMSG_BufferPtr pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
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
