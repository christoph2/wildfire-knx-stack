/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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

/*
**  Application-Layer / Managment.
*/


/*
#ifdef DEBUG
    assert(r==TRUE)
    DEBUG_PRINT("Error releasing Message-Buffer @ " ## __FILE__ ## "(" ## __LINE__ ## ")")
#endif
*/



#include "Application.h"

void A_Broadcast_Req(PMSG_Buffer pBuffer,Knx_AddressType source,uint16 apci,uint8 *data,uint8 len);
void A_Individual_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint16 apci,uint8 *data,uint8 len);

void AL_SetPropertyHeader(PMSG_Buffer pBuffer,uint8 obj_index,uint8 prop_id,uint8 nr_of_elem,uint16 start_index);


#define APDU_PHYS_ADDR      ((uint8)0)   /* A_IndividualAddress_Write-PDU */

/*
M_USER_DATA_INDIVIDUAL_REQ
M_USER_DATA_CONNECTED_REQ
M_INTERFACEOBJ_DATA_REQ
*/

/*
**  Praefixes:
**  ==========================
**  GOS_ : GroupObject-Server.
**  MMS_ : Memory-Server (check: getrennter UserMemory-Server? - ('MEM_' ?)).
**  PPS_ : Property-Server ('IOS' - 'Interface-Object-Server').
**  DMO_ : Device-Model.
*/


static void Disp_T_ConnectInd(void),Disp_T_ConnectCon(void),Disp_T_DisconnectInd(void);
static void Disp_T_DisconnectCon(void),Disp_T_DataConnectedInd(void),Disp_T_DataConnectedCon(void);
static void Disp_T_DataBroadcastInd(void),Disp_T_DataBroadcastCon(void),Disp_T_DataIndividualInd(void);
static void Disp_T_DataIndividualCon(void);


static const KNXLayerServiceFunctionType ALM_Services[]={
/*      Service                     Handler                 */
/*      ====================================================*/
/*      T_CONNECT_IND           */  Disp_T_ConnectInd,
/*      T_CONNECT_CON           */  Disp_T_ConnectCon,
/*      T_DISCONNECT_IND        */  Disp_T_DisconnectInd,
/*      T_DISCONNECT_CON        */  Disp_T_DisconnectCon,
/*      T_DATA_CONNECTED_IND    */  Disp_T_DataConnectedInd,
/*      T_DATA_CONNECTED_CON    */  Disp_T_DataConnectedCon,
/*      T_DATA_INDIVIDUAL_IND   */  Disp_T_DataIndividualInd,
/*      T_DATA_INDIVIDUAL_CON   */  Disp_T_DataIndividualCon,
/*      T_DATA_BROADCAST_IND    */  Disp_T_DataBroadcastInd,
/*      T_DATA_BROADCAST_CON    */  Disp_T_DataBroadcastCon
/*      ====================================================*/
};

static const KNXLayerServicesType ALM_ServiceTable[]={
    {KNX_ALM_SERVICES,10,ALM_Services}
};


void ALM_Task(void)
{
    KNXDispDispatchLayer(TASK_MG_ID,ALM_ServiceTable);
}


/*
**
**  Services from Transport-Layer.
**
*/

static void Disp_T_ConnectInd(void)
{
    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
}

static void Disp_T_DisconnectInd(void)
{
    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
}

static void Disp_T_DataConnectedInd(void)
{
    uint8 apci_type=AL_GetAPCIType(MSG_GetMessagePtr(MSG_ScratchBuffer));

    switch (apci_type) {
        case apciMEMORY_READ:
            break;
        case apciMEMORY_WRITE:
            break;
        case apciESCAPE:
            break;
        case apciADC_READ:
            break;
        case apciRESTART:
            break;
        case apciUSER_MSG:
            break;
        default:
            (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
            break;
    }

    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
}

static void Disp_T_DataBroadcastInd(void)
{
    uint8 apci_type=AL_GetAPCIType(MSG_GetMessagePtr(MSG_ScratchBuffer));
    Knx_AddressType addr;

    switch (apci_type) {
        case apciINDIVIDUAL_ADDRESS_WRITE:
            if (ADR_InProgrammingMode()) {  /* todo: only if 'PID_SERVICE_CONTROL' is activatet! */
                AL_GetAPDUData(MSG_GetMessagePtr(MSG_ScratchBuffer),APDU_PHYS_ADDR,(uint8*)&addr,(uint8)2);
/*                                addr[0]=AL_GetAPDUDataByte(pmsg,APDU_PHYS_ADDR); */
/*                                addr[1]=AL_GetAPDUDataByte(pmsg,APDU_PHYS_ADDR+1); */
                ADR_SetPhysAddr(addr);
            }
            (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
            break;
        case apciINDIVIDUAL_ADDRESS_READ:
            if (ADR_InProgrammingMode()) {
                addr=ADR_GetPhysAddr();
                (void)MSG_ClearBuffer(MSG_ScratchBuffer);
                A_IndividualAddress_Read_Res(MSG_ScratchBuffer,addr);
            }
            break;
        case apciESCAPE:
            break;
        default:
            break;
    }
}

static void Disp_T_DataIndividualInd(void)
{
    uint8 apci_type=AL_GetAPCIType(MSG_GetMessagePtr(MSG_ScratchBuffer));

    if (apci_type==apciESCAPE) {
        switch (MSG_GetAPCI(MSG_ScratchBuffer)) {
            case A_PROPERTYVALUE_READ:
                IOS_Dispatch(MSG_ScratchBuffer,IOS_PROP_READ,FALSE);
                break;
            case A_PROPERTYVALUE_WRITE:
                IOS_Dispatch(MSG_ScratchBuffer,IOS_PROP_WRITE,FALSE);
                break;
            case A_PROPERTYDESCRIPTION_READ:
                IOS_Dispatch(MSG_ScratchBuffer,IOS_PROP_DESC_READ,FALSE);
                break;
            default:
                (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
                break;
        }
    }
}

static void Disp_T_ConnectCon(void)
{
    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
}

static void Disp_T_DisconnectCon(void)
{
    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
}

static void Disp_T_DataConnectedCon(void)
{
    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
}

static void Disp_T_DataBroadcastCon(void)
{
    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
}

static void Disp_T_DataIndividualCon(void)
{
    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
}

/*
**
**  Services from User-Layer.
**
*/


/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/



/* REF: EIB-Handbuch 2/1/1 "EIB-System-Introduction" s. 43. */
uint8 AL_GetAPCIType(const KNX_StandardFrameRefType pmsg)
{
        return ((pmsg->tpci & (uint8)0x03)<<2) | ((pmsg->apci & (uint8)0xC0)>>6);
}

uint8 AL_GetAPDUDataByte(const KNX_StandardFrameRefType pmsg,uint8 offset)
{
    offset=MIN(offset,MAX_ADPU_LEN-(uint8)1);

    return pmsg->data[offset];
}

void AL_SetAPDUDataByte(const KNX_StandardFrameRefType pmsg,uint8 offset,const uint8 value)
{
    offset=MIN(offset,MAX_ADPU_LEN-(uint8)1);

    pmsg->data[offset]=value;
}


void AL_GetAPDUData(const KNX_StandardFrameRefType pmsg,uint8 offset,uint8 *data,uint8 len)
{
    offset=MIN(offset,MAX_ADPU_LEN-(uint8)1);
    len=MIN(len,MAX_ADPU_LEN-offset);

    if (len>(uint8)0 && (data!=(uint8*)NULL)) {
        CopyRAM(data,(uint8*)pmsg->data+offset,len);
    }
}

void AL_SetAPDUData(const KNX_StandardFrameRefType pmsg,uint8 offset,uint8 *data,uint8 len)
{
    offset=MIN(offset,MAX_ADPU_LEN-(uint8)1);
    len=MIN(len,MAX_ADPU_LEN-offset);

    if ((len>(uint8)0) && (data!=(uint8*)NULL)) {
        CopyRAM((uint8*)pmsg->data+offset,data,len);
    }
}

/*
**
**
**  BROADCAST-Services.
**
**
*/

void A_Broadcast_Req(PMSG_Buffer pBuffer,Knx_AddressType source,uint16 apci,uint8 *data,uint8 len)
{

    len=MIN(len,MAX_ADPU_LEN);

    (void)MSG_ClearBuffer(pBuffer);

    MSG_SetAPCI(pBuffer,apci);
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,(uint8)8+len);
    AL_SetAPDUData(MSG_GetMessagePtr(pBuffer),(uint8)0,data,len);
    pBuffer->service=T_DATA_BROADCAST_REQ;

    (void)MSG_Post(pBuffer);
}

/*
**  A_IndividualAddress
*/

void A_IndividualAddress_Read_Res(PMSG_Buffer pBuffer,Knx_AddressType source)
{
    A_Broadcast_Req(pBuffer,source,A_PHYSICALADDRESS_RESPONSE,(uint8*)NULL,(uint8)0);
}

void A_IndividualAddress_Read_Req(PMSG_Buffer pBuffer,Knx_AddressType source)
{
    A_Broadcast_Req(pBuffer,source,A_PHYSICALADDRESS_READ,(uint8*)NULL,(uint8)0);
}

void A_IndividualAddress_Write_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType newaddress)
{
/*    uint8 data[2]; */

/*
**    data[0]=newaddress[0];
**    data[1]=newaddress[1];
**    A_Broadcast_Req(pBuffer,source,A_PHYSICALADDRESS_WRITE,(uint8*)data,2);
*/
    A_Broadcast_Req(pBuffer,source,A_PHYSICALADDRESS_WRITE,(uint8*)&source,(uint8)2);
}

/*
**  A_IndividualAddressSerialNumber
*/

void A_IndividualAddressSerialNumber_Read_Res(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_SerialNumberType serial_number,
        Knx_AddressType domain_addr)
{
    AL_SetAPDUData(MSG_GetMessagePtr(pBuffer),(uint8)0,serial_number,sizeof(Knx_SerialNumberType));
    AL_SetAPDUData(MSG_GetMessagePtr(pBuffer),(uint8)6,(uint8*)&domain_addr,sizeof(Knx_AddressType));

    A_Broadcast_Req(pBuffer,source,A_PHYSICALADDRESSSERIALNUMBER_RESPONSE,(uint8*)NULL,(uint8)10);
}

void A_IndividualAddressSerialNumber_Read_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_SerialNumberType serial_number)
{
    AL_SetAPDUData(MSG_GetMessagePtr(pBuffer),(uint8)0,serial_number,sizeof(Knx_SerialNumberType));

    A_Broadcast_Req(pBuffer,source,A_PHYSICALADDRESSSERIALNUMBER_READ,(uint8*)NULL,(uint8)6);
}


void A_IndividualAddressSerialNumber_Write_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_SerialNumberType serial_number,
        Knx_AddressType new_addr)
{
    AL_SetAPDUData(MSG_GetMessagePtr(pBuffer),(uint8)0,serial_number,sizeof(Knx_SerialNumberType));
    AL_SetAPDUData(MSG_GetMessagePtr(pBuffer),(uint8)6,(uint8*)&new_addr,sizeof(Knx_AddressType));

    A_Broadcast_Req(pBuffer,source,A_PHYSICALADDRESSSERIALNUMBER_WRITE,(uint8*)NULL,(uint8)12);
}


/*
**  A_ServiceInformation_Indication_Write
*/

void A_ServiceInformation_Indication_Write_Req(PMSG_Buffer pBuffer,Knx_AddressType source,uint8 info)
{
    uint8 data[3];

    data[0]=info;
    data[1]=data[2]=(uint8)0x00;   /* reserved. */

    A_Broadcast_Req(pBuffer,source,A_SERVICEINFORMATION_INDICATION_WRITE,(uint8*)data,3);
}

/*
**  A_DomainAddress
*/

void A_DomainAddress_Read_Res(PMSG_Buffer pBuffer,Knx_AddressType source)
{
    A_Broadcast_Req(pBuffer,source,A_DOMAINADDRESS_RESPONSE,(uint8*)NULL,(uint8)0);
}

void A_DomainAddress_Read_Req(PMSG_Buffer pBuffer,Knx_AddressType source)
{
    A_Broadcast_Req(pBuffer,source,A_DOMAINADDRESS_READ,(uint8*)NULL,(uint8)0);
}

void A_DomainAddress_Write_req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType domain_ddress)
{
    uint8 data[2];

/*    data[0]=domain_ddress[0];
/*    data[1]=domain_ddress[1]; */

    *(uint16*)data[0]=domain_ddress;

    A_Broadcast_Req(pBuffer,source,A_DOMAINADDRESS_WRITE,(uint8*)data,(uint8)2);
}

void A_DomainAddressSelective_Read_req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType domain_ddress,
        Knx_AddressType start_address,uint8 range)
{
    uint8 data[5];

    *(uint16*)data[0]=domain_ddress;
    *(uint16*)data[2]=start_address;
/*
    data[0]=domain_ddress[0];
    data[1]=domain_ddress[1];
    data[2]=start_address[0];
    data[3]=start_address[1];
*/
    data[4]=range;

    A_Broadcast_Req(pBuffer,source,A_DOMAINADDRESSSELECTIVE_READ,(uint8*)data,(uint8)5);
}

/*
**
**
**  Point-to-Point-Connectionless-Services.
**
**
*/

void A_Individual_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint16 apci,uint8 *data,uint8 len)
{

    len=MIN(len,MAX_ADPU_LEN);

    (void)MSG_ClearBuffer(pBuffer);

    MSG_SetAPCI(pBuffer,apci);
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,(uint8)8+len);
    AL_SetAPDUData(MSG_GetMessagePtr(pBuffer),(uint8)0,data,len);
    pBuffer->service=T_DATA_INDIVIDUAL_REQ;

    (void)MSG_Post(pBuffer);
}


void AL_SetPropertyHeader(PMSG_Buffer pBuffer,uint8 obj_index,uint8 prop_id,uint8 nr_of_elem,uint16 start_index)
{
    uint8 data[4];

    data[0]=obj_index;
    data[1]=prop_id;
    data[2]=((nr_of_elem & (uint8)0x0f)<<4) | (HIBYTE(start_index) & (uint8)0x0f);
    data[3]=LOBYTE(start_index);

    AL_SetAPDUData(MSG_GetMessagePtr(pBuffer),(uint8)0,data,(uint8)4);
}

void A_PropertyValue_Read_Res(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 obj_index,
        uint8 prop_id,uint8 nr_of_elem,uint16 start_index,uint8 *data)
{
    AL_SetPropertyHeader(pBuffer,obj_index,prop_id,nr_of_elem,start_index);

/* Data-Offset = 4 */
/* A_PROPERTYVALUE_RESPONSE */
}

void A_PropertyValue_Read_Res_NoData(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 obj_index,
        uint8 prop_id,uint8 nr_of_elem,uint16 start_index)
{
    AL_SetPropertyHeader(pBuffer,obj_index,prop_id,nr_of_elem,start_index);
    A_Individual_Req(pBuffer,source,dest,A_PROPERTYVALUE_RESPONSE,(uint8*)NULL,(uint8)4);
}

void A_PropertyValue_Read_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 obj_index,
        uint8 prop_id,uint8 nr_of_elem,uint16 start_index)
{
    AL_SetPropertyHeader(pBuffer,obj_index,prop_id,nr_of_elem,start_index);
    A_Individual_Req(pBuffer,source,dest,A_PROPERTYVALUE_READ,(uint8*)NULL,(uint8)4);
}

/*
A_PROPERTYVALUE_WRITE
*/


void A_PropertyDescription_Read_Res(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 obj_index,
        uint8 prop_id,uint8 prop_index,uint8 type,uint16 nr_of_elem,uint8 access)
{
    uint8 data[7];

/* todo: AL_SetADPUData */
    data[0]=obj_index;
    data[1]=prop_id;
    data[2]=prop_index;
    data[3]=type;
    data[4]=HIBYTE(nr_of_elem);
    data[5]=LOBYTE(nr_of_elem);
    data[6]=access;

    A_Individual_Req(pBuffer,source,dest,A_PROPERTYDESCRIPTION_RESPONSE,(uint8*)data,(uint8)7);
}

void A_PropertyDescription_Read_Res_NoData(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 obj_index,
        uint8 prop_id,uint8 prop_index)
{
    uint8 data[3];

/* todo: AL_SetADPUData */
    data[0]=obj_index;
    data[1]=prop_id;
    data[2]=prop_index;

    A_Individual_Req(pBuffer,source,dest,A_PROPERTYDESCRIPTION_RESPONSE,(uint8*)data,(uint8)3);
}

void A_PropertyDescription_Read_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 obj_index,
        uint8 prop_id,uint8 prop_index)
{
    uint8 data[3];
/* todo: AL_SetADPUData */
    data[0]=obj_index;
    data[1]=prop_id;
    data[2]=prop_index;

    A_Individual_Req(pBuffer,source,dest,A_PROPERTYDESCRIPTION_READ,(uint8*)data,(uint8)3);
}

/*
Device-Descriptor:

Descriptor Type 0 (mask version)
--------------------------------

Mask Type (8 bits)          Firmware Version (8 bits)
MMMM        TTTT            VVVV    SSSS
Medium Type Firmware Type   Version Sub-code

Medium Type                         Firmware Type
-------------------------------------------------------------------------
0000b 0h Twisted Pair 1             0000b 0h Standard BAU
0001b 1h Power line 110             0111b 7h Control Unit
0010b 2h Radio Frequency            1000b 8h IR-Decoder
0011b 3h Twisted Pair 0             1001b 9h Coupler to TP
0100b 4h Power line 132             1111b Fh Escape see Descriptor Type 1
0101b 5h Reserved for fast Media
1111b Fh Escape


Mask    Mask    Medium          Device                  Version
Type    Version
00h     10h     Twisted Pair 1  BCU 1, BIM M111         0
00h     11h     Twisted Pair 1  BCU 1, BIM M111         1
00h     12h     Twisted Pair 1  BCU 1, BIM M111         2
10h     12h     Power Line 110  BCU 1                   2
10h     13h     Power Line 110  BCU 1                   3
00h     20h     Twisted Pair 1  BCU 2, BIM M113         0
07h     00h     Twisted Pair 1  BIM M112                0
07h     01h     Twisted Pair 1  BIM M112                1
08h     10h     Twisted Pair 1  IR-Decoder              0
08h     11h     Twisted Pair 1  IR-Decoder              1
09h     10h     Twisted Pair 1  Coupler 1.0             0
09h     11h     Twisted Pair 1  Coupler 1.1             1
19h     00h     Twisted Pair 1  Media Coupler PL-TP     0
30h     12h     Twisted Pair 0  BCU 1                   2
40h     12h     Power Line 132  BCU 1                   2

*/

