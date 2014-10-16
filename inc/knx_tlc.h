/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
*                                       cpu12.gems@googlemail.com>
*
*   All Rights Reserved
*
*  This program is free softwKNXe; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free SoftwKNXe Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
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

FUNC(void, KSTACK_CODE) T_Connect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source,
                                      Knx_AddressType dest
                                      );
FUNC(void, KSTACK_CODE) T_Disconnect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source,
                                         Knx_AddressType dest
                                         );

FUNC(void, KSTACK_CODE) T_Ack_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source,
                                  Knx_AddressType dest, uint8_t SeqNo
                                  );
FUNC(void, KSTACK_CODE) T_Nak_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source,
                                  Knx_AddressType dest, uint8_t SeqNo
                                  );

FUNC(uint8_t, KSTACK_CODE)    KnxTLC_GetSequenceNumberSend(void);
FUNC(uint8_t, KSTACK_CODE)    KnxTLC_GetSequenceNumberReceived(void);
FUNC(uint8_t, KSTACK_CODE)    KnxTLC_GetRepetitionCount(void);
FUNC(uint8_t, KSTACK_CODE)    KnxTLC_GetSequenceNumberOfPDU(void);
FUNC(Knx_AddressType, KSTACK_CODE)  KnxTLC_GetSourceAddress(void);
FUNC(Knx_AddressType, KSTACK_CODE)  KnxTLC_GetConnectionAddress(void);

FUNC(void, KSTACK_CODE) KnxTLC_SetSequenceNumberSend(uint8_t SequenceNumberSend);
FUNC(void, KSTACK_CODE) KnxTLC_SetSequenceNumberReceived(uint8_t SequenceNumberReceived);
FUNC(void, KSTACK_CODE) KnxTLC_SetRepetitionCount(uint8_t RepetitionCount);
FUNC(void, KSTACK_CODE) KnxTLC_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU);
FUNC(void, KSTACK_CODE) KnxTLC_SetSourceAddress(Knx_AddressType SourceAddress);
FUNC(void, KSTACK_CODE) KnxTLC_SetConnectionAddress(Knx_AddressType ConnectionAddress);
#else
void    KnxTLC_Init(void);
void    KnxTLC_Task(void);

void    T_Connect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
void    T_Disconnect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);

void    T_Ack_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo);
void    T_Nak_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo);

uint8_t           KnxTLC_GetSequenceNumberSend(void);
uint8_t           KnxTLC_GetSequenceNumberReceived(void);
uint8_t           KnxTLC_GetRepetitionCount(void);
uint8_t           KnxTLC_GetSequenceNumberOfPDU(void);
Knx_AddressType KnxTLC_GetSourceAddress(void);
Knx_AddressType KnxTLC_GetConnectionAddress(void);

void    KnxTLC_SetSequenceNumberSend(uint8_t SequenceNumberSend);
void    KnxTLC_SetSequenceNumberReceived(uint8_t SequenceNumberReceived);
void    KnxTLC_SetRepetitionCount(uint8_t RepetitionCount);
void    KnxTLC_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU);
void    KnxTLC_SetSourceAddress(Knx_AddressType SourceAddress);
void    KnxTLC_SetConnectionAddress(Knx_AddressType ConnectionAddress);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TLC_H */

