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
#include "knx_layer_transport.h"

static void Disp_T_DataGroupReq(void), Disp_T_PollDataReq(void), Disp_N_PollDataCon(void);
static void Disp_N_DataGroupInd(void), Disp_N_DataGroupCon(void);

static const KNXLayerServiceFunctionType TLG_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      N_DATA_GROUP_IND        */ Disp_N_DataGroupInd,
/*      N_DATA_GROUP_CON        */ Disp_N_DataGroupCon,
/*      N_POLL_DATA_CON         */ Disp_N_PollDataCon,
/*      T_DATA_GROUP_REQ        */ Disp_T_DataGroupReq,
/*      T_POLL_DATA_REQ         */ Disp_T_PollDataReq,
/*      ====================================================*/
};

static const KNXLayerServicesType TLG_ServiceTable[] = {
    {KNX_TLG_SERVICES, 5, TLG_Services}
};

void TLG_Task(void)
{
    KNXDispDispatchLayer(TASK_TL_ID, TLG_ServiceTable);
}

/*
**
**  Services from Network-Layer.
**
*/
static void Disp_N_DataGroupInd(void)
{
    MSG_ScratchBuffer->service = T_DATA_GROUP_IND;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_DataGroupCon(void)
{
    MSG_ScratchBuffer->service = T_DATA_GROUP_CON;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_PollDataCon(void)
{
    MSG_ScratchBuffer->service = T_POLL_DATA_CON;
    (void)MSG_Post(MSG_ScratchBuffer);
}

/*
**
**  Services from Application-Layer.
**
*/
static void Disp_T_DataGroupReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer, TPCI_UDT);
    MSG_ScratchBuffer->service = N_DATA_GROUP_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_PollDataReq(void)
{
    /* todo: Implement!!! */
}

