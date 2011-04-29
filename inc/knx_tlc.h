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
#if !defined(__KNX_TLC_H)
#define __KNX_TLC_H

#include "knx_layer_transport.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

void    KnxTLC_Init(void);
void    KnxTLC_Task(void);

void    T_Connect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
void    T_Disconnect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);

void    T_Ack_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo);
void    T_Nak_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 SeqNo);

uint8           KnxTLC_GetSequenceNumberSend(void);
uint8           KnxTLC_GetSequenceNumberReceived(void);
uint8           KnxTLC_GetRepetitionCount(void);
uint8           KnxTLC_GetSequenceNumberOfPDU(void);
Knx_AddressType KnxTLC_GetSourceAddress(void);
Knx_AddressType KnxTLC_GetConnectionAddress(void);

void    KnxTLC_SetSequenceNumberSend(uint8 SequenceNumberSend);
void    KnxTLC_SetSequenceNumberReceived(uint8 SequenceNumberReceived);
void    KnxTLC_SetRepetitionCount(uint8 RepetitionCount);
void    KnxTLC_SetSequenceNumberOfPDU(uint8 SequenceNumberOfPDU);
void    KnxTLC_SetSourceAddress(Knx_AddressType SourceAddress);
void    KnxTLC_SetConnectionAddress(Knx_AddressType ConnectionAddress);

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TLC_H */
