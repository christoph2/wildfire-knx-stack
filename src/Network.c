
#include "Network.h"

void NL_CheckRoutingCount(PMSG_Buffer pBuffer);

/*
**  todo: define-Switch 'strict' (oder so...) der bei Managment-Telegrammen auf die Priorität 'SYSTEM' prüft.
**   Diese Option ließe sich noch auf andere Bereiche ausweiten (check: auf welche?).
*/


/*
**  Telegramme, die mit einem Routing-Count von 0..6 empfangen werden, werden grundsätzlich mit dem
**  Routing-Count 6 (bzw. EEPROM-Parameter) beantwortet, im Falle von 7 ist die Antwort ebenfalls 7.
*/

/*
** todo: Bit #1 des Control-Fields als 'ROUTING_CTRL' verwenden.
**       'No-Routing-Ctrl' betrifft hierbei den Network-Layer: Der Routing-Count wurde vom
**       Anwender festgelegt (s.o.), kann z.B. bei DAA (Distributed Address Asignment)
**       aber auch '0' sein (Subnet only).
*/


/*
**  Flags im Control-Field:
**  =======================
**  "The L_Data.con message exists in a positive and a negative version. Both versions differ in the
**  value of error flag contained in the control field octet: the positive L_Data.con message has the
**  error flag reset, the negative one set."
**  ERROR-Flag ist Bit #0.
*/

/*
**  todo: auf irgendeinem Layer für Broadcast_reqs die Dest.-Adresse auf '0000' setzen!!!
*/


/*
**  Hinweis: Der Link-Layer Parameter 'ack_request', wenn auf 'FALSE' gesetzt, verhindert
**           eine Telegramm-Wiederholung, falls das entsprechende Telegramm von keinem
**           Busteilnehmer ackknowledged wird!!!
*/

#define CONF_OK     0x00
#define CONF_ERROR  0x01


static void Disp_N_DataIndividualReq(void),Disp_N_DataGroupReq(void),Disp_N_PollDataReq(void);
static void Disp_L_PollDataCon(void),Disp_L_DataInd(void),Disp_L_BusmonInd(void);
static void Disp_N_DataBroadcastReq(void),Disp_L_DataCon(void);

static const KNXLayerServiceFunctionType NL_Services[]={
/*      Service                     Handler                 */
/*      ====================================================*/
/*      L_DATA_IND              */  Disp_L_DataInd,
/*      L_DATA_CON              */  Disp_L_DataCon,
/*      L_POLL_DATA_CON         */  Disp_L_PollDataCon,
/*      L_BUSMON_IND            */  Disp_L_BusmonInd,
/*      N_DATA_INDIVIDUAL_REQ   */  Disp_N_DataIndividualReq,
/*      N_DATA_GROUP_REQ        */  Disp_N_DataGroupReq,
/*      N_DATA_BROADCAST_REQ    */  Disp_N_DataBroadcastReq,
/*      N_POLL_DATA_REQ         */  Disp_N_PollDataReq
/*      ====================================================*/
};

static const KNXLayerServicesType NL_ServiceTable[]={
    {KNX_NL_SERVICES,8,NL_Services}
};


void NL_Task(void)
{
    KNXDispDispatchLayer(TASK_NL_ID,NL_ServiceTable);
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
    if ((MSG_GetMessagePtr(MSG_ScratchBuffer)->ncpi & 0x80)==0x80) { /* Hinweis: zu umständlich, vereinfachen!!! */
        if (ADR_IsBroadcastAddress(MSG_GetMessagePtr(MSG_ScratchBuffer)->dest)) { /* check: 'always avaluates to FALSE' !!! */
            /* Broadcast-Communication. */
            MSG_ScratchBuffer->service=N_DATA_BROADCAST_IND;
            (void)MSG_Post(MSG_ScratchBuffer);
        } else {
            /* Multicast-Communication. */
            MSG_ScratchBuffer->service=N_DATA_GROUP_IND;
            (void)MSG_Post(MSG_ScratchBuffer);
        }
    } else {
        /* Individual-Adressing. */
        MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_IND;
        (void)MSG_Post(MSG_ScratchBuffer);
    }
}

static void Disp_L_DataCon(void)
{
    /* todo: Implementieren!!! */
}

static void Disp_L_BusmonInd(void)
{
    /* todo: Implementieren!!! */
}

static void Disp_L_PollDataCon(void)
{
    /* todo: Implementieren!!! */
}


/*
**
**  Services from Transport-Layer.
**
*/
static void Disp_N_DataBroadcastReq(void)
{
    MSG_SetAddressType(MSG_ScratchBuffer,atMULTICAST);
    MSG_SetRoutingCount(MSG_ScratchBuffer);
    MSG_ScratchBuffer->service=L_DATA_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_DataIndividualReq(void)
{
    MSG_SetAddressType(MSG_ScratchBuffer,atINDIVIDUAL);
    MSG_SetRoutingCount(MSG_ScratchBuffer);
    MSG_ScratchBuffer->service=L_DATA_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_DataGroupReq(void)
{
    MSG_SetAddressType(MSG_ScratchBuffer,atMULTICAST);
    MSG_SetRoutingCount(MSG_ScratchBuffer);
    MSG_ScratchBuffer->service=L_DATA_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_PollDataReq(void)
{
    /* todo: Implementieren!!! */
}

void NL_CheckRoutingCount(PMSG_Buffer pBuffer)
{
    if (MSG_GetRoutingCount(pBuffer)==MSG_NO_ROUTING_CTRL) {
        MSG_SetRoutingCtrl(pBuffer,TRUE);
    } else {
        MSG_SetRoutingCtrl(pBuffer,FALSE);
    }
}
