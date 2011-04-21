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
    Parameters of Data Link Layer
    -----------------------------
    The following parameters influence the behaviour of Data Link Layer and are
    required inside Data Link Layer in order to operate correctly:

    - Individual Address:    unique Individual Address of this device
    - address table:         address table with the group address(es) of this device
    - nak_retry:             defines the number of retries in case of a NAK response
                             or a acknowledgment timeout
    - busy_retry:            defines the number of retries in case of a BUSY response
    - poll group:            address the poll group address of this device
    - response slot:         number the response slot number of this device
    - data link layer mode:  either the normal or the bus monitor mode of the data link layer.
*/

/*
typedef enum tagKNX_BusStateType {
    E_BUSSTATE_OK,
    E_BUSSTATE_BUSOFF,
    E_BUSSTATE_SAVING
} KNX_BusStateType;

void BusStateHandler(KNX_BusStateType);

KNX_BusStateType PH_GetBusState(void);
*/

#if !defined(__HIWARE__)
/* #include <stdio.h> */
#endif

#include "tpuart.h"
#include "Messaging.h"

#define ACK_INFORMATION ((uint8)0x10)

#define ACK_ADDRESSED   ((uint8)0x01)
#define ACK_BUSY        ((uint8)0x02)
#define ACK_NACK        ((uint8)0x04)

static void Disp_L_DataReq(void),Disp_L_PollDataReq(void);

static const KNXLayerServiceFunctionType LL_Services[]={
/*      Service                     Handler                 */
/*      ====================================================*/
/*      L_DATA_REQ              */  Disp_L_DataReq,
/*      L_POLL_DATA_REQ         */  Disp_L_PollDataReq,
/*      ====================================================*/
};

static const KNXLayerServicesType LL_ServiceTable[]={
    {KNX_LL_SERVICES,2,LL_Services}
};



/*
**  "The local layer-2 passes an L_Data.con primitive to the local user that indicates either a correct or
**  erroneous data transfer. Depending if an L2-acknowledgement is requested or not, this confirmation is
**  related to the reception of the L2-acknowledgement, or only to the transmission of the frame on the
**  medium."
*/


/* const uint8 T0[]={0xbc,0x11,0x64,0x09,0x01,0xe3,0x00,0x80,0x0d,0x14,0x44}; */
/*      ==> EIS5-Telegramm (26.00) */
/*      von     1.01.100 nach 1/1/001 */

/* const uint8 T[]={0xbc,0x11,0x01,0x49,0x01,0xe2,0x00,0x80,0x5f,0x26}; */
/*      (01.01.001==>09/1/001 Val: 103) */


/*uint8 T1[]={0xbc,0x11,0x64,0x11,0x01,0x61,0x43,0x80,0x84}; */
    /* A_Restart. */

/*uint8 T1[]={0xb0,0x11,0x64,0x00,0x00,0xE3,0x00,0xC0,0xaf,0xfe,0x48}; */
    /* A_PhysAddr_Write(0xAFFE). */

/*uint8 T1[]={0xb0,0x11,0x64,0x00,0x00,0xE1,0x01,0x00,0xda}; */
   /* A_PhysAddr_Read(). */

/*uint8 T1[]={0xb0,0x11,0x64,0x11,0x01,0x65,0x03,0xd5,0x01,0x05,0x10,0x00,0x8d}; */
    /* A_PropertyValueRead(). */

/* const uint8 T1[]={0xb0,0x11,0x64,0x11,0x01,0x64,0x03,0xd8,0x00,0x00,0x06,0x93}; */
    /* A_PropertyDescriptionRead(). */

const uint8 T0[]={
   (uint8)0xbc,(uint8)0x11,(uint8)0x64,(uint8)0x11,(uint8)0x01,(uint8)0x60,(uint8)0x80,(uint8)0xc6
};
    /* T_Connect (01.01.100 ==> 01.01.001). */

const uint8 T1[]={
   (uint8)0xbc,(uint8)0x11,(uint8)0x64,(uint8)0x11,(uint8)0x01,(uint8)0x67,(uint8)0x42,(uint8)0x84,
   (uint8)0x01,(uint8)0x80,(uint8)0xaa,(uint8)0xbb,(uint8)0xcc,(uint8)0xdd,(uint8)0x06
};
    /* A_MemoryWrite */

const uint8 T2[]={
   (uint8)0xbc,(uint8)0x11,(uint8)0x64,(uint8)0x11,(uint8)0x01,(uint8)0x60,(uint8)0x81,(uint8)0xc7
};
    /* T_Disconnect (01.01.100 ==> 01.01.001). */


TPUART_RCV_STATE rcvState;
TPUART_RCV_SERVICE rcvService;

uint8 RcvLen;
uint8 RcvIdx;
uint8 Checksum;
uint8 tsap;
Knx_AddressType dest_addr;
boolean addressed;

uint8 TpuartRcvBuf[BUF_LEN];

PMSG_Buffer pBuffer;

uint8 AckService;

uint8 CalculateChecksum(PMSG_Buffer ptr);
void PH_AckInformation_req(uint8 flags);
void DBG_DUMP(PMSG_Buffer ptr);

void decode(uint8 b);

void StartTimeout(void);
void StopTimeout(void);
void OnTimeout(void);           /* Callback. */

void TPUARTInit(void)
{
        rcvState=TPSR_WAIT_RESET_IND;
        RcvLen=RcvIdx=(uint8)0x00;
        AckService=(uint8)0x00;
        rcvService=SERVICE_NONE;
        Utl_MemSet(TpuartRcvBuf,0,BUF_LEN);
        rcvState=TPSR_WAIT;
}

void TPTest(void)
{
        int b,len;
        static int cnt;

        switch (cnt) {
            case (uint8)0:
                len=sizeof(T0);

                for (b=(uint8)0;b<len;b++) {
                    decode(T0[b]);
                }
                cnt++;
                break;
            case (uint8)1:
                len=sizeof(T1);

                for (b=(uint8)0;b<len;b++) {
                    decode(T1[b]);
                }
                cnt++;
                break;
            case (uint8)2:
                len=sizeof(T2);

                for (b=(uint8)0;b<len;b++) {
                    decode(T2[b]);
                }
                cnt++;
                break;
            default:
                cnt=(uint8)0;
                break;
        }

/*
        len=sizeof(T);

        for (b=0;b<len;b++) {
                decode(T[b]);
        }
*/
}

void decode(uint8 b)
{
        uint8 state;

        boolean stop;

        TpuartRcvBuf[RcvIdx++]=b;

        switch (rcvState) {
                case TPSR_WAIT:

/*
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
*/
                if ((b & (uint8)0x13)==DATA_SERVICE_MASK) {
                        StartTimeout();

/*                      prio=(b & PRIO_MASK)>>2; */

/*                      repeated=((~b) & REPEATED_MASK)>>5;     // 1 ==> not repeated. */

/*                      rcvService=SERVICE_DATA; */

                        if ((b & (uint8)0xd0)==L_DATA_IND_MASK) {
                                RcvLen=(uint8)5;
                                tsap=(uint8)0x00;  /* 0xFE */
                                Checksum=b;
                                rcvState=TPSR_DATA_CONT1;
                        } else if ((b & (uint8)0xd0)==L_LONG_DATA_IND_MASK) {
/*                              rcvState=TPSR_LONG_DATA; */
                                rcvState=TPSR_WAIT;
                        } else if ((b & (uint8)0xff)==L_POLL_DATA_IND_MASK) {
                                rcvState=TPSR_POLL_DATA;
                        } else {        /* Errornous Byte. */
                                rcvState=TPSR_WAIT;     /* wait for valid Service. */
                                rcvService=SERVICE_NONE;
#if !defined(__HIWARE__)
/*    printf("0x%02x\tERROR\n",b); */
#endif
                        }
                } else if ((b==IACK_IND) || (b==INACK_IND) || (b==IBUSY_IND)) {
                        rcvState=TPSR_WAIT;     /* wait for next Service. */
                        rcvService=SERVICE_IACK;
                        RcvIdx=(uint8)0;
                        AckService=b;
                } else if ((b & (uint8)0x03)==(uint8)0x03) {
                        rcvState=TPSR_WAIT;     /* wait for next Service. */
                        rcvService=SERVICE_CONTROL;
                        RcvIdx=(uint8)0;
                        if (b==RESET_IND) {

                        } else if ((b & (uint8)0x7f)==(uint8)0x0b) {

                        } else if ((b & (uint8)0x07)==(uint8)0x07) {

                                state=(b & (uint8)0xf8)>>3;
                        } else {        /* Errornous Byte. */
                                rcvService=SERVICE_NONE;
#if !defined(__HIWARE__)
/*    printf("0x%02x\tERROR\n",b); */
#endif
                        }
                }
/*
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
*/

                        break;
                case TPSR_DATA_CONT1:
                        Checksum^=b;
                        if ((--RcvLen)==(uint8)0x00) {
                            /* if (!PassthroughEveryTelegramm()) // RouteEvery */

                            dest_addr=btohs(*(uint16*)&TpuartRcvBuf[3]);

                            if ((TpuartRcvBuf[5] & atMULTICAST)) {
                                addressed=ADR_IsAddressed(dest_addr,&tsap);
                            } else {
                                addressed=ADR_IsOwnPhysicalAddr(dest_addr);
                            }

                            if (addressed) {
                                PH_AckInformation_req(ACK_ADDRESSED);
                            }

                            RcvLen=(b & (uint8)0x0f)+2;    /* todo: Limit. */
                            rcvState=TPSR_DATA_CONT2;
                        }
                        break;
                case TPSR_DATA_CONT2:
                        if ((--RcvLen)==(uint8)0x00) { /* O.K., complete Telegram receeived. */
                                Checksum^=(uint8)0xff;

                                if ((Checksum==TpuartRcvBuf[RcvIdx-(uint8)1])) { /* Checksum valid? */
                                    if (addressed) {
                                        pBuffer=MSG_AllocateBuffer();
                                        if (pBuffer!=(PMSG_Buffer)NULL) {
                                            pBuffer->service=L_DATA_IND;
                                            pBuffer->sap=tsap;
                                            pBuffer->len=RcvLen=(TpuartRcvBuf[5] & (uint8)0x0f)+(uint8)7;

                                            Utl_MemCopy((void*)pBuffer->msg,(void*)TpuartRcvBuf,RcvLen);
                                            (void)MSG_Post(pBuffer);
                                        } else {
                                            stop=TRUE;
                                            /* todo: Error-Handling. */
                                        }
                                    }

                                } else {
#if !defined(__HIWARE__)
/*    printf("\n*** CHECKSUM-ERROR ***\n"); */
#endif
                                }

                                rcvService=SERVICE_DATA;
                                rcvState=TPSR_WAIT;
                                RcvIdx=(uint8)0;
                        } else {
                                Checksum^=b;
                        }
                        break;
/*
                case TPSR_LONG_DATA:

                        break;
*/
                case TPSR_POLL_DATA:

                        break;
                default:
                        break;
        }
}

void StartTimeout(void)
{

}

void StopTimeout(void)
{

}

void OnTimeout(void)            /* Callback. */
{
/*      Differenzieren: z.B.: 'TPSR_WAIT_RESET_IND'. */
        rcvState=TPSR_WAIT;
}

void DBG_DUMP(PMSG_Buffer ptr)
{
    uint8 i/*,chk*/;
#if     !defined(__HIWARE__)
    for (i=(uint8)0;i<ptr->len;i++) {

/*        printf("%02X ",ptr->msg[i]);  */

        if (i==(uint8)6) {
/*            printf("["); */
        }
    }
/*    printf("]\t%02X\n",CalculateChecksum(ptr)); */
#endif
}

void PH_AckInformation_req(uint8 flags)
{
/* PutSCI(ACK_INFORMATION | flags); */
}

void LL_Task(void)
{
        KNXDispDispatchLayer(TASK_LL_ID,LL_ServiceTable);
}

/*
**
**  Services from Network-Layer.
**
*/

static void Disp_L_DataReq(void)
{
    uint8 chk;

    MSG_SetFrameType(MSG_ScratchBuffer,ftStandard);

    /* PREPARE_CONTROL_FIELD() */
    MSG_ScratchBuffer->msg[0] |= (uint8)0x30;        /* fixed one bit + repeated. */
    MSG_ScratchBuffer->msg[0] &= (~(uint8)3);  /* clear two LSBs. */
    /**/

    chk=CalculateChecksum(MSG_ScratchBuffer);

    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
    DBG_DUMP(MSG_ScratchBuffer);
}

static void Disp_L_PollDataReq(void)
{
    /* todo: Implement!!! */
    MSG_SetFrameType(MSG_ScratchBuffer,ftPolling);
}

uint8 CalculateChecksum(PMSG_Buffer ptr)
{
    uint8 chk=(uint8)0xff;
    uint8 i;

    for (i=(uint8)0;i<ptr->len;i++) {
        chk^=ptr->msg[i];
    }

    return chk;
}

