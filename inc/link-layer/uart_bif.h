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
#if !defined(__UART_BIF_H)
#define __UART_BIF_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


#include "k-ps/config.h"

/*
**  Service-IDs.
*/
#define KNX_SERVICE_LL_WRITE_FRAME              ((uint8_t)0x01)

/*
**  Module Errors.
*/
#define UART_BIF_E_UNINIT                       ((uint8_t)0x01)

#if 0
#define SPI_E_UNINIT                            ((uint8)0x1a)
#define SPI_E_SEQ_PENDING                       ((uint8)0x2a)
#define SPI_E_SEQ_IN_PROCESS                    ((uint8)0x3a)
#define SPI_E_ALREADY_INITIALIZED               ((uint8)0x4a)
#endif


/**
 *
 * Services from Host-Controller.
 *
 */
/* INTERNAL COMMANDS – DEVICE SPECIFIC */
#define U_RESET_REQ             ((uint8_t)0x01)
#define U_STATE_REQ             ((uint8_t)0x02)

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
#define U_SETBUSY_REQ           ((uint8_t)0x03)
#define U_QUITBUSY_REQ          ((uint8_t)0x04)
#endif /* KNX_BUS_INTERFACE */

#define U_BUSMON_REQ            ((uint8_t)0x05)

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
#define U_PRODUCTID_REQUEST     ((uint8_t)0x14)
#define U_ACTIVATEBUSYMODE_REQ  ((uint8_t)0x15)
#define U_RESETBUSYMODE_REQ     ((uint8_t)0x16)
#define U_MXRSTCNT_REQ          ((uint8_t)0x18)
#define U_ACTIVATECRC_REQ       ((uint8_t)0x19)
#define U_SETADDRESS_REQ        ((uint8_t)0x1C)
#endif /* KNX_BUS_INTERFACE */

#define U_SETADDRESS_REQ        ((uint8_t)0xF1)
#define U_SETREPETITION_REQ     ((uint8_t)0xF2)

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
#define U_L_DATAOFFSET_REQ      ((uint8_t)0x08) /* 08 - 0C */
#define U_SYSTEMSTAT_REQ        ((uint8_t)0x0D)
#define U_STOPMODE_REQ          ((uint8_t)0x0E)
#define U_EXITSTOPMODE_REQ      ((uint8_t)0x0F)
#endif /* KNX_BUS_INTERFACE */

#define U_ACKN_REQ              ((uint8_t)0x10) /* 10 - 17 */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
#define U_CONFIGURE_REQ         ((uint8_t)0x18) /* 18 - 1F */
#define U_INTREGWR_REQ          ((uint8_t)0x28) /* 28 - 2B */
#define U_INTREGRD_REQ          ((uint8_t)0x38) /* 38 - 3B */
#endif /* KNX_BUS_INTERFACE */

#define U_POLLINGSTATE_REQ      ((uint8_t)0xE0) /* E0 - EE */


/* KNX TRANSMIT DATA COMMANDS */
#define U_L_DATASTART_REQ       ((uint8_t)0x80)
#define U_L_DATACONT_REQ        ((uint8_t)0x80) /* 81 - BF */
#define U_L_DATAEND_REQ         ((uint8_t)0x40) /* 47 - 7F */

/**
 *
 * Services to Host-Controller.
 *
 */
#define L_ACKN_IND              ((uint8_t)0x00)

#define U_CONFIGURE_IND         ((uint8_t)0x01)

#define U_RESET_IND             ((uint8_t)0x03)
#define U_STATE_IND             ((uint8_t)0x07)
#define L_DATA_CON              ((uint8_t)0x0b)
#define L_DATA_EXTENDED_IND     ((uint8_t)0x10)

#define U_FRAMESTATE_IND        ((uint8_t)0x13)
#define U_STOPMODE_IND          ((uint8_t)0x2b)
#define U_SYSTEMSTAT_IND        ((uint8_t)0x4b)

#define L_DATA_STANDARD_IND     ((uint8_t)0x90)

#define U_FRAMEEND_IND          ((uint8_t)0xcb)

#define L_POLL_DATA_IND         ((uint8_t)0xf0)


/**
 *  Local unconfirmed services.
 */
void U_ActivateBusmon_req(void);

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void U_ActivateBusyMode_req(void);
void U_ResetBusyMode_req(void);
void U_SetRepetition_req(uint8_t rst);
void U_ActivateCRC_req(void);
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2 || KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
void U_SetRepetition_req(uint8_t rst);
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120

#endif /* KNX_BUS_INTERFACE */

/**
 *  Global Function-like Macros.
 */



/**
 *  Global Functions.
 */
void KnxLL_FeedReceiver(uint8_t byte);
void KnxLL_Init(void);
void KnxLL_Task(void);
void KnxLL_WriteFrame(uint8_t const * frame, uint8_t length);
boolean KnxLL_IsBusy(void);
void KnxLL_BusyWait(void);
boolean KnxLL_IsConfirmed(void);
void KnxLL_TimeoutCB(void);

/**
 *  Local confirmed services.
 */

void U_Reset_req(void);
void U_State_req(void);

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void U_ProductID_req(void);
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
void U_SetAddress_req(uint16_t address);
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120

#endif /* KNX_BUS_INTERFACE */


/**
 *  Transmission services.
 */


#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __UART_BIF_H */

