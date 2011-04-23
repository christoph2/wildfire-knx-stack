/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__KNX_LAYER_TRANSPORT_H)
#define __KNX_LAYER_TRANSPORT_H

#include "knx_address.h"
#include "knx_messaging.h"
#include "StateMachineKNX.h"
#include "knx_timer.h"
#include "knx_disp.h"

/*
**  TPCI-Codings.
*/

#define TPCI_UDT                ((uint8)0x00)       /* Unnumbered Data. */
#define TPCI_NDT                ((uint8)0x40)       /* Numbered Data (T_DATA_CONNECTED_REQ_PDU). */
#define TPCI_UCD                ((uint8)0x80)       /* Unnumbered Control. */
#define TPCI_NCD                ((uint8)0xC0)       /* Numbered Control (TACK/TNACK). */

#define T_ACK_PDU               ((uint8)0xC2)
#define T_NAK_PDU               ((uint8)0xC3)

#define T_CONNECT_REQ_PDU       ((uint8)0x80)
#define T_DISCONNECT_REQ_PDU    ((uint8)0x81)

#define T_DATA_TAG_GROUP_PDU    ((uint8)0x04)    /* Interface-Objects using Group-Addressing (LTE-HEE). */

void    TL_Init(void);
void    TLG_Task(void);
void    TLC_Task(void);

void    T_Connect_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest);
void    T_Disconnect_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest);

void    T_Ack_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo);
void    T_Nak_Req(PMSG_Buffer pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo);

/* 'knx_tlc.h' !!! */
uint8           KNXTlc_GetSequenceNumberSend(void);
uint8           KNXTlc_GetSequenceNumberReceived(void);
uint8           KNXTlc_GetRepetitionCount(void);
uint8           KNXTlc_GetSequenceNumberOfPDU(void);
Knx_AddressType KNXTlc_GetSourceAddress(void);
Knx_AddressType KNXTlc_GetConnectionAddress(void);

void    KNXTlc_SetSequenceNumberSend(uint8 SequenceNumberSend);
void    KNXTlc_SetSequenceNumberReceived(uint8 SequenceNumberReceived);
void    KNXTlc_SetRepetitionCount(uint8 RepetitionCount);
void    KNXTlc_SetSequenceNumberOfPDU(uint8 SequenceNumberOfPDU);
void    KNXTlc_SetSourceAddress(Knx_AddressType SourceAddress);
void    KNXTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress);

#endif  /* __KNX_LAYER_TRANSPORT_H */
