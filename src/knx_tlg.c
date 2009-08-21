#include "Transport.h"

static void Disp_T_DataGroupReq(void),Disp_T_PollDataReq(void),Disp_N_PollDataCon(void);
static void Disp_N_DataGroupInd(void),Disp_N_DataGroupCon(void);


static const KNXLayerServiceFunctionType TLG_Services[]={
/*      Service                     Handler                 */
/*      ====================================================*/
/*      N_DATA_GROUP_IND        */  Disp_N_DataGroupInd,
/*      N_DATA_GROUP_CON        */  Disp_N_DataGroupCon,
/*      N_POLL_DATA_CON         */  Disp_N_PollDataCon,
/*      T_DATA_GROUP_REQ        */  Disp_T_DataGroupReq,
/*      T_POLL_DATA_REQ         */  Disp_T_PollDataReq,
/*      ====================================================*/
};

static const KNXLayerServicesType TLG_ServiceTable[]={
    KNX_TLG_SERVICES,5,TLG_Services
};

void TLG_Task(void)
{
    KNXDispDispatchLayer(TASK_TL_ID,TLG_ServiceTable);
}

/*
**
**  Services from Network-Layer.
**
*/
static void Disp_N_DataGroupInd(void) 
{
    MSG_ScratchBuffer->service=T_DATA_GROUP_IND;
    (void)MSG_Post(MSG_ScratchBuffer);          
}

static void Disp_N_DataGroupCon(void) 
{
    MSG_ScratchBuffer->service=T_DATA_GROUP_CON;
    (void)MSG_Post(MSG_ScratchBuffer);          
}

static void Disp_N_PollDataCon(void) 
{
    MSG_ScratchBuffer->service=T_POLL_DATA_CON;
    (void)MSG_Post(MSG_ScratchBuffer);                             
}

/*
**
**  Services from Application-Layer.
**
*/
static void Disp_T_DataGroupReq(void) 
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_UDT);
    MSG_ScratchBuffer->service=N_DATA_GROUP_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_PollDataReq(void) 
{
    /* todo: Implementieren!!! */
}
