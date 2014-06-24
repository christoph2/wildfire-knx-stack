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

#include "link-layer/knx_tpuart.h"
#include "knx_messaging.h"

/*
** Local defines.
*/
#define ACK_INFORMATION ((uint8_t)0x10)
#define ACK_ADDRESSED   ((uint8_t)0x01)
#define ACK_BUSY        ((uint8_t)0x02)
#define ACK_NACK        ((uint8_t)0x04)

/*
** Local functions prototypes.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_L_DataReq(void), Disp_L_PollDataReq(void);


FUNC(uint8_t, KSTACK_CODE)    CalculateChecksum(KnxMSG_BufferPtr ptr);
FUNC(void, KSTACK_CODE)    PH_AckInformation_req(uint8_t flags);
FUNC(void, KSTACK_CODE)    DBG_DUMP(KnxMSG_BufferPtr ptr);
FUNC(void, KSTACK_CODE)    decode(uint8_t b);
FUNC(void, KSTACK_CODE)    StartTimeout(void);
FUNC(void, KSTACK_CODE)    StopTimeout(void);
FUNC(void, KSTACK_CODE)    OnTimeout(void);        /* Callback. */
#else
STATIC void Disp_L_DataReq(void), Disp_L_PollDataReq(void);
uint8_t       CalculateChecksum(KnxMSG_BufferPtr ptr);
void        PH_AckInformation_req(uint8_t flags);
void        DBG_DUMP(KnxMSG_BufferPtr ptr);
void        decode(uint8_t b);
void        StartTimeout(void);
void        StopTimeout(void);
void        OnTimeout(void);    /* Callback. */


#endif /* KSTACK_MEMORY_MAPPING */

/*
** Local constants.
*/
STATIC const Knx_LayerServiceFunctionType LL_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      L_DATA_REQ              */ Disp_L_DataReq,
/*      L_POLL_DATA_REQ         */ Disp_L_PollDataReq,
/*      ====================================================*/
};

STATIC const Knx_LayerServicesType LL_ServiceTable[] = {
    {KNX_LL_SERVICES, 2, LL_Services}
};

/*
**  "The local layer-2 passes an L_Data.con primitive to the local user that indicates either a correct or
**  erroneous data transfer. Depending if an L2-acknowledgement is requested or not, this confirmation is
**  related to the reception of the L2-acknowledgement, or only to the transmission of the frame on the
**  medium."
*/

/* const uint8_t T0[]={0xbc,0x11,0x64,0x09,0x01,0xe3,0x00,0x80,0x0d,0x14,0x44}; */
/*      ==> EIS5-Telegramm (26.00) */
/*      von     1.01.100 nach 1/1/001 */

/* const uint8_t T[]={0xbc,0x11,0x01,0x49,0x01,0xe2,0x00,0x80,0x5f,0x26}; */
/*      (01.01.001==>09/1/001 Val: 103) */

/*uint8_t T1[]={0xbc,0x11,0x64,0x11,0x01,0x61,0x43,0x80,0x84}; */
/* A_Restart. */

/*uint8_t T1[]={0xb0,0x11,0x64,0x00,0x00,0xE3,0x00,0xC0,0xaf,0xfe,0x48}; */
/* A_PhysAddr_Write(0xAFFE). */

/*uint8_t T1[]={0xb0,0x11,0x64,0x00,0x00,0xE1,0x01,0x00,0xda}; */
/* A_PhysAddr_Read(). */

/*uint8_t T1[]={0xb0,0x11,0x64,0x11,0x01,0x65,0x03,0xd5,0x01,0x05,0x10,0x00,0x8d}; */
/* A_PropertyValueRead(). */

/* const uint8_t T1[]={0xb0,0x11,0x64,0x11,0x01,0x64,0x03,0xd8,0x00,0x00,0x06,0x93}; */
/* A_PropertyDescriptionRead(). */

const uint8_t T0[] = {
    (uint8_t)0xbc, (uint8_t)0x11, (uint8_t)0x64, (uint8_t)0x11, (uint8_t)0x01, (uint8_t)0x60, (uint8_t)0x80, (uint8_t)0xc6
};
/* T_Connect (01.01.100 ==> 01.01.001). */

const uint8_t T1[] = {
    (uint8_t)0xbc, (uint8_t)0x11, (uint8_t)0x64, (uint8_t)0x11, (uint8_t)0x01, (uint8_t)0x67, (uint8_t)0x42, (uint8_t)0x84,
    (uint8_t)0x01, (uint8_t)0x80, (uint8_t)0xaa, (uint8_t)0xbb, (uint8_t)0xcc, (uint8_t)0xdd, (uint8_t)0x06
};
/* A_MemoryWrite */

const uint8_t T2[] = {
    (uint8_t)0xbc, (uint8_t)0x11, (uint8_t)0x64, (uint8_t)0x11, (uint8_t)0x01, (uint8_t)0x60, (uint8_t)0x81, (uint8_t)0xc7
};
/* T_Disconnect (01.01.100 ==> 01.01.001). */

/*
** Local variables.
*/
TPUART_RCV_STATE    rcvState;
TPUART_RCV_SERVICE  rcvService;

uint8_t           RcvLen;
uint8_t           RcvIdx;
uint8_t           Checksum;
uint8_t           tsap;
Knx_AddressType dest_addr;
boolean         addressed;

uint8_t TpuartRcvBuf[BUF_LEN];

KnxMSG_BufferPtr pBuffer;

uint8_t AckService;

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxLL_Init(void)
#else
void KnxLL_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    rcvState   = TPSR_WAIT_RESET_IND;
    RcvLen     = RcvIdx = (uint8_t)0x00;
    AckService = (uint8_t)0x00;
    rcvService = SERVICE_NONE;
    Utl_MemSet(TpuartRcvBuf, 0, BUF_LEN);
    rcvState = TPSR_WAIT;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) TPTest(void)
#else
void TPTest(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    int         b, len;
    static int  cnt;

    switch (cnt) {
        case (uint8_t)0:
            len = sizeof(T0);

            for (b = (uint8_t)0; b < len; b++) {
                decode(T0[b]);
            }

            cnt++;
            break;
        case (uint8_t)1:
            len = sizeof(T1);

            for (b = (uint8_t)0; b < len; b++) {
                decode(T1[b]);
            }

            cnt++;
            break;
        case (uint8_t)2:
            len = sizeof(T2);

            for (b = (uint8_t)0; b < len; b++) {
                decode(T2[b]);
            }

            cnt++;
            break;
        default:
            cnt = (uint8_t)0;
            break;
    }

/*
        len=sizeof(T);

        for (b=0;b<len;b++) {
                decode(T[b]);
        }
 */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) decode(uint8_t b)
#else
void decode(uint8_t b)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t state;

    boolean stop;

    TpuartRcvBuf[RcvIdx++] = b;

    switch (rcvState) {
        case TPSR_WAIT:

/*
   ////////////////////////////////////////
   ////////////////////////////////////////
   ////////////////////////////////////////
 */
            if ((b & (uint8_t)0x13) == DATA_SERVICE_MASK) {
                StartTimeout();

/*                      prio=(b & PRIO_MASK)>>2; */

/*                      repeated=((~b) & REPEATED_MASK)>>5;     // 1 ==> not repeated. */

/*                      rcvService=SERVICE_DATA; */

                if ((b & (uint8_t)0xd0) == L_DATA_IND_MASK) {
                    RcvLen     = (uint8_t)5;
                    tsap       = (uint8_t)0x00;      /* 0xFE */
                    Checksum   = b;
                    rcvState   = TPSR_DATA_CONT1;
                } else if ((b & (uint8_t)0xd0) == L_LONG_DATA_IND_MASK) {
/*                              rcvState=TPSR_LONG_DATA; */
                    rcvState = TPSR_WAIT;
                } else if ((b & (uint8_t)0xff) == L_POLL_DATA_IND_MASK) {
                    rcvState = TPSR_POLL_DATA;
                } else {                    /* Errornous Byte. */
                    rcvState   = TPSR_WAIT; /* wait for valid Service. */
                    rcvService = SERVICE_NONE;
#if !defined(__HIWARE__)
/*    printf("0x%02x\tERROR\n",b); */
#endif
                }
            } else if ((b == IACK_IND) || (b == INACK_IND) || (b == IBUSY_IND)) {
                rcvState   = TPSR_WAIT;         /* wait for next Service. */
                rcvService = SERVICE_IACK;
                RcvIdx     = (uint8_t)0;
                AckService = b;
            } else if ((b & (uint8_t)0x03) == (uint8_t)0x03) {
                rcvState   = TPSR_WAIT;         /* wait for next Service. */
                rcvService = SERVICE_CONTROL;
                RcvIdx     = (uint8_t)0;

                if (b == RESET_IND) {

                } else if ((b & (uint8_t)0x7f) == (uint8_t)0x0b) {

                } else if ((b & (uint8_t)0x07) == (uint8_t)0x07) {

                    state = (b & (uint8_t)0xf8) >> 3;
                } else {                /* Errornous Byte. */
                    rcvService = SERVICE_NONE;
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
            Checksum ^= b;

            if ((--RcvLen) == (uint8_t)0x00) {
                /* if (!PassthroughEveryTelegramm()) // RouteEvery */

                dest_addr = btohs(*(uint16_t *)&TpuartRcvBuf[3]);

                if ((TpuartRcvBuf[5] & atMULTICAST)) {
                    addressed = KnxADR_IsAddressed(dest_addr, &tsap);
                } else {
                    addressed = KnxADR_IsOwnPhysicalAddr(dest_addr);
                }

                if (addressed) {
                    PH_AckInformation_req(ACK_ADDRESSED);
                }

                RcvLen     = (b & (uint8_t)0x0f) + 2;        /* todo: Limit. */
                rcvState   = TPSR_DATA_CONT2;
            }

            break;
        case TPSR_DATA_CONT2:

            if ((--RcvLen) == (uint8_t)0x00) {           /* O.K., complete Telegram receeived. */
                Checksum ^= (uint8_t)0xff;

                if ((Checksum == TpuartRcvBuf[RcvIdx - (uint8_t)1])) {             /* Checksum valid? */
                    if (addressed) {
                        pBuffer = KnxMSG_AllocateBuffer();

                        if (pBuffer != (KnxMSG_BufferPtr)NULL) {
                            pBuffer->service   = L_DATA_IND;
                            pBuffer->sap       = tsap;
                            pBuffer->len       = RcvLen = (TpuartRcvBuf[5] & (uint8_t)0x0f) + (uint8_t)7;

                            Utl_MemCopy((void *)pBuffer->msg, (void *)TpuartRcvBuf, RcvLen);
                            (void)KnxMSG_Post(pBuffer);
                        } else {
                            stop = TRUE;
                            /* todo: Error-Handling. */
                        }
                    }

                } else {
#if !defined(__HIWARE__)
/*    printf("\n*** CHECKSUM-ERROR ***\n"); */
#endif
                }

                rcvService = SERVICE_DATA;
                rcvState   = TPSR_WAIT;
                RcvIdx     = (uint8_t)0;
            } else {
                Checksum ^= b;
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


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) StartTimeout(void)
#else
void StartTimeout(void)
#endif /* KSTACK_MEMORY_MAPPING */
{

}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) StopTimeout(void)
#else
void StopTimeout(void)
#endif /* KSTACK_MEMORY_MAPPING */
{

}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) OnTimeout(void)
#else
void OnTimeout(void)            /* Callback. */
#endif /* KSTACK_MEMORY_MAPPING */
{
/*      Differenzieren: z.B.: 'TPSR_WAIT_RESET_IND'. */
    rcvState = TPSR_WAIT;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) DBG_DUMP(KnxMSG_BufferPtr ptr)
#else
void DBG_DUMP(KnxMSG_BufferPtr ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t i /*,chk*/;

#if     !defined(__HIWARE__)

    for (i = (uint8_t)0; i < ptr->len; i++) {

/*        printf("%02X ",ptr->msg[i]);  */

        if (i == (uint8_t)6) {
/*            printf("["); */
        }
    }

/*    printf("]\t%02X\n",CalculateChecksum(ptr)); */
#endif
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) PH_AckInformation_req(uint8_t flags)
#else
void PH_AckInformation_req(uint8_t flags)
#endif /* KSTACK_MEMORY_MAPPING */
{
/* PutSCI(ACK_INFORMATION | flags); */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxLL_Task(void)
#else
void KnxLL_Task(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxDisp_DispatchLayer(TASK_LL_ID, LL_ServiceTable);
}


/*
**
**  Services from Network-Layer.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_L_DataReq(void)
#else
STATIC void Disp_L_DataReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t chk;

    KnxMSG_SetFrameType(KnxMSG_ScratchBufferPtr, ftStandard);

    /* PREPARE_CONTROL_FIELD() */
    KnxMSG_ScratchBufferPtr->msg[0]   |= (uint8_t)0x30;   /* fixed one bit + repeated. */
    KnxMSG_ScratchBufferPtr->msg[0]   &= (~(uint8_t)3);   /* clear two LSBs. */
    /**/

    chk = CalculateChecksum(KnxMSG_ScratchBufferPtr);

    KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
    DBG_DUMP(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_L_PollDataReq(void)
#else
STATIC void Disp_L_PollDataReq(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!!! */
    KnxMSG_SetFrameType(KnxMSG_ScratchBufferPtr, ftPolling);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) CalculateChecksum(KnxMSG_BufferPtr ptr)
#else
uint8_t CalculateChecksum(KnxMSG_BufferPtr ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   chk = (uint8_t)0xff;
    uint8_t   i;

    for (i = (uint8_t)0; i < ptr->len; i++) {
        chk ^= ptr->msg[i];
    }

    return chk;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

