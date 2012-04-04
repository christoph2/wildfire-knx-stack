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
#if !defined(__KNX_TLC_H)
#define __KNX_TLC_H

#include "knx_layer_transport.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTLC_Init(void);
FUNC(void, KSTACK_CODE) KnxTLC_Task(void);

FUNC(void, KSTACK_CODE) T_Connect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                      Knx_AddressType dest
                                      );
FUNC(void, KSTACK_CODE) T_Disconnect_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                         Knx_AddressType dest
                                         );

FUNC(void, KSTACK_CODE) T_Ack_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                  Knx_AddressType dest, uint8 SeqNo
                                  );
FUNC(void, KSTACK_CODE) T_Nak_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                  Knx_AddressType dest, uint8 SeqNo
                                  );

FUNC(uint8, KSTACK_CODE)    KnxTLC_GetSequenceNumberSend(void);
FUNC(uint8, KSTACK_CODE)    KnxTLC_GetSequenceNumberReceived(void);
FUNC(uint8, KSTACK_CODE)    KnxTLC_GetRepetitionCount(void);
FUNC(uint8, KSTACK_CODE)    KnxTLC_GetSequenceNumberOfPDU(void);
FUNC(Knx_AddressType, KSTACK_CODE)  KnxTLC_GetSourceAddress(void);
FUNC(Knx_AddressType, KSTACK_CODE)  KnxTLC_GetConnectionAddress(void);

FUNC(void, KSTACK_CODE) KnxTLC_SetSequenceNumberSend(uint8 SequenceNumberSend);
FUNC(void, KSTACK_CODE) KnxTLC_SetSequenceNumberReceived(uint8 SequenceNumberReceived);
FUNC(void, KSTACK_CODE) KnxTLC_SetRepetitionCount(uint8 RepetitionCount);
FUNC(void, KSTACK_CODE) KnxTLC_SetSequenceNumberOfPDU(uint8 SequenceNumberOfPDU);
FUNC(void, KSTACK_CODE) KnxTLC_SetSourceAddress(Knx_AddressType SourceAddress);
FUNC(void, KSTACK_CODE) KnxTLC_SetConnectionAddress(Knx_AddressType ConnectionAddress);
#else
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


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TLC_H */

