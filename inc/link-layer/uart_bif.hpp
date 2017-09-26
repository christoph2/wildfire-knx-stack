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
#if !defined(__UART_BIF_HPP)
#define __UART_BIF_HPP

#include "knx_types.hpp"
#include "knx_defs.hpp"
#include "knx_serialport.hpp"
#include "Wildfire_Config.hpp"

#include <atomic>
#include <functional>

namespace knx {

/*
**  Service-IDs.
*/
constexpr uint8_t KNX_SERVICE_LL_WRITE_FRAME = 0x01;

/*
**  Module Errors.
*/
constexpr uint8_t UART_BIF_E_UNINIT                        = 0x01;


/**
 *
 * Services from Host-Controller.
 *
 */
/* INTERNAL COMMANDS – DEVICE SPECIFIC */
constexpr uint8_t U_RESET_REQ              = 0x01;
constexpr uint8_t U_STATE_REQ              = 0x02;

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
constexpr uint8_t U_SETBUSY_REQ            = 0x03;
constexpr uint8_t U_QUITBUSY_REQ           = 0x04;
#endif /* KNX_BUS_INTERFACE */

constexpr uint8_t U_BUSMON_REQ             = 0x05;

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
constexpr uint8_t U_PRODUCTID_REQUEST      = 0x14;
constexpr uint8_t U_ACTIVATEBUSYMODE_REQ   = 0x15;
constexpr uint8_t U_RESETBUSYMODE_REQ      = 0x16;
constexpr uint8_t U_MXRSTCNT_REQ           = 0x18;
constexpr uint8_t U_ACTIVATECRC_REQ        = 0x19;
constexpr uint8_t U_SETADDRESS_REQ         = 0x1C;
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
constexpr uint8_t U_L_DATAOFFSET_REQ       = 0x08; /* 08 - 0C */
constexpr uint8_t U_SYSTEMSTAT_REQ         = 0x0D;
constexpr uint8_t U_STOPMODE_REQ           = 0x0E;
constexpr uint8_t U_EXITSTOPMODE_REQ       = 0x0F;
#endif /* KNX_BUS_INTERFACE */

constexpr uint8_t U_ACKN_REQ               = 0x10; /* 10 - 17 */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
constexpr uint8_t U_CONFIGURE_REQ          = 0x18; /* 18 - 1F */
constexpr uint8_t U_INTREGWR_REQ           = 0x28; /* 28 - 2B */
constexpr uint8_t U_INTREGRD_REQ           = 0x38; /* 38 - 3B */
#endif /* KNX_BUS_INTERFACE */

constexpr uint8_t U_POLLINGSTATE_REQ       = 0xE0; /* E0 - EE */


/* KNX TRANSMIT DATA COMMANDS */
constexpr uint8_t U_L_DATASTART_REQ        = 0x80;
constexpr uint8_t U_L_DATACONT_REQ         = 0x80; /* 81 - BF */
constexpr uint8_t U_L_DATAEND_REQ          = 0x40; /* 47 - 7F */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120 /* TODO: Überprüfen!!!*/
constexpr uint8_t U_SETADDRESS_REQ         = 0xF1;
constexpr uint8_t U_SETREPETITION_REQ      = 0xF2;
#endif


/**
 *
 * Services to Host-Controller.
 *
 */
constexpr uint8_t L_ACKN_IND               = 0x00;

constexpr uint8_t U_CONFIGURE_IND          = 0x01;

constexpr uint8_t U_RESET_IND              = 0x03;
constexpr uint8_t U_STATE_IND              = 0x07;
constexpr uint8_t L_DATA_CON               = 0x0b;
constexpr uint8_t L_DATA_EXTENDED_IND      = 0x10;

constexpr uint8_t U_FRAMESTATE_IND         = 0x13;
constexpr uint8_t U_STOPMODE_IND           = 0x2b;
constexpr uint8_t U_SYSTEMSTAT_IND         = 0x4b;

constexpr uint8_t L_DATA_STANDARD_IND      = 0x90;

constexpr uint8_t U_FRAMEEND_IND           = 0xcb;

constexpr uint8_t L_POLL_DATA_IND          = 0xf0;

/*
** Global Types.
*/
enum class KnxLL_StateType {
    KNX_LL_STATE_IDLE,
    KNX_LL_STATE_RECEIVING,
    KNX_LL_STATE_SENDING,
    KNX_LL_STATE_AWAITING_RESPONSE_LOCAL,
    KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION,
    KNX_LL_STATE_AWAITING_RECEIPTION,
    KNX_LL_STATE_TIMED_OUT
};

typedef struct tagKnxLL_ExpectationType {
    uint8_t ExpectedByteCount;
    uint8_t ExpectedService;
    uint8_t ExpectedMask;
} KnxLL_ExpectationType;


typedef enum tagKnxLL_LocalConfirmationType {
    KNX_LL_CONF_NEGATIVE,
    KNX_LL_CONF_POSITIVE
} KnxLL_LocalConfirmationType;


typedef enum tagKnxLL_ReceiverStageType {
    KNX_LL_RECEIVER_STAGE_NONE,
    KNX_LL_RECEIVER_STAGE_HEADER,
    KNX_LL_RECEIVER_STAGE_TRAILER
} KnxLL_ReceiverStageType;


constexpr uint8_t KNX_LL_BUF_SIZE = 0xff;

class LinkLayer {
public:
    explicit LinkLayer(ISerialPort * port);

    KnxLL_StateType getState() const;
    void setState(KnxLL_StateType state);
    void feedReceiver(uint8_t byte);
    void timeoutCB();

    bool isAddressed(uint8_t daf, uint16_t address);
    bool isBusy();
    void busyWait();
    bool isConfirmed();

    bool internalCommand(uint8_t const * frame, uint8_t length, KnxLL_StateType desiredState);
    bool internalCommandUnconfirmed(uint8_t const * frame, uint8_t length);
    bool internalCommandConfirmed(uint8_t const * frame, uint8_t length);

    void expect(uint8_t service, uint8_t mask, uint8_t byteCount);
    void writeFrame(uint8_t const * frame, uint8_t length);

/*
**
**  Services.
**
*/
    void U_ActivateBusmon_req();

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
    void U_ActivateBusyMode_req();
    void U_ResetBusyMode_req();
    void U_SetRepetition_req(uint8_t rst);
    void U_ActivateCRC_req();
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2 || KNX_BUS_INTERFACE == KNX_BIF_NCN5120
void U_SetRepetition_req(uint8_t rst);
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120

#endif /* KNX_BUS_INTERFACE */

    void U_Ackn_req(uint8_t what);
    void U_Reset_req();
    void U_State_req();

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
    void U_ProductID_req();
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
    void U_SetAddress_req(uint16_t address);
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_NCN5120

#endif /* KNX_BUS_INTERFACE */


/*
**
**
**
*/

    void KnxLL_Task();

    void disp_L_Data_Req(void);
    void disp_L_PollData_Req(void);

    void KnxLl_Data_Con(Knx_StatusType status);
    void KnxLL_DataStandard_Ind(uint8_t const * frame);

    static uint8_t checksum(uint8_t const* frame, uint8_t length);
private:

    void stateLock() const;
    void stateUnlock() const;

    ISerialPort * port_;

    volatile KnxLL_StateType state_;
    mutable std::atomic_flag stateFlag_ = ATOMIC_FLAG_INIT;

    uint8_t sequenceNo_;
    uint8_t runningFCB_;
    KnxLL_LocalConfirmationType localConfirmation_;
    bool repeated_;
    KnxLL_ExpectationType expectation_;
    uint8_t receiverIndex_;
    uint8_t buffer_[KNX_LL_BUF_SIZE];
    KnxLL_ReceiverStageType receiverStage_;
    static const Knx_LayerServiceFunctionType KnxLl_Services[];
    static const Knx_LayerServicesType KnxLl_ServiceTable[];
};


/**
 *  Local unconfirmed services.
 */

/**
 *  Callback stuff.
 */
#if KNX_DYNAMIC_CALLBACKS == STD_ON
using KnxCallback_U_Timeout_Ind_Type = void (*)();
using KnxCallback_U_Reset_Ind_Type = void (*)();
using KnxCallback_U_State_Ind_Type = void (*)(uint8_t state);

//using KnxCallback_L_Data_Con_Type = std::function<void(KnxLL_StateType state)>;
using KnxCallback_L_Data_Con_Type = void (*)(Knx_StatusType state);

void KnxLL_Set_U_Timeout_Ind_Callback(KnxCallback_U_Timeout_Ind_Type const * const callback);
void KnxLL_Set_U_Reset_Ind_Callback(KnxCallback_U_Reset_Ind_Type const * const callback);
void KnxLL_Set_U_State_Ind_Callback(KnxCallback_U_State_Ind_Type const * const callback);

void KnxLL_Set_L_Data_Con_Callback(KnxCallback_L_Data_Con_Type& callback);

#endif /* KNX_DYNAMIC_CALLBACKS */


/**
 *  Transmission services.
 */

} // namespace knx

#endif  /* __UART_BIF_HPP */

