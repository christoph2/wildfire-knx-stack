/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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

#include "link-layer\uart_bif.h"
#include "Port_Serial.h"

#define KNX_LL_BUF_SIZE     (0xff)

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
    KNX_LL_STATE_TIMED_OUT
} KnxLL_StateType;


typedef enum tagKnxLL_EventType {
    KNX_LL_EVENT_REQUEST,
    KNX_LL_EVENT_INDICATION,
    KNX_LL_EVENT_TIMEOUT,
} KnxLL_EventType;

/*!
 *
 *  Local Variables.
 *
 */

static KnxLL_StateType KnxLL_State;
static uint8_t KnxLL_SequenceNo;

static uint8_t KnxLL_Buffer[KNX_LL_BUF_SIZE];

static uint8_t KnxLL_ExpectedByteCount;
static uint8_t KnxLL_ExpectedService;
static uint8_t KnxLL_ExpectedMask;

/*!
 *
 *  Local Function Prototypes.
 *
 */

static boolean KnxLL_InternalCommand(uint8_t const * frame, uint8_t length, KnxLL_StateType desiredState);
static boolean KnxLL_InternalCommandUnconfirmed(uint8_t const * frame, uint8_t length);
static boolean KnxLL_InternalCommandConfirmed(uint8_t const * frame, uint8_t length);
static uint8_t KnxLL_Checksum(uint8_t const * frame, uint8_t length);
static void KnxLL_Expect(uint8_t service, uint8_t mask, uint8_t byteCount);
static void KnxLL_WriteFrame(uint8_t const * frame, uint8_t length);
/*!
 *
 *  Global Functions.
 *
 */

void KnxLL_Init(void)
{
    KnxLL_State = KNX_LL_STATE_IDLE;
    KnxLL_SequenceNo = (uint8_t)0x00;
    KnxLL_ExpectedByteCount = (uint8_t)0x00;
}

void KnxLL_FeedReceiver(uint8_t byte)
{
    if (KnxLL_State == KNX_LL_STATE_AWAITING_RESPONSE_LOCAL) {
        KnxLL_State = KNX_LL_STATE_IDLE;
        KnxLL_Buffer[0] = byte;
    } else {
        /* Ignore anything else for now. */
    }

}

void KnxLL_Task(void)
{

}


boolean KnxLL_Transmit(uint8_t const * frame, uint8_t length)
{
    return TRUE;
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
static boolean KnxLL_InternalCommand(uint8_t const * frame, uint8_t length, KnxLL_StateType desiredState)
{
    boolean result;

    if (KnxLL_State != KNX_LL_STATE_IDLE) {
        return FALSE;
    }
    //KnxLL_State = KNX_LL_STATE_SENDING;
    KnxLL_State = desiredState;
    result = (boolean)Port_WriteToBusInterface(frame, length);
    //KnxLL_State = desiredState;
    return result;
}

static boolean KnxLL_InternalCommandUnconfirmed(uint8_t const * frame, uint8_t length)
{
    return KnxLL_InternalCommand(frame, length, KNX_LL_STATE_IDLE);
}

static boolean KnxLL_InternalCommandConfirmed(uint8_t const * frame, uint8_t length)
{
    return KnxLL_InternalCommand(frame, length, KNX_LL_STATE_AWAITING_RESPONSE_LOCAL);
}

static void KnxLL_Expect(uint8_t service, uint8_t mask, uint8_t byteCount)
{
    KnxLL_ExpectedService = service;
    KnxLL_ExpectedMask = mask;
    KnxLL_ExpectedByteCount = byteCount;
}

static uint8_t KnxLL_Checksum(uint8_t const * frame, uint8_t length)
{
    uint8_t checksum = (uint8_t)0xff;
    uint8_t idx;

    for (idx = 0; idx < length; ++idx) {
        checksum ^= frame[idx];
    }

    return checksum;
}

#include <stdio.h>
#define WRITE(ch)   printf("%02x ", (ch))

static void KnxLL_WriteFrame(uint8_t const * frame, uint8_t length)
{
    uint8_t idx;
    uint8_t checksum;

    checksum = KnxLL_Checksum(frame, length);
    WRITE(U_L_DATASTART_REQ);
    WRITE(frame[0]);
    printf(" ");
    for (idx = 1; idx < length; ++idx) {
        WRITE(U_L_DATACONT_REQ | idx);
        WRITE(frame[idx]);
        printf(" ");
    }
    WRITE(U_L_DATAEND_REQ | idx);
    WRITE(checksum);
    printf("\n");
}


/**
 *
 *  Local unconfirmed services.
 *
 */
void U_ActivateBusmon_req(void)
{
    KnxLL_Buffer[0] = U_BUSMON_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
void U_ActivateBusyMode_req(void)
{
    KnxLL_Buffer[0] = U_SETBUSY_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}

void U_ResetBusyMode_req(void)
{
    KnxLL_Buffer[0] = U_QUITBUSY_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void U_ActivateBusyMode_req(void)
{
    KnxLL_Buffer[0] = U_ACTIVATEBUSYMODE_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}

void U_ResetBusyMode_req(void)
{
    KnxLL_Buffer[0] = U_RESETBUSYMODE_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}

void U_SetRepetition_req(uint8_t rst)
{
    KnxLL_Buffer[0] = U_MXRSTCNT_REQ;
    KnxLL_Buffer[1] = rst;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 2);
}

void U_ActivateCRC_req(void)
{
    KnxLL_Buffer[0] = U_ACTIVATECRC_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}

void U_SetAddress_req(uint16_t address)
{
    KnxLL_Buffer[0] = U_SETADDRESS_REQ;
    KnxLL_Buffer[1] = HIBYTE(address);
    KnxLL_Buffer[2] = LOBYTE(address);
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 3);
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
void U_SetRepetition_req(uint8_t rst)
{
    KnxLL_Buffer[0] = U_SETREPETITION_REQ;
    KnxLL_Buffer[1] = rst;
    KnxLL_Buffer[2] = 0x00; /* Dummy */
    KnxLL_Buffer[3] = 0x00; /* Dummy */
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 4);
}
#endif /* KNX_BUS_INTERFACE */

/**
 *
 *  Local confirmed services.
 *
 */

void U_Reset_req(void)
{
    KnxLL_Buffer[0] = U_RESET_REQ;
    KnxLL_Expect(U_RESET_IND, 0xff, 1);
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
}

void U_State_req(void)
{
    KnxLL_Buffer[0] = U_STATE_REQ;
    KnxLL_Expect(U_STATE_IND, 0x07, 1);
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
}

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void U_ProductID_req(void)
{
    KnxLL_Buffer[0] = U_PRODUCTID_REQUEST;
    KnxLL_Expect(0x00, 0x00, 1);    /* Expect any single byte value. */
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
void U_SetAddress_req(uint16_t address) /* NB: TPUART2's SetAddress is unconfirmed! */
{
    KnxLL_Buffer[0] = U_SETADDRESS_REQ;
    KnxLL_Expect(U_CONFIGURE_IND, 0x83, 1);
    KnxLL_Buffer[1] = HIBYTE(address);
    KnxLL_Buffer[2] = LOBYTE(address);
    KnxLL_Buffer[3] = 0x00; /* Dummy */
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 4);
}
#endif /* KNX_BUS_INTERFACE */

/**
 *  Transmission services.
 */


#if 0
void U_SystemState_req(void)
{
    KnxLL_Buffer[0] = U_SYSTEMSTAT_REQ;
    KnxLL_Expect(U_SYSTEMSTAT_IND, 0xff, 2);
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 2);
}

void U_StopMode_req(void)
{
    KnxLL_Buffer[0] = U_STOPMODE_REQ;
    KnxLL_Expect(U_STOPMODE_IND, 0xff, 1);
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
}

void U_ExitStopMode_req(void)
{
    KnxLL_Buffer[0] = U_EXITSTOPMODE_REQ;
    KnxLL_Expect(U_RESET_IND, 0xff, 1);
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
}

void U_Configure_req(uint8_t conf)
{
    KnxLL_Buffer[0] = U_CONFIGURE_REQ | (conf & 0x07);
    KnxLL_Expect(U_CONFIGURE_IND, 0xff, 1);
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);

}

void U_IntRegWr_req(uint8_t addr, uint8_t value)
{
    KnxLL_Buffer[0] = U_INTREGWR_REQ | (addr & 0x03);
    KnxLL_Buffer[1] = value;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 2);
}

void U_IntRegRd_req(uint8_t addr)
{
    KnxLL_Buffer[0] = U_INTREGRD_REQ;
    KnxLL_Expect(0x00, 0x00, 1);
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
}
#endif
