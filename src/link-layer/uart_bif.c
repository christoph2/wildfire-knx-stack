/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
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
 *
 * Common Services are supported by every implementation, i.e. TPUART1, TPUART2 and NCN5120.
 *
 */

#include "link-layer/uart_bif.h"
#include "knx_debug.h"
#include "knx_et.h"
#include "knx_ffi.h"
#include "knx_timer.h"
#include "knx_disp.h"
#include "knx_messaging.h"
#include "port/port_serial.h"

#include <stdio.h>

#define KNX_LL_BUF_SIZE     (0xff)

/*
 *  Local Defines.
 */

/*
** Offsets into KNX Frame.
*/
#define OFFS_CTRL                   ((uint8_t)0)
#define OFFS_SOURCE_ADDR_H          ((uint8_t)1)
#define OFFS_SOURCE_ADDR_L          ((uint8_t)2)
#define OFFS_DEST_ADDR_H            ((uint8_t)3)
#define OFFS_DEST_ADDR_L            ((uint8_t)4)
#define OFFS_NPCI                   ((uint8_t)5)
#define OFFS_TPCI                   ((uint8_t)6)
#define OFFS_APCI                   ((uint8_t)7)

#define ACK_NACK                    ((uint8_t)4)
#define ACK_BUSY                    ((uint8_t)2)
#define ACK_ADDRESSED               ((uint8_t)1)

#define ADDRESS_TYPE_INDIVIDUAL     ((uint8_t) 0x00)
#define ADDRESS_TYPE_MULTICAST      ((uint8_t) 0x80)

/*
** Local Function-like Macros.
*/
#define KNX_LL_DESTINATION_ADDRESS()    Utl_Ntohs(KNX_CAST_ELEMENT(KnxLL_Buffer, OFFS_DEST_ADDR_H, uint16_t))
#define KNX_LL_ADDRESS_TYPE()           (KnxLL_Buffer[OFFS_NPCI] & 0x80)

/*!
 *
 *  Local Types.
 *
 */

typedef enum tagKnxLL_StateType {
    KNX_LL_STATE_IDLE,
    KNX_LL_STATE_RECEIVING,
    KNX_LL_STATE_SENDING,
    KNX_LL_STATE_AWAITING_RESPONSE_LOCAL,
    KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION,
    KNX_LL_STATE_AWAITING_RECEIPTION,
    KNX_LL_STATE_TIMED_OUT
} KnxLL_StateType;

typedef enum tagKnxLL_ReceiverStageType {
  KNX_LL_RECEIVER_STAGE_HEADER,
  KNX_LL_RECEIVER_STAGE_TRAILER
} KnxLL_ReceiverStageType;

typedef enum tagKnxLL_EventType {
    KNX_LL_EVENT_REQUEST,
    KNX_LL_EVENT_INDICATION,
    KNX_LL_EVENT_TIMEOUT,
} KnxLL_EventType;

typedef struct tagKnxLL_ExpectationType {
    uint8_t ExpectedByteCount;
    uint8_t ExpectedService;
    uint8_t ExpectedMask;
} KnxLL_ExpectationType;


#if 0
typedef struct tagKnxLL_LayerStateType {
    #if 0
 KnxLL_StateType KnxLL_State;
 uint8_t KnxLL_SequenceNo;

 uint8_t KnxLL_Buffer[KNX_LL_BUF_SIZE];

 KnxLL_ExpectationType KnxLL_Expectation = {0};
 uint8_t KnxLL_ReceiverIndex;
 KnxLL_ReceiverStageType KnxLL_ReceiverStage;
 uint8_t KnxLL_RunningFCB;
 KnxLL_LocalConfirmationType KnxLL_LocalConfirmation;
    #endif

} KnxLL_LayerStateType;
#endif

typedef enum tagKnxLL_LocalConfirmationType {
    KNX_LL_CONF_NEGATIVE,
    KNX_LL_CONF_POSITIVE
} KnxLL_LocalConfirmationType;


/*!
*
*  Local Function Prototypes.
*
*/
STATIC boolean KnxLL_InternalCommand(uint8_t const * frame, uint8_t length, KnxLL_StateType desiredState);
STATIC boolean KnxLL_InternalCommandUnconfirmed(uint8_t const * frame, uint8_t length);
STATIC boolean KnxLL_InternalCommandConfirmed(uint8_t const * frame, uint8_t length);
STATIC uint8_t KnxLL_Checksum(uint8_t const * frame, uint8_t length);
STATIC void KnxLL_Expect(uint8_t service, uint8_t mask, uint8_t byteCount);
STATIC void Disp_L_Data_Req(void);
STATIC void Disp_L_PollData_Req(void);
STATIC void KnxLL_DataStandard_Ind(uint8_t const * frame);
STATIC void KnxLl_Data_Con(Knx_StatusType status);


/*
 * Local Constants.
 */
STATIC const Knx_LayerServiceFunctionType KnxLl_Services[] = {
    /*      Service                     Handler                 */
    /*      ====================================================*/
    /*      L_DATA_REQ              */ Disp_L_Data_Req,
    /*      L_POLL_DATA_REQ         */ Disp_L_PollData_Req,
    /*      ====================================================*/
};

STATIC const Knx_LayerServicesType KnxLl_ServiceTable[] = {
    { KNX_LL_SERVICES, SIZEOF_ARRAY(KnxLl_Services), KnxLl_Services }
};

/*!
 *
 *  Local Variables.
 *
 */

STATIC KnxLL_StateType KnxLL_State;
STATIC uint8_t KnxLL_SequenceNo;

STATIC uint8_t KnxLL_Buffer[KNX_LL_BUF_SIZE];

STATIC KnxLL_ExpectationType KnxLL_Expectation = { 0 };
STATIC uint8_t KnxLL_ReceiverIndex;
STATIC KnxLL_ReceiverStageType KnxLL_ReceiverStage;
STATIC uint8_t KnxLL_RunningFCB;
STATIC KnxLL_LocalConfirmationType KnxLL_LocalConfirmation;

STATIC boolean KnxLL_Repeated = FALSE;


KNX_IMPLEMENT_MODULE_STATE_VAR(UART_BIF);

/*!
 *
 *  Global Functions.
 *
 */


/**
 * Initialises the Data-Link-Layer.
 *
 */
void KnxLL_Init(void)
{
    KNX_MODULE_INITIALIZE(UART_BIF);

    KnxLL_State = KNX_LL_STATE_IDLE;
    KnxLL_SequenceNo = (uint8_t)0x00;
    KnxLL_ReceiverIndex = (uint8_t)0x00;
    KnxLL_RunningFCB = (uint8_t)0x00;
    KnxLL_LocalConfirmation = KNX_LL_CONF_NEGATIVE;
    KnxLL_Repeated = FALSE;
    Utl_MemSet(&KnxLL_Expectation, (uint8_t)'\x00', (uint16_t)sizeof(KnxLL_ExpectationType));
}


/**
 *  Gets called from interrupt-context if a timeout occured.
 *
 */
void KnxLL_TimeoutCB(void)
{
    DBG_PRINTLN("L2 TIMEOUT.");
    KnxLL_State = KNX_LL_STATE_TIMED_OUT;
    KnxLL_FeedReceiver((uint8_t)0x00);
}


boolean KnxLL_IsAddressed(uint8_t daf, uint16_t address)
{
    if (daf == ADDRESS_TYPE_MULTICAST) {

    } else if (daf == ADDRESS_TYPE_INDIVIDUAL) {

    }

    return TRUE;
}

// This constitutes the link-layer statemachine.
void KnxLL_FeedReceiver(uint8_t octet)
{
    if (KnxLL_State == KNX_LL_STATE_AWAITING_RESPONSE_LOCAL) {
        if (KnxLL_Expectation.ExpectedService == (octet & KnxLL_Expectation.ExpectedMask)) {
            if (KnxLL_Expectation.ExpectedByteCount == 1) {
                //printf("fo: 0x%02x\n", octet);
                TMR_STOP_DL_TIMER();
                KnxLL_State = KNX_LL_STATE_IDLE;
                switch (KnxLL_Expectation.ExpectedService) {
                case U_RESET_IND:
                    DBG_PRINTLN("U_Reset_Res");
                    break;
                case U_STATE_IND:
                    DBG_PRINTLN("U_State_Res");
                    break;
                default:
                    printf("Unexpected octet: 0x%02x\n", octet);
                    break;
                }
            }
        }
        // KnxLL_Buffer[0] = octet;
    } else if (KnxLL_State == KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION) {
        TMR_STOP_DL_TIMER();
        KnxLL_Buffer[KnxLL_ReceiverIndex] = octet;
        KnxLL_ReceiverIndex++;
        TMR_START_DL_TIMER();
        if (KnxLL_ReceiverIndex == 0x01) {
            if ((octet & U_STATE_IND) == U_STATE_IND) {
                DBG_PRINTLN("Receiver Error!\n");
            }
            else if ((octet & L_DATA_CON) == L_DATA_CON) {
                TMR_STOP_DL_TIMER();
                if ((octet & 0x80) == 0x80) {
                    KnxLL_LocalConfirmation = KNX_LL_CONF_POSITIVE;
                } else {
                    KnxLL_LocalConfirmation = KNX_LL_CONF_NEGATIVE;
                }
                //KNX_CALLBACK_L_CON((octet & 0x80) == 0x80);
                KnxLl_Data_Con( ((octet & 0x80) == 0x80) ? KNX_E_OK : KNX_E_NOT_OK );
                KnxLL_State = KNX_LL_STATE_IDLE;
            }
            else if ((octet & 0x10) == 0x10) {    /* Weak check. */
                //KnxLL_Repeated = (octet & 0x20) == 0x00;
            }
        }
        else if (KnxLL_ReceiverIndex == KnxLL_Expectation.ExpectedByteCount) {
            //printf("Finished. [0x%02x]\n", octet);
            KnxLL_ReceiverIndex = (uint8_t)0x00;
        }
    } else if (KnxLL_State == KNX_LL_STATE_AWAITING_RECEIPTION) {
        TMR_STOP_DL_TIMER();
        KnxLL_ReceiverIndex++;
        KnxLL_Buffer[KnxLL_ReceiverIndex] = octet;
        KnxLL_RunningFCB ^= octet;
        //printf("R: %02x ", octet);
        TMR_START_DL_TIMER();
        if (KnxLL_ReceiverStage == KNX_LL_RECEIVER_STAGE_HEADER) {
            if (KnxLL_ReceiverIndex == OFFS_NPCI) {
                KnxLL_Expectation.ExpectedByteCount = (uint8_t)((octet & 0x0f) + 2);
                KnxLL_ReceiverStage = KNX_LL_RECEIVER_STAGE_TRAILER;
            } else if (KnxLL_ReceiverIndex == OFFS_DEST_ADDR_H) {
                if (KnxLL_IsAddressed(KNX_LL_ADDRESS_TYPE(), KNX_LL_DESTINATION_ADDRESS())) {
                    U_Ackn_req(ACK_ADDRESSED);
                      //printf("\nDA: 0x%04X\n", KNX_LL_DESTINATION_ADDRESS());
                }
            }
        } else if (KnxLL_ReceiverStage == KNX_LL_RECEIVER_STAGE_TRAILER) {
            if (KnxLL_ReceiverIndex == KnxLL_Expectation.ExpectedByteCount + OFFS_NPCI) {
                KnxLL_State = KNX_LL_STATE_IDLE;
                /* TODO: Check FCB. */
                /* TODO: Callback. */
                KnxLL_DataStandard_Ind(KnxLL_Buffer);
            }
        }

    } else if (KnxLL_State == KNX_LL_STATE_TIMED_OUT) {
        // TODO: Callback/Callout.
        KnxLL_State = KNX_LL_STATE_IDLE;
    } else if (KnxLL_State == KNX_LL_STATE_IDLE) {
        if ((octet & U_STATE_IND) == U_STATE_IND) {
            printf("U_State_Ind [0x%02x]\n", octet);
        } else if ((octet & 0x7f) == L_DATA_CON) {
            DBG_PRINTLN("L_Data_Con");    // ???
        } else if ((octet & 0xd3) == L_DATA_EXTENDED_IND) { // #if defined()
            DBG_PRINTLN("L_DataExtended_Ind");
        } else if ((octet & 0xd3) == L_DATA_STANDARD_IND)  {
            //DBG_PRINTLN("L_DataStandard_Ind");
            //printf("CTRL[%02x]\n", octet);
            KnxLL_State = KNX_LL_STATE_AWAITING_RECEIPTION; /* TODO: Distiguish Standard/Extendend Frames */
            KnxLL_ReceiverStage = KNX_LL_RECEIVER_STAGE_HEADER;
            KnxLL_ReceiverIndex = (uint8_t)0;
            KnxLL_RunningFCB = (uint8_t)0xff ^ octet;
            KnxLL_Buffer[KnxLL_ReceiverIndex] = octet;
            KnxLL_Repeated = (octet & 0x20) == 0x00;
            TMR_START_DL_TIMER();
        } else if (octet == U_RESET_IND) {
            DBG_PRINTLN("U_Reset_Ind");
        } else if (octet == L_POLL_DATA_IND) { // #if defined()
            DBG_PRINTLN("L_PollData_Ind");
        }
#if 0
    MASK: 0x00
        U_ProductID.response == > xxxxxxxx[0x00]
    MASK: 0x07
          State.response / indication == > xxxxx111[0x07]
      MASK: 0x7f
            L_DATA.confirm == > x0001011[0x0b]
        MASK: 0xd3
              L_EXT_DATA.req == > 00x1xx00[0x10]
              L_DATA.req == > 10x1xx00[0x90]
          MASK: 0xff
                Reset - Indication == > 00000011[0x03]
                NotAcknowledge frame == > 00001100[0x0c]
                Busy frame == > 11000000[0xc0]
                Acknowledge frame == > 11001100[0xcc]
                L_POLLDATA.req == > 11110000[0xf0]
#endif

    } else {
       /* Ignore anything else. */
    }

}

void KnxLL_Task(void)
{
    KnxDisp_DispatchLayer(TASK_LL_ID, KnxLl_ServiceTable);
}

STATIC KnxMsg_Buffer * txBuffer;

/*
**
**  Services from Network-Layer.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_L_Data_Req(void)
#else
STATIC void Disp_L_Data_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t chk;

    KnxMsg_SetFrameType(KnxMsg_ScratchBufferPtr, KNX_FRAME_STANDARD);

    /* PREPARE_CONTROL_FIELD() */
    KnxMsg_ScratchBufferPtr->msg[0] |= (uint8_t)0x30;   /* fixed one bit + repeated. */
    KnxMsg_ScratchBufferPtr->msg[0] &= (~(uint8_t)3);   /* clear two LSBs. */
    /**/

    chk = KnxLL_Checksum(KnxMsg_ScratchBufferPtr->msg, KnxMsg_ScratchBufferPtr->len);
    
    DBG_PRINTLN("");
    DBG_PRINT("Disp_L_Data_Req: ");
    Dbg_DumpHex(KnxMsg_ScratchBufferPtr->msg, KnxMsg_ScratchBufferPtr->len);

    KnxLL_WriteFrame(KnxMsg_ScratchBufferPtr->msg, KnxMsg_ScratchBufferPtr->len);
    (void)KnxMsg_ReleaseBuffer(KnxMsg_ScratchBufferPtr);
}



#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) Disp_L_PollData_Req(void)
#else
STATIC void Disp_L_PollData_Req(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: Implement!!! */
    KnxMsg_SetFrameType(KnxMsg_ScratchBufferPtr, KNX_FRAME_POLLING);
}


STATIC void KnxLl_Data_Con(Knx_StatusType status)
{
    //KnxMsg_Buffer * txBuffer;
    uint16_t length;

    if (KnxMsg_AllocateBuffer(&txBuffer) == KNX_E_OK) {
        txBuffer->service = KNX_SERVICE_L_DATA_CON;
        txBuffer->status = status;

        txBuffer->len = length = (KnxLL_Buffer[5] & (uint8_t)0x0f) + (uint8_t)7;
        Utl_MemCopy((void *)txBuffer->msg, (void *)KnxLL_Buffer, length);

        (void)KnxMsg_Post(txBuffer);
    } else {
        ASSERT(FALSE);
    }
}

boolean KnxLL_IsBusy(void)
{
    boolean result;

    PORT_LOCK_TASK_LEVEL();
    result = (KnxLL_State != KNX_LL_STATE_IDLE);
    PORT_UNLOCK_TASK_LEVEL();
    return result;
}


void KnxLL_DataStandard_Ind(uint8_t const * frame)
{
    KnxMsg_Buffer * pBuffer;
    uint8_t length = (uint8_t)0x00;

    if (KnxLL_Repeated) {
        return;     /* Don't route duplicates for now. */
    }

    (void)KnxMsg_AllocateBuffer(&pBuffer);

    if (pBuffer != (KnxMsg_Buffer *)NULL) {
        pBuffer->service = KNX_SERVICE_L_DATA_IND;
//        pBuffer->sap = tsap;
        pBuffer->len = length = (frame[5] & (uint8_t)0x0f) + (uint8_t)7;    // TODO: Refactor to function-like macro.

        Utl_MemCopy((void *)pBuffer->msg, (void *)frame, (uint16_t)length);
        (void)KnxMsg_Post(pBuffer);
    }

    DBG_PRINT("");
    DBG_PRINT("L_DataStandard_Ind: ");
    Dbg_DumpHex(frame, length);
}

/**
* Wait for availability of link-layer.
*
* NB: Services shall never be issued while link-layer is busy!
*/
void KnxLL_BusyWait(void)
{   
    while (KnxLL_IsBusy()) {
    };
}

boolean KnxLL_IsConfirmed(void)
{
    boolean result;

    PORT_LOCK_TASK_LEVEL();
    result = KnxLL_LocalConfirmation == KNX_LL_CONF_POSITIVE;
    PORT_UNLOCK_TASK_LEVEL();

    return result;
}

/*!
 *
 * Local Functions.
 *
 */

/**
 * Issues an internal Command, i.e. related to the interface IC, not EIB.
 *
 * @param frame Octets to be send.
 * @param length Number of octets.
 * @param desiredState Desired state to enter (in case of confirmed commands, we need to wait for an answer).
 *
 * @return boolean
 */
STATIC boolean KnxLL_InternalCommand(uint8_t const * frame, uint8_t length, KnxLL_StateType desiredState)
{
    boolean result;

    PORT_LOCK_TASK_LEVEL();
    if (KnxLL_State != KNX_LL_STATE_IDLE) {
        PORT_UNLOCK_TASK_LEVEL();
        return FALSE;
    }
    //KnxLL_State = KNX_LL_STATE_SENDING;
    KnxLL_State = desiredState;
    result = Port_Serial_Write(frame, (uint32_t)length);
    PORT_UNLOCK_TASK_LEVEL();
    TMR_START_DL_TIMER();
    //KnxLL_State = desiredState;
    return result;
}

STATIC boolean KnxLL_InternalCommandUnconfirmed(uint8_t const * frame, uint8_t length)
{
    return KnxLL_InternalCommand(frame, length, KNX_LL_STATE_IDLE);
}

STATIC boolean KnxLL_InternalCommandConfirmed(uint8_t const * frame, uint8_t length)
{
    return KnxLL_InternalCommand(frame, length, KNX_LL_STATE_AWAITING_RESPONSE_LOCAL);
}

STATIC void KnxLL_Expect(uint8_t service, uint8_t mask, uint8_t byteCount)
{
    KnxLL_Expectation.ExpectedService = service;
    KnxLL_Expectation.ExpectedMask = mask;
    KnxLL_Expectation.ExpectedByteCount = byteCount;
}

STATIC uint8_t KnxLL_Checksum(uint8_t const * frame, uint8_t length)
{
    uint8_t checksum = (uint8_t)0xff;
    uint8_t idx;

    for (idx = (uint8_t)0; idx < length; ++idx) {
        checksum ^= frame[idx];
    }

    return checksum;
}

void KnxLL_WriteFrame(uint8_t const * frame, uint8_t length)
{
    uint8_t idx;
    uint8_t checksum;
    char buffer[128];

    KNX_ASSERT_MODULE_IS_INITIALIZED(UART_BIF, KNX_SERVICE_LL_WRITE_FRAME);

    checksum = KnxLL_Checksum(frame, length);
    buffer[0] = U_L_DATASTART_REQ;
    buffer[1] = frame[0];
    for (idx = (uint8_t)2; idx < (length << 1); idx += (uint8_t)2) {
        buffer[idx] = U_L_DATACONT_REQ | (idx >> 1);
        buffer[idx + 1] = frame[idx >> 1];
    }
    buffer[idx] = U_L_DATAEND_REQ | (idx >> 1);
    buffer[idx + 1] = checksum;
    (void)Port_Serial_Write((uint8_t *)buffer, (uint32_t)(idx + (uint8_t)2));
    KnxLL_Expect((uint8_t)0x00, (uint8_t)0x00, length + (uint8_t)1);
    KnxLL_State = KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION;
    KnxLL_ReceiverIndex = (uint8_t)0x00;
    TMR_START_DL_TIMER();
}


/**
 *
 *  Local unconfirmed services.
 *
 */
void U_ActivateBusmon_req(void)
{
    DBG_PRINTLN("U_ActivateBusmon_req");
    KnxLL_Buffer[0] = U_BUSMON_REQ;
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)1);
}

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
void U_ActivateBusyMode_req(void)
{
    DBG_PRINTLN("U_ActivateBusyMode_req");
    KnxLL_Buffer[0] = U_SETBUSY_REQ;
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)1);
}

void U_ResetBusyMode_req(void)
{
    DBG_PRINTLN("U_ResetBusyMode_req");
    KnxLL_Buffer[0] = U_QUITBUSY_REQ;
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)1);
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void U_ActivateBusyMode_req(void)
{
    DBG_PRINTLN("U_ActivateBusyMode_req");
    KnxLL_Buffer[0] = U_ACTIVATEBUSYMODE_REQ;
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)1);
}


void U_ResetBusyMode_req(void)
{
    DBG_PRINTLN("U_ResetBusyMode_req");
    KnxLL_Buffer[0] = U_RESETBUSYMODE_REQ;
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)1);
}

void U_SetRepetition_req(uint8_t rst)
{
    DBG_PRINTLN("U_SetRepetition_req");
    KnxLL_Buffer[0] = U_MXRSTCNT_REQ;
    KnxLL_Buffer[1] = rst;
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)2);
}

void U_ActivateCRC_req(void)
{
    DBG_PRINTLN("U_ActivateCRC_req");
    KnxLL_Buffer[0] = U_ACTIVATECRC_REQ;
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)1);
}

void U_SetAddress_req(uint16_t address)
{
    DBG_PRINTLN("U_SetAddress_req");
    KnxLL_Buffer[0] = U_SETADDRESS_REQ;
    KnxLL_Buffer[1] = KNX_HIBYTE(address);
    KnxLL_Buffer[2] = KNX_LOBYTE(address);
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)3);
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
void U_SetRepetition_req(uint8_t rst)
{
    DBG_PRINTLN("U_SetRepetition_req");
    KnxLL_Buffer[0] = U_SETREPETITION_REQ;
    KnxLL_Buffer[1] = rst;
    KnxLL_Buffer[2] = (uint8_t)0x00; /* Dummy */
    KnxLL_Buffer[3] = (uint8_t)0x00; /* Dummy */
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)4);
}
#endif /* KNX_BUS_INTERFACE */

void U_Ackn_req(uint8_t what)
{

//    DBG_PRINTLN("U_Ackn_req");
    KnxLL_Buffer[0] = U_ACKN_REQ | (what & 0x07);
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)1);
}

/**
 *
 *  Local confirmed services.
 *
 */

void U_Reset_req(void)
{
    DBG_PRINTLN("U_Reset_req");
    KnxLL_Buffer[0] = U_RESET_REQ;
    (void)KnxLL_InternalCommandConfirmed(KnxLL_Buffer, (uint8_t)1);
    KnxLL_Expect(U_RESET_IND, (uint8_t)0xff, (uint8_t)1);
}

void U_State_req(void)
{
    DBG_PRINTLN("U_State_req");
    KnxLL_Buffer[0] = U_STATE_REQ;
    (void)KnxLL_InternalCommandConfirmed(KnxLL_Buffer, (uint8_t)1);
    KnxLL_Expect(U_STATE_IND, (uint8_t)0x07, (uint8_t)1);
}

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void U_ProductID_req(void)
{
    DBG_PRINTLN("U_ProductID_req");
    KnxLL_Buffer[0] = U_PRODUCTID_REQUEST;
    (void)KnxLL_InternalCommandConfirmed(KnxLL_Buffer, (uint8_t)1);
    KnxLL_Expect((uint8_t)0x00, (uint8_t)0x00, (uint8_t)1);    /* Expect any single byte value. */
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
void U_SetAddress_req(uint16_t address) /* NB: TPUART2's SetAddress is unconfirmed! */
{
    DBG_PRINTLN("U_SetAddress_req");
    KnxLL_Buffer[0] = U_SETADDRESS_REQ;
    KnxLL_Buffer[1] = KNX_HIBYTE(address);
    KnxLL_Buffer[2] = KNX_LOBYTE(address);
    KnxLL_Buffer[3] = (uint8_t)0x00; /* Dummy */
    (void)KnxLL_InternalCommandConfirmed(KnxLL_Buffer, (uint8_t)4);
    KnxLL_Expect(U_CONFIGURE_IND, (uint8_t)0x83, (uint8_t)1);
}
#endif /* KNX_BUS_INTERFACE */

/**
 *  Transmission services.
 */


#if 0
void U_SystemState_req(void)
{
    DBG_PRINTLN("U_SystemState_req");
    KnxLL_Buffer[0] = U_SYSTEMSTAT_REQ;
    (void)KnxLL_InternalCommandConfirmed(KnxLL_Buffer, (uint8_t)2);
    KnxLL_Expect(U_SYSTEMSTAT_IND, (uint8_t)0xff, (uint8_t)2);
}

void U_StopMode_req(void)
{
    DBG_PRINTLN("U_StopMode_req");
    KnxLL_Buffer[0] = U_STOPMODE_REQ;
    (void)KnxLL_InternalCommandConfirmed(KnxLL_Buffer, (uint8_t)1);
    KnxLL_Expect(U_STOPMODE_IND, (uint8_t)0xff, (uint8_t)1);
}

void U_ExitStopMode_req(void)
{
    DBG_PRINTLN("U_ExitStopMode_req");
    KnxLL_Buffer[0] = U_EXITSTOPMODE_REQ;
    (void)KnxLL_InternalCommandConfirmed(KnxLL_Buffer, (uint8_t)1);
    KnxLL_Expect(U_RESET_IND, (uint8_t)0xff, (uint8_t)1);
}

void U_Configure_req(uint8_t conf)
{
    DBG_PRINTLN("U_Configure_req");
    KnxLL_Buffer[0] = U_CONFIGURE_REQ | (conf & 0x07);
    (void)KnxLL_InternalCommandConfirmed(KnxLL_Buffer, (uint8_t)1);
    KnxLL_Expect(U_CONFIGURE_IND, (uint8_t)0xff, (uint8_t)1);
}

void U_IntRegWr_req(uint8_t addr, uint8_t value)
{
    DBG_PRINTLN("U_IntRegWr_req");
    KnxLL_Buffer[0] = U_INTREGWR_REQ | (addr & 0x03);
    KnxLL_Buffer[1] = value;
    (void)KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, (uint8_t)2);
}

void U_IntRegRd_req(uint8_t addr)
{
    DBG_PRINTLN("U_IntRegRd_req");
    KnxLL_Buffer[0] = U_INTREGRD_REQ;
    (void)KnxLL_InternalCommandConfirmed(KnxLL_Buffer, (uint8_t)1);
    KnxLL_Expect((uint8_t)0x00, (uint8_t)0x00, (uint8_t)1);
}
#endif

