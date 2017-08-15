/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
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
#if !defined(__KNX_TLC_HPP)
#define __KNX_TLC_HPP

#include "knx_layer_transport.hpp"

namespace knx {

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxTlc_Init();
FUNC(void, KSTACK_CODE) KnxTlc_Task();

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

FUNC(uint8_t, KSTACK_CODE)    KnxTlc_GetSequenceNumberSend();
FUNC(uint8_t, KSTACK_CODE)    KnxTlc_GetSequenceNumberReceived();
FUNC(uint8_t, KSTACK_CODE)    KnxTlc_GetRepetitionCount();
FUNC(uint8_t, KSTACK_CODE)    KnxTlc_GetSequenceNumberOfPDU();
FUNC(Knx_AddressType, KSTACK_CODE)  KnxTlc_GetSourceAddress();
FUNC(Knx_AddressType, KSTACK_CODE)  KnxTlc_GetConnectionAddress();

FUNC(void, KSTACK_CODE) KnxTlc_SetSequenceNumberSend(uint8_t SequenceNumberSend);
FUNC(void, KSTACK_CODE) KnxTlc_SetSequenceNumberReceived(uint8_t SequenceNumberReceived);
FUNC(void, KSTACK_CODE) KnxTlc_SetRepetitionCount(uint8_t RepetitionCount);
FUNC(void, KSTACK_CODE) KnxTlc_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU);
FUNC(void, KSTACK_CODE) KnxTlc_SetSourceAddress(Knx_AddressType SourceAddress);
FUNC(void, KSTACK_CODE) KnxTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress);
#else
void KnxTlc_Init();
void KnxTlc_Task();

void T_Connect_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest);
void T_Disconnect_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest);

void T_Ack_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo);
void T_Nak_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo);

uint8_t KnxTlc_GetSequenceNumberSend();
uint8_t KnxTlc_GetSequenceNumberReceived();
uint8_t KnxTlc_GetRepetitionCount();
uint8_t KnxTlc_GetSequenceNumberOfPDU();
Knx_AddressType KnxTlc_GetSourceAddress();
Knx_AddressType KnxTlc_GetConnectionAddress();

void KnxTlc_SetSequenceNumberSend(uint8_t SequenceNumberSend);
void KnxTlc_SetSequenceNumberReceived(uint8_t SequenceNumberReceived);
void KnxTlc_SetRepetitionCount(uint8_t RepetitionCount);
void KnxTlc_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU);
void KnxTlc_SetSourceAddress(Knx_AddressType SourceAddress);
void KnxTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress);


#endif /* KSTACK_MEMORY_MAPPING */

} // namespace knx

#endif  /* __KNX_TLC_HPP */

