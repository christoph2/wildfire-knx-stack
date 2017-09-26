/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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

#include <cstdint>

#include "link-layer/uart_bif.hpp"
#include "knx_debug.hpp"
#include "knx_et.hpp"
#include "knx_timer.hpp"
#include "knx_disp.hpp"
#include "knx_messaging.hpp"
#include "knx_msgif.hpp"
#include "port/port_serial.hpp"

namespace knx {


/*
 *  Local Defines.
 */

/*
** Offsets into KNX Frame.
*/
constexpr uint8_t OFFS_CTRL                   = 0;
constexpr uint8_t OFFS_SOURCE_ADDR_H          = 1;
constexpr uint8_t OFFS_SOURCE_ADDR_L          = 2;
constexpr uint8_t OFFS_DEST_ADDR_H            = 3;
constexpr uint8_t OFFS_DEST_ADDR_L            = 4;
constexpr uint8_t OFFS_NPCI                   = 5;
constexpr uint8_t OFFS_TPCI                   = 6;
constexpr uint8_t OFFS_APCI                   = 7;

constexpr uint8_t ACK_NACK                    = 4;
constexpr uint8_t ACK_BUSY                    = 2;
constexpr uint8_t ACK_ADDRESSED               = 1;

constexpr uint8_t ADDRESS_TYPE_INDIVIDUAL      = 0x00;
constexpr uint8_t ADDRESS_TYPE_MULTICAST       = 0x80;

/*
** Local Function-like Macros.
*/
#define KNX_LL_DESTINATION_ADDRESS()    Utl_Ntohs(KNX_CAST_ELEMENT(buffer_, OFFS_DEST_ADDR_H, uint16_t))
#define KNX_LL_ADDRESS_TYPE()           (buffer_[OFFS_NPCI] & 0x80)

/*!
 *
 *  Local Types.
 *
 */
typedef enum tagKnxLL_EventType {
    KNX_LL_EVENT_REQUEST,
    KNX_LL_EVENT_INDICATION,
    KNX_LL_EVENT_TIMEOUT,
} KnxLL_EventType;


/*
 * Local Constants.
 */
const Knx_LayerServiceFunctionType LinkLayer::KnxLl_Services[] = {
    /*      Service                     Handler                 */
    /*      ====================================================*/
    /*      L_DATA_REQ              */ reinterpret_cast<Knx_LayerServiceFunctionType>(&disp_L_Data_Req),
    /*      L_POLL_DATA_REQ         */ reinterpret_cast<Knx_LayerServiceFunctionType>(&disp_L_PollData_Req),
    /*      ====================================================*/
};

const Knx_LayerServicesType LinkLayer::KnxLl_ServiceTable[] = {
    { KNX_LL_SERVICES, SIZEOF_ARRAY(KnxLl_Services), KnxLl_Services }
};

/**!
  *
  * Dynamic Callbacks.
  *
  */
#if KNX_DYNAMIC_CALLBACKS == STD_ON
static KnxCallback_U_Timeout_Ind_Type KnxCallback_U_Timeout_Ind;
static KnxCallback_U_Reset_Ind_Type KnxCallback_U_Reset_Ind;
static KnxCallback_U_State_Ind_Type KnxCallback_U_State_Ind;

static KnxCallback_L_Data_Con_Type KnxCallback_L_Data_Con;
#endif  /* KNX_DYNAMIC_CALLBACKS */


/*!
 *
 *  Global Functions.
 *
 */


/**
 * Initialises the Data-Link-Layer.
 *
 */
LinkLayer::LinkLayer(ISerialPort * port)
{
    port_ = port;
    sequenceNo_ = (uint8_t)0x00;
    receiverIndex_ = (uint8_t)0x00;
    runningFCB_ = (uint8_t)0x00;
    localConfirmation_ = KNX_LL_CONF_NEGATIVE;
    repeated_ = false;
    expectation_ = { 0 };
    receiverIndex_ = 0;
    receiverStage_ = KNX_LL_RECEIVER_STAGE_NONE;
    setState(KnxLL_StateType::KNX_LL_STATE_IDLE);
}


void LinkLayer::stateLock() const
{
    while(stateFlag_.test_and_set(std::memory_order_acquire)) {
    }
}


void LinkLayer::stateUnlock() const
{
    stateFlag_.clear(std::memory_order_release);
}


KnxLL_StateType LinkLayer::getState() const
{
    stateLock();
    auto state = state_;
    stateUnlock();
    return state;
}


void LinkLayer::setState(KnxLL_StateType state)
{
    stateLock();
    state_ = state;
    stateUnlock();
}


uint8_t LinkLayer::checksum(uint8_t const* frame, uint8_t length)
{
    uint8_t checksum = (uint8_t)0xff;
    uint8_t idx;

    for (idx = (uint8_t)0; idx < length; ++idx) {
        checksum ^= frame[idx];
    }

    return checksum;
}


// This constitutes the link-layer statemachine.
void LinkLayer::feedReceiver(uint8_t octet)
{
    //DBG_PRINTLN(octet);
    if (getState() == KnxLL_StateType::KNX_LL_STATE_AWAITING_RESPONSE_LOCAL) {
        if (expectation_.ExpectedService == (octet & expectation_.ExpectedMask)) {
            if (expectation_.ExpectedByteCount == 1) {
                //printf("fo: 0x%02x\n", octet);
                TMR_STOP_DL_TIMER();
                setState(KnxLL_StateType::KNX_LL_STATE_IDLE);
                switch (expectation_.ExpectedService) {
                    case U_RESET_IND:
                        DBG_PRINTLN("U_RESET_IND");
                        functionCaller(KNX_CALLBACK_NAME(U_Reset_Ind));
                        break;
                    case U_STATE_IND:
                        DBG_PRINTLN("U_STATE_IND");
                        functionCaller(KNX_CALLBACK_NAME(U_State_Ind), octet);
                        break;
                    default:
                        //printf("Unexpected octet: 0x%02x\n", octet);
                        break;
                }
            }
        }
        // buffer_[0] = octet;
    } else if (getState() == KnxLL_StateType::KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION) {
        //DBG_PRINTLN("KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION");
        TMR_STOP_DL_TIMER();
        buffer_[receiverIndex_] = octet;
        receiverIndex_++;
        TMR_START_DL_TIMER();
        if (receiverIndex_ == 0x01) {
            if ((octet & U_STATE_IND) == U_STATE_IND) {
                //DBG_PRINTLN("Receiver Error!\n");
                functionCaller(KNX_CALLBACK_NAME(U_State_Ind), octet);
            }
            else if ((octet & L_DATA_CON) == L_DATA_CON) {
                TMR_STOP_DL_TIMER();
                if ((octet & 0x80) == 0x80) {
                    //DBG_PRINTLN("L_DATA_CON: ++");
                    localConfirmation_ = KNX_LL_CONF_POSITIVE;
                } else {
                    //DBG_PRINTLN("L_DATA_CON: --");
                    localConfirmation_ = KNX_LL_CONF_NEGATIVE;
                }
                KnxLl_Data_Con( ((octet & 0x80) == 0x80) ? Knx_StatusType::KNX_E_OK : Knx_StatusType::KNX_E_NOT_OK );
                setState(KnxLL_StateType::KNX_LL_STATE_IDLE);
                //DBG_PRINTLN("Reseting state to IDLE.");
            }
            else if ((octet & 0x10) == 0x10) {    /* Weak check. */
                // repeated_ = (octet & 0x20) == 0x00;
                //std::cout << "Rep? " << ((octet & 0x20) == 0x00) << std::endl;
            }
        }
        else if (receiverIndex_ == expectation_.ExpectedByteCount) {
            //printf("Finished. [0x%02x]\n", octet);
            //KnxEt_DumpHex(buffer_, receiverIndex_);

            receiverIndex_ = (uint8_t)0x00;
        }
    } else if (getState() == KnxLL_StateType::KNX_LL_STATE_AWAITING_RECEIPTION) {
        //DBG_PRINTLN("KNX_LL_STATE_AWAITING_RECEIPTION");
        TMR_STOP_DL_TIMER();
        receiverIndex_++;
        buffer_[receiverIndex_] = octet;
        runningFCB_ ^= octet;
        //printf("R: %02x ", octet);
        TMR_START_DL_TIMER();
        if (receiverStage_ == KNX_LL_RECEIVER_STAGE_HEADER) {
            if (receiverIndex_ == OFFS_NPCI) {
                expectation_.ExpectedByteCount = (uint8_t)((octet & 0x0f) + 2);
                receiverStage_ = KNX_LL_RECEIVER_STAGE_TRAILER;
            } else if (receiverIndex_ == OFFS_DEST_ADDR_H) {
                if (isAddressed(KNX_LL_ADDRESS_TYPE(), KNX_LL_DESTINATION_ADDRESS())) {
                    U_Ackn_req(ACK_ADDRESSED);
                      //printf("\nDA: 0x%04X\n", KNX_LL_DESTINATION_ADDRESS());
                }
            }
        } else if (receiverStage_ == KNX_LL_RECEIVER_STAGE_TRAILER) {
            if (receiverIndex_ == expectation_.ExpectedByteCount + OFFS_NPCI) {
                setState(KnxLL_StateType::KNX_LL_STATE_IDLE);
                /* TODO: Check FCB. */
                /* TODO: Callback. */
                KnxLL_DataStandard_Ind(buffer_);
            }
        }
    } else if (getState() == KnxLL_StateType::KNX_LL_STATE_TIMED_OUT) {
        functionCaller(KNX_CALLBACK_NAME(U_Timeout_Ind));
        setState(KnxLL_StateType::KNX_LL_STATE_IDLE);
    } else if (getState() == KnxLL_StateType::KNX_LL_STATE_IDLE) {
        if ((octet & U_STATE_IND) == U_STATE_IND) {
            DBG_PRINTLN("U_STATE_IND");
            //printf("U_State_Ind [0x%02x]\n", octet);
            functionCaller(KNX_CALLBACK_NAME(U_State_Ind), octet);
        } else if ((octet & 0x7f) == L_DATA_CON) {
            //DBG_PRINTLN("L_Data_Con");    // ???
            DBG_PRINTLN("L_DATA_CON");
        } else if ((octet & 0xd3) == L_DATA_EXTENDED_IND) { // #if defined()
            DBG_PRINTLN("L_DATA_EXTENDED_IND");
            //DBG_PRINTLN("L_DataExtended_Ind");
        } else if ((octet & 0xd3) == L_DATA_STANDARD_IND)  {
            DBG_PRINTLN("L_DATA_STANDARD_IND");
            //DBG_PRINTLN("L_DataStandard_Ind");
            //printf("CTRL[%02x]\n", octet);
            setState(KnxLL_StateType::KNX_LL_STATE_AWAITING_RECEIPTION); /* TODO: Distiguish Standard/Extendend Frames */
            receiverStage_ = KNX_LL_RECEIVER_STAGE_HEADER;
            receiverIndex_ = (uint8_t)0;
            runningFCB_ = (uint8_t)0xff ^ octet;
            buffer_[receiverIndex_] = octet;
            repeated_ = (octet & 0x20) == 0x00;
            TMR_START_DL_TIMER();
        } else if (octet == U_RESET_IND) {
            functionCaller(KNX_CALLBACK_NAME(U_Reset_Ind));
        } else if (octet == L_POLL_DATA_IND) { // #if defined()
            //DBG_PRINTLN("L_PollData_Ind");
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


bool LinkLayer::isBusy(void)
{
    bool result;

    PORT_LOCK_TASK_LEVEL();
    result = (getState() != KnxLL_StateType::KNX_LL_STATE_IDLE);
    PORT_UNLOCK_TASK_LEVEL();
    return result;
}


/**
* Wait for availability of link-layer.
*
* NB: Services shall never be issued while link-layer is busy!
*/
void LinkLayer::busyWait(void)
{   
    while (isBusy()) {
    }
}


bool LinkLayer::isConfirmed()
{
    bool result;

    PORT_LOCK_TASK_LEVEL();
    result = localConfirmation_ == KNX_LL_CONF_POSITIVE;
    PORT_UNLOCK_TASK_LEVEL();

    return result;
}

/**
 *  Gets called from interrupt-context if a timeout occured.
 *
 */
void LinkLayer::timeoutCB()
{
    DBG_PRINTLN("L2 TIMEOUT.");
    setState(KnxLL_StateType::KNX_LL_STATE_TIMED_OUT);
    feedReceiver((uint8_t)0x00);
}


bool LinkLayer::isAddressed(uint8_t daf, uint16_t address)
{
    if (daf == ADDRESS_TYPE_MULTICAST) {

    } else if (daf == ADDRESS_TYPE_INDIVIDUAL) {

    }

    return true;
}


/**
 * Issues an internal Command, i.e. related to the interface IC, not EIB.
 *
 * @param frame Octets to be send.
 * @param length Number of octets.
 * @param desiredState Desired state to enter (in case of confirmed commands, we need to wait for an answer).
 *
 * @return bool
 */
bool LinkLayer::internalCommand(uint8_t const * frame, uint8_t length, KnxLL_StateType desiredState)
{
    bool result;

    PORT_LOCK_TASK_LEVEL();
    if (getState() != KnxLL_StateType::KNX_LL_STATE_IDLE) {
        PORT_UNLOCK_TASK_LEVEL();
        return false;
    }
    setState(desiredState);
    result = port_->write(frame, (uint32_t)length);
    PORT_UNLOCK_TASK_LEVEL();
    TMR_START_DL_TIMER();
    return result;
}

bool LinkLayer::internalCommandUnconfirmed(uint8_t const * frame, uint8_t length)
{
    return internalCommand(frame, length, KnxLL_StateType::KNX_LL_STATE_IDLE);
}

bool LinkLayer::internalCommandConfirmed(uint8_t const * frame, uint8_t length)
{
    return internalCommand(frame, length, KnxLL_StateType::KNX_LL_STATE_AWAITING_RESPONSE_LOCAL);
}


#if KNX_STACK_TYPE == KNX_FULL_STACK
void KnxLL_Task(void)
{
    KnxDisp_DispatchLayer(TASK_LL_ID, KnxLl_ServiceTable);
}
#endif /* KNX_STACK_TYPE */

/*
 *
 * Basic configuration: KNX_LINK_LAYER_ONLY | KNX_FULL_STACK
 *
 */

/*
**
**  Services from Network-Layer.
**
*/
void LinkLayer::disp_L_Data_Req(void)
{
    //uint8_t chk;

    KnxMsg_SetFrameType(KnxMsg_ScratchBufferPtr, Knx_FrameType::KNX_FRAME_STANDARD);

    /* PREPARE_CONTROL_FIELD() */
    KnxMsg_ScratchBufferPtr->msg.raw[0] |= (uint8_t)0x30;   /* fixed one bit + repeated. */
    KnxMsg_ScratchBufferPtr->msg.raw[0] &= (~(uint8_t)3);   /* clear two LSBs. */
    /**/
    
    //DBG_PRINTLN("");
    //DBG_PRINT("Disp_L_Data_Req: ");
    KnxEt_DumpHex(KnxMsg_ScratchBufferPtr->msg.raw, KnxMsg_ScratchBufferPtr->len);

//    KnxLL_WriteFrame(KnxMsg_ScratchBufferPtr->msg.raw, KnxMsg_ScratchBufferPtr->len);
    writeFrame(KnxMsg_ScratchBufferPtr->msg.raw, KnxMsg_ScratchBufferPtr->len);
    (void)KnxMsg_ReleaseBuffer(KnxMsg_ScratchBufferPtr);
}


void LinkLayer::disp_L_PollData_Req(void)
{
    /* todo: Implement!!! */
    KnxMsg_SetFrameType(KnxMsg_ScratchBufferPtr, Knx_FrameType::KNX_FRAME_POLLING);
}




void LinkLayer::KnxLl_Data_Con(Knx_StatusType status)
{
    DBG_PRINTLN("LL_DATA_CON...");
    functionCaller(KNX_CALLBACK_NAME(L_Data_Con), status);
    KnxMsgIf_Post(buffer_, Knx_ServiceType::KNX_SERVICE_L_DATA_CON, status);
}


void LinkLayer::KnxLL_DataStandard_Ind(uint8_t const * frame)
{
    if (repeated_) {
        return;     /* Don't route duplicates for now. */
    }

    KnxMsgIf_Post(frame, Knx_ServiceType::KNX_SERVICE_L_DATA_IND, Knx_StatusType::KNX_E_OK);
}


/*!
 *
 * Local Functions.
 *
 */


void LinkLayer::expect(uint8_t service, uint8_t mask, uint8_t byteCount)
{
    expectation_.ExpectedService = service;
    expectation_.ExpectedMask = mask;
    expectation_.ExpectedByteCount = byteCount;
}

void LinkLayer::writeFrame(uint8_t const * frame, uint8_t length)

{
    uint8_t idx;
    uint8_t cs;
    char buffer[128];

    cs = checksum(frame, length);
    buffer[0] = U_L_DATASTART_REQ;
    buffer[1] = frame[0];
    for (idx = (uint8_t)2; idx < (length << 1); idx += (uint8_t)2) {
        buffer[idx] = U_L_DATACONT_REQ | (idx >> 1);
        buffer[idx + 1] = frame[idx >> 1];
    }
    buffer[idx] = U_L_DATAEND_REQ | (idx >> 1);
    buffer[idx + 1] = cs;
    port_->write((uint8_t *)buffer, (uint32_t)(idx + (uint8_t)2));
    expect((uint8_t)0x00, (uint8_t)0x00, length + (uint8_t)1);
    setState(KnxLL_StateType::KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION);
    receiverIndex_ = (uint8_t)0x00;
    TMR_START_DL_TIMER();
}


/**
 *
 *  Local unconfirmed services.
 *
 */
void LinkLayer::U_ActivateBusmon_req(void)
{
    DBG_PRINTLN("U_ActivateBusmon_req");
    buffer_[0] = U_BUSMON_REQ;
    internalCommandUnconfirmed(buffer_, (uint8_t)1);
}

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
void LinkLayer::U_ActivateBusyMode_req(void)
{
    DBG_PRINTLN("U_ActivateBusyMode_req");
    buffer_[0] = U_SETBUSY_REQ;
    internalCommandUnconfirmed(buffer_, (uint8_t)1);
}

void LinkLayer::U_ResetBusyMode_req(void)
{
    DBG_PRINTLN("U_ResetBusyMode_req");
    buffer_[0] = U_QUITBUSY_REQ;
    internalCommandUnconfirmed(buffer_, (uint8_t)1);
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void LinkLayer::U_ActivateBusyMode_req(void)
{
    DBG_PRINTLN("U_ActivateBusyMode_req");
    buffer_[0] = U_ACTIVATEBUSYMODE_REQ;
    nternalCommandUnconfirmed(buffer_, (uint8_t)1);
}


void LinkLayer::U_ResetBusyMode_req(void)
{
    DBG_PRINTLN("U_ResetBusyMode_req");
    buffer_[0] = U_RESETBUSYMODE_REQ;
    internalCommandUnconfirmed(buffer_, (uint8_t)1);
}

void LinkLayer::U_SetRepetition_req(uint8_t rst)
{
    DBG_PRINTLN("U_SetRepetition_req");
    buffer_[0] = U_MXRSTCNT_REQ;
    buffer_[1] = rst;
    internalCommandUnconfirmed(buffer_, (uint8_t)2);
}

void LinkLayer::U_ActivateCRC_req(void)
{
    DBG_PRINTLN("U_ActivateCRC_req");
    buffer_[0] = U_ACTIVATECRC_REQ;
    internalCommandUnconfirmed(buffer_, (uint8_t)1);
}

void LinkLayer::U_SetAddress_req(uint16_t address)
{
    DBG_PRINTLN("U_SetAddress_req");
    buffer_[0] = U_SETADDRESS_REQ;
    buffer_[1] = KNX_HIBYTE(address);
    buffer_[2] = KNX_LOBYTE(address);
    internalCommandUnconfirmed(buffer_, (uint8_t)3);
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
void LinkLayer::U_SetRepetition_req(uint8_t rst)
{
    DBG_PRINTLN("U_SetRepetition_req");
    buffer_[0] = U_SETREPETITION_REQ;
    buffer_[1] = rst;
    buffer_[2] = (uint8_t)0x00; /* Dummy */
    buffer_[3] = (uint8_t)0x00; /* Dummy */
    internalCommandUnconfirmed(buffer_, (uint8_t)4);
}
#endif /* KNX_BUS_INTERFACE */

void LinkLayer::U_Ackn_req(uint8_t what)
{

//    DBG_PRINTLN("U_Ackn_req");
    buffer_[0] = U_ACKN_REQ | (what & 0x07);
    internalCommandUnconfirmed(buffer_, (uint8_t)1);
}

/**
 *
 *  Local confirmed services.
 *
 */

void LinkLayer::U_Reset_req(void)
{
    DBG_PRINTLN("U_Reset_req");
    buffer_[0] = U_RESET_REQ;
    internalCommandConfirmed(buffer_, (uint8_t)1);
    expect(U_RESET_IND, (uint8_t)0xff, (uint8_t)1);
}

void LinkLayer::U_State_req(void)
{
    DBG_PRINTLN("U_State_req");
    buffer_[0] = U_STATE_REQ;
    internalCommandConfirmed(buffer_, (uint8_t)1);
    expect(U_STATE_IND, (uint8_t)0x07, (uint8_t)1);
}

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void LinkLayer::U_ProductID_req(void)
{
    DBG_PRINTLN("U_ProductID_req");
    buffer_[0] = U_PRODUCTID_REQUEST;
    internalCommandConfirmed(buffer_, (uint8_t)1);
    expect((uint8_t)0x00, (uint8_t)0x00, (uint8_t)1);    /* Expect any single byte value. */
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
void LinkLayer::U_SetAddress_req(uint16_t address) /* NB: TPUART2's SetAddress is unconfirmed! */
{
    DBG_PRINTLN("U_SetAddress_req");
    buffer_[0] = U_SETADDRESS_REQ;
    buffer_[1] = KNX_HIBYTE(address);
    buffer_[2] = KNX_LOBYTE(address);
    buffer_[3] = (uint8_t)0x00; /* Dummy */
    internalCommandConfirmed(buffer_, (uint8_t)4);
    expect(U_CONFIGURE_IND, (uint8_t)0x83, (uint8_t)1);
}
#endif /* KNX_BUS_INTERFACE */

/**
 *  Transmission services.
 */


#if 0
void LinkLayer::U_SystemState_req(void)
{
    DBG_PRINTLN("U_SystemState_req");
    buffer_[0] = U_SYSTEMSTAT_REQ;
    internalCommandConfirmed(buffer_, (uint8_t)2);
    expect(U_SYSTEMSTAT_IND, (uint8_t)0xff, (uint8_t)2);
}

void LinkLayer::U_StopMode_req(void)
{
    DBG_PRINTLN("U_StopMode_req");
    buffer_[0] = U_STOPMODE_REQ;
    internalCommandConfirmed(buffer_, (uint8_t)1);
    expect(U_STOPMODE_IND, (uint8_t)0xff, (uint8_t)1);
}

void LinkLayer::U_ExitStopMode_req(void)
{
    DBG_PRINTLN("U_ExitStopMode_req");
    buffer_[0] = U_EXITSTOPMODE_REQ;
    internalCommandConfirmed(buffer_, (uint8_t)1);
    expect(U_RESET_IND, (uint8_t)0xff, (uint8_t)1);
}

void LinkLayer::U_Configure_req(uint8_t conf)
{
    DBG_PRINTLN("U_Configure_req");
    buffer_[0] = U_CONFIGURE_REQ | (conf & 0x07);
    internalCommandConfirmed(buffer_, (uint8_t)1);
    expect(U_CONFIGURE_IND, (uint8_t)0xff, (uint8_t)1);
}

void LinkLayer::U_IntRegWr_req(uint8_t addr, uint8_t value)
{
    DBG_PRINTLN("U_IntRegWr_req");
    buffer_[0] = U_INTREGWR_REQ | (addr & 0x03);
    buffer_[1] = value;
    internalCommandUnconfirmed(buffer_, (uint8_t)2);
}

void LinkLayer::U_IntRegRd_req(uint8_t addr)
{
    DBG_PRINTLN("U_IntRegRd_req");
    buffer_[0] = U_INTREGRD_REQ;
    internalCommandConfirmed(buffer_, (uint8_t)1);
    expect((uint8_t)0x00, (uint8_t)0x00, (uint8_t)1);
}
#endif

/*!
  *
  * Callback Setters.
  *
  */
#if KNX_DYNAMIC_CALLBACKS == STD_ON
void KnxLL_Set_U_Timeout_Ind_Callback(KnxCallback_U_Timeout_Ind_Type const * const callback)
{
    KnxCallback_U_Timeout_Ind = *callback;
}

void KnxLL_Set_U_Reset_Ind_Callback(KnxCallback_U_Reset_Ind_Type const * const callback)
{
    KnxCallback_U_Reset_Ind = *callback;
}

void KnxLL_Set_U_State_Ind_Callback(KnxCallback_U_State_Ind_Type const * const callback)
{
    KnxCallback_U_State_Ind = *callback;
}

void KnxLL_Set_L_Data_Con_Callback(KnxCallback_L_Data_Con_Type& callback)
{
    KnxCallback_L_Data_Con = callback;
}


#endif  /* KNX_DYNAMIC_CALLBACKS */

} // namespace knx

