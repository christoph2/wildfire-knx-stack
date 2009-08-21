#if !defined(__TRANSPORT_H)
#define __TRANSPORT_H

#include "Address.h"
#include "Messaging.h"
#include "StateMachineKNX.h"
#include "Timer.h"
#include "knx_disp.h"

/*
**  TPCI-Codings.
*/

#define TPCI_UDT                ((uint8)0x00)    /* Unnumbered Data. */
#define TPCI_NDT                ((uint8)0x40)    /* Numbered Data (T_DATA_CONNECTED_REQ_PDU). */
#define TPCI_UCD                ((uint8)0x80)    /* Unnumbered Control. */
#define TPCI_NCD                ((uint8)0xC0)    /* Numbered Control (TACK/TNACK). */

#define T_ACK_PDU               ((uint8)0xC2)
#define T_NAK_PDU               ((uint8)0xC3)

#define T_CONNECT_REQ_PDU       ((uint8)0x80)
#define T_DISCONNECT_REQ_PDU    ((uint8)0x81)

#define T_DATA_TAG_GROUP_PDU    ((uint8)0x04)    /* Interface-Objects using Group-Addressing (LTE-HEE). */

void TL_Init(void);
void TLG_Task(void);
void TLC_Task(void);

void T_Connect_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest);
void T_Disconnect_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest);

void T_Ack_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 SeqNo);
void T_Nak_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 SeqNo);

/* 'knx_tlc.h' !!! */
uint8 KNXTlcGetSequenceNumberSend(void);
uint8 KNXTlcGetSequenceNumberReceived(void);
uint8 KNXTlcGetRepetitionCount(void);
uint8 KNXTlcGetSequenceNumberOfPDU(void);
Knx_AddressType KNXTlcGetSourceAddress(void);
Knx_AddressType KNXTlcGetConnectionAddress(void);


void KNXTlcSetSequenceNumberSend(uint8 SequenceNumberSend);
void KNXTlcSetSequenceNumberReceived(uint8 SequenceNumberReceived);
void KNXTlcSetRepetitionCount(uint8 RepetitionCount);
void KNXTlcSetSequenceNumberOfPDU(uint8 SequenceNumberOfPDU);
void KNXTlcSetSourceAddress(Knx_AddressType SourceAddress);
void KNXTlcSetConnectionAddress(Knx_AddressType ConnectionAddress);

#endif  /* __TRANSPORT_H */
