/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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

void NL_CheckRoutingCount(PMSG_Buffer pBuffer);

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

static void Disp_N_DataIndividualReq(void), Disp_N_DataGroupReq(void), Disp_N_PollDataReq(void);
static void Disp_L_PollDataCon(void), Disp_L_DataInd(void), Disp_L_BusmonInd(void);
static void Disp_N_DataBroadcastReq(void), Disp_L_DataCon(void);

static const KNXLayerServiceFunctionType NL_Services[] = {
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

static const KNXLayerServicesType NL_ServiceTable[] = {
    {KNX_NL_SERVICES, (uint8)8, NL_Services}
};

void NL_Task(void)
{
    KNXDispDispatchLayer(TASK_NL_ID, NL_ServiceTable);
}

void NL_Init(void)
{

}

/*
**
**  Services from Link-Layer.
**
*/

static void Disp_L_DataInd(void)
{
    if ((MSG_GetMessagePtr(MSG_ScratchBuffer)->ncpi & (uint8)0x80) == (uint8)0x80) {
        if (ADR_IsBroadcastAddress(MSG_GetMessagePtr(MSG_ScratchBuffer)->dest)) {
            /* Broadcast-Communication. */
            MSG_ScratchBuffer->service = N_DATA_BROADCAST_IND;
            (void)MSG_Post(MSG_ScratchBuffer);
        } else {
            /* Multicast-Communication. */
            MSG_ScratchBuffer->service = N_DATA_GROUP_IND;
            (void)MSG_Post(MSG_ScratchBuffer);
        }
    } else {
        /* Individual-Adressing. */
        MSG_ScratchBuffer->service = N_DATA_INDIVIDUAL_IND;
        (void)MSG_Post(MSG_ScratchBuffer);
    }
}

static void Disp_L_DataCon(void)
{
    /* todo: Implement!! */
}

static void Disp_L_BusmonInd(void)
{
    /* todo: Implement!!! */
}

static void Disp_L_PollDataCon(void)
{
    /* todo: Implement!!! */
}

/*
**
**  Services from Transport-Layer.
**
*/
static void Disp_N_DataBroadcastReq(void)
{
    MSG_SetAddressType(MSG_ScratchBuffer, atMULTICAST);
    MSG_SetRoutingCount(MSG_ScratchBuffer);
    MSG_ScratchBuffer->service = L_DATA_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_DataIndividualReq(void)
{
    MSG_SetAddressType(MSG_ScratchBuffer, atINDIVIDUAL);
    MSG_SetRoutingCount(MSG_ScratchBuffer);
    MSG_ScratchBuffer->service = L_DATA_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_DataGroupReq(void)
{
    MSG_SetAddressType(MSG_ScratchBuffer, atMULTICAST);
    MSG_SetRoutingCount(MSG_ScratchBuffer);
    MSG_ScratchBuffer->service = L_DATA_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_PollDataReq(void)
{
    /* todo: Implement!!! */
}

void NL_CheckRoutingCount(PMSG_Buffer pBuffer)
{
    if (MSG_GetRoutingCount(pBuffer) == MSG_NO_ROUTING_CTRL) {
        MSG_SetRoutingCtrl(pBuffer, TRUE);
    } else {
        MSG_SetRoutingCtrl(pBuffer, FALSE);
    }
}

