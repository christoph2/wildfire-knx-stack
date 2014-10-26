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
#include "knx_nl.h"
#include "knx_debug.h"

#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) NL_CheckRoutingCount(KnxMsg_BufferPtr pBuffer);
#else
static void KnxNl_CheckRoutingCount(KnxMsg_BufferPtr pBuffer);


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
STATIC void N_DataIndividual_Req(void), N_DataGroup_Req(void), N_PollData_Req(void);
STATIC void L_PollData_Con(void), L_Data_Ind(void), L_Busmon_Ind(void);
STATIC void N_DataBroadcast_Req(void), L_Data_Con(void);
#endif /* KSTACK_MEMORY_MAPPING */

STATIC void KnxNl_DispatchIncoming(Knx_ServiceTypeType const * services);

/*
**  Local Constants.
*/
STATIC const Knx_ServiceTypeType KnxNl_IndicationServices[3] = {
    KNX_SERVICE_N_DATA_BROADCAST_IND, KNX_SERVICE_N_DATA_GROUP_IND, KNX_SERVICE_N_DATA_INDIVIDUAL_IND
};

STATIC const Knx_ServiceTypeType KnxNl_ConfirmationServices[3] = {
    KNX_SERVICE_N_DATA_BROADCAST_CON, KNX_SERVICE_N_DATA_GROUP_CON, KNX_SERVICE_N_DATA_INDIVIDUAL_CON
};

STATIC const Knx_LayerServiceFunctionType KnxNl_Services[] = {
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
    KnxNl_DispatchIncoming(KnxNl_IndicationServices);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) L_Data_Con(void)
#else
STATIC void L_Data_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    //printf("L_Data_Con [%s]\n", (KnxMsg_ScratchBufferPtr->status == KNX_E_OK) ? "OK" : "NOT_OK");
    KnxNl_DispatchIncoming(KnxNl_ConfirmationServices);
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
    KnxMsg_SetAddressType(KnxMsg_ScratchBufferPtr, KNX_ADDR_MULTICAST);
    KnxMsg_SetRoutingCount(KnxMsg_ScratchBufferPtr);
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_L_DATA_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) N_DataIndividual_Req(void)
#else
STATIC void N_DataIndividual_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    //DBG_PRINTLN("N_DataIndividual_Req");

    KnxMsg_SetAddressType(KnxMsg_ScratchBufferPtr, KNX_ADDR_INDIVIDUAL);
    KnxMsg_SetRoutingCount(KnxMsg_ScratchBufferPtr);
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_L_DATA_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) N_DataGroup_Req(void)
#else
STATIC void N_DataGroup_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_SetAddressType(KnxMsg_ScratchBufferPtr, KNX_ADDR_MULTICAST);
    KnxMsg_SetRoutingCount(KnxMsg_ScratchBufferPtr);
    KnxMsg_ScratchBufferPtr->service = KNX_SERVICE_L_DATA_REQ;
    (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
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
STATIC FUNC(void, KSTACK_CODE) KnxNl_CheckRoutingCount(KnxMsg_BufferPtr pBuffer)
#else
STATIC void KnxNl_CheckRoutingCount(KnxMsg_BufferPtr pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (KnxMsg_GetRoutingCount(pBuffer) == MSG_NO_ROUTING_CTRL) {
        KnxMsg_SetRoutingCtrl(pBuffer, TRUE);
    } else {
        KnxMsg_SetRoutingCtrl(pBuffer, FALSE);
    }
}

/*
**
**  Local Helper Functions.
**
*/
STATIC void KnxNl_DispatchIncoming(Knx_ServiceTypeType const * services)
{
    if ((KnxMsg_GetMessagePtr(KnxMsg_ScratchBufferPtr)->npci & (uint8_t)0x80) == (uint8_t)0x80) {
        if (KnxADR_IsBroadcastAddress(*KnxMsg_GetMessagePtr(KnxMsg_ScratchBufferPtr)->dest)) {
            /* Broadcast-Communication. */
            DBG_PRINTLN("Broadcast-Communication [Con/Ind]");
            KnxMsg_ScratchBufferPtr->service = services[0];
            (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
        }
        else {
            /* Multicast-Communication. */
            DBG_PRINTLN("Multicast-Communication [Con/Ind]");
            KnxMsg_ScratchBufferPtr->service = services[1];
            (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
        }
    }
    else {
        /* Individual-Adressing. */
        DBG_PRINTLN("Individual-Adressing [Con/Ind]");
        KnxMsg_ScratchBufferPtr->service = services[2];
        (void)KnxMsg_Post(KnxMsg_ScratchBufferPtr);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
