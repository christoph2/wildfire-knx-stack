/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
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
FUNC(void, KSTACK_CODE) KnxTlc_Init(void);
FUNC(void, KSTACK_CODE) KnxTlc_Task(void);

FUNC(void, KSTACK_CODE) T_Connect_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source,
                                      Knx_AddressType dest
                                      );
FUNC(void, KSTACK_CODE) T_Disconnect_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source,
                                         Knx_AddressType dest
                                         );

FUNC(void, KSTACK_CODE) T_Ack_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source,
                                  Knx_AddressType dest, uint8_t SeqNo
                                  );
FUNC(void, KSTACK_CODE) T_Nak_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source,
                                  Knx_AddressType dest, uint8_t SeqNo
                                  );

FUNC(uint8_t, KSTACK_CODE)    KnxTlc_GetSequenceNumberSend(void);
FUNC(uint8_t, KSTACK_CODE)    KnxTlc_GetSequenceNumberReceived(void);
FUNC(uint8_t, KSTACK_CODE)    KnxTlc_GetRepetitionCount(void);
FUNC(uint8_t, KSTACK_CODE)    KnxTlc_GetSequenceNumberOfPDU(void);
FUNC(Knx_AddressType, KSTACK_CODE)  KnxTlc_GetSourceAddress(void);
FUNC(Knx_AddressType, KSTACK_CODE)  KnxTlc_GetConnectionAddress(void);

FUNC(void, KSTACK_CODE) KnxTlc_SetSequenceNumberSend(uint8_t SequenceNumberSend);
FUNC(void, KSTACK_CODE) KnxTlc_SetSequenceNumberReceived(uint8_t SequenceNumberReceived);
FUNC(void, KSTACK_CODE) KnxTlc_SetRepetitionCount(uint8_t RepetitionCount);
FUNC(void, KSTACK_CODE) KnxTlc_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU);
FUNC(void, KSTACK_CODE) KnxTlc_SetSourceAddress(Knx_AddressType SourceAddress);
FUNC(void, KSTACK_CODE) KnxTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress);
#else
void    KnxTlc_Init(void);
void    KnxTlc_Task(void);

void    T_Connect_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest);
void    T_Disconnect_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest);

void    T_Ack_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo);
void    T_Nak_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo);

uint8_t           KnxTlc_GetSequenceNumberSend(void);
uint8_t           KnxTlc_GetSequenceNumberReceived(void);
uint8_t           KnxTlc_GetRepetitionCount(void);
uint8_t           KnxTlc_GetSequenceNumberOfPDU(void);
Knx_AddressType KnxTlc_GetSourceAddress(void);
Knx_AddressType KnxTlc_GetConnectionAddress(void);

void    KnxTlc_SetSequenceNumberSend(uint8_t SequenceNumberSend);
void    KnxTlc_SetSequenceNumberReceived(uint8_t SequenceNumberReceived);
void    KnxTlc_SetRepetitionCount(uint8_t RepetitionCount);
void    KnxTlc_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU);
void    KnxTlc_SetSourceAddress(Knx_AddressType SourceAddress);
void    KnxTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_TLC_H */

