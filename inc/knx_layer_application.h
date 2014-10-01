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
#if !defined(__KNX_LAYER_APPLICATION_H)
#define __KNX_LAYER_APPLICATION_H

/*
**
**  Declarations common to the group-oriented and the managment parts of the application layer.
**
*/
#include "knx_alg.h"

#include "knx_messaging.h"
#include "knx_conversion.h"
#include "knx_address.h"
#include "knx_layer_transport.h"
#include "knx_ios.h"
#include "knx_lsm.h"
#include "knx_apci.h"
#include "Appl.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
** Global variables.
*/
extern const uint8_t KNX_OBJ_LEN_TAB[];
extern const uint8_t APP_CommObjTab[];
extern uint8_t DEV_Device_Control;
extern uint8_t DEV_Current_Accesslevel;
extern uint8_t DEV_SystemState;

/*
** Global function-like macros.
*/
#define KnxAl_GetAPDUShortData(pmsg, nbits)         ((uint8_t)(pmsg)->apci & (uint8_t)KNX_AL_SHORT_DATA_MASK[(nbits) - (uint8_t)1])
#define KnxAl_SetAPDUShortData(pmsg, data, nbits)   ((pmsg)->apci =      \
                                                         ((pmsg)->apci & \
                                                          ~(uint8_t)0x3f) | ((data) & KNX_AL_SHORT_DATA_MASK[(nbits) - (uint8_t)1])

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) DEV_SetVerifyMode(boolean on);
FUNC(void, KSTACK_CODE) DEV_ChkOwnPhysAddrRcvd(boolean on);
FUNC(void, KSTACK_CODE) DEV_PhysAddrWriteEnable(boolean on);
FUNC(void, KSTACK_CODE) KnxALM_Task(void);
FUNC(void, KSTACK_CODE) KnxALG_PollCycle(void);
FUNC(Knx_APCITypeType, KSTACK_CODE) KnxAl_GetAPCIType(const KNX_StandardFrameRefType pmsg);
FUNC(void, KSTACK_CODE) KnxAl_GetAPDUData(const KNX_StandardFrameRefType pmsg, uint8_t offset,
                                              P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) data
                                              uint8_t len
                                              );
FUNC(void, KSTACK_CODE) KnxAl_SetAPDUData(const KNX_StandardFrameRefType pmsg, uint8_t offset,
                                              P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) data
                                              uint8_t len
                                              );
FUNC(uint8_t, KSTACK_CODE) KnxAl_GetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8_t offset);
FUNC(void, KSTACK_CODE) KnxAl_SetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8_t offset, uint8_t value);
#else
void DEV_SetVerifyMode(boolean on);
void DEV_ChkOwnPhysAddrRcvd(boolean on);
void DEV_PhysAddrWriteEnable(boolean on);
void KnxALM_Task(void);
void KnxALG_PollCycle(void);
Knx_APCITypeType KnxAl_GetAPCIType(const KNX_StandardFrameRefType pmsg);
void KnxAl_GetAPDUData(const KNX_StandardFrameRefType pmsg, uint8_t offset, uint8_t * data, uint8_t len);
void KnxAl_SetAPDUData(const KNX_StandardFrameRefType pmsg, uint8_t offset, uint8_t * data, uint8_t len);
uint8_t KnxAl_GetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8_t offset);
void KnxAl_SetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8_t offset, const uint8_t value);


#endif /* KSTACK_MEMORY_MAPPING */

/*
**  Application-Layer-Services.
*/

#if KSTACK_MEMORY_MAPPING == STD_ON
/* Broadcast-Services. */
FUNC(void, KSTACK_CODE) A_IndividualAddress_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source);
FUNC(void, KSTACK_CODE) A_IndividualAddress_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source);
FUNC(void, KSTACK_CODE) A_IndividualAddress_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                      Knx_AddressType newaddress
                                                      );

FUNC(void, KSTACK_CODE) A_IndividualAddressSerialNumber_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                                 Knx_SerialNumberType serial_number, Knx_AddressType domain_addr
                                                                 );
FUNC(void, KSTACK_CODE) A_IndividualAddressSerialNumber_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                                 Knx_SerialNumberType serial_number
                                                                 );
FUNC(void, KSTACK_CODE) A_IndividualAddressSerialNumber_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                                  Knx_SerialNumberType serial_number, Knx_AddressType new_addr
                                                                  );

FUNC(void, KSTACK_CODE) A_ServiceInformation_Indication_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                                  uint8_t info
                                                                  );

FUNC(void, KSTACK_CODE) A_DomainAddress_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source);
FUNC(void, KSTACK_CODE) A_DomainAddress_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source);
FUNC(void, KSTACK_CODE) A_DomainAddress_Write_req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                  Knx_AddressType domain_ddress
                                                  );
FUNC(void, KSTACK_CODE) A_DomainAddressSelective_Read_req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                          Knx_AddressType domain_ddress, Knx_AddressType start_address, uint8_t range
                                                          );

FUNC(void, KSTACK_CODE) A_PropertyValue_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                 Knx_AddressType dest, uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem,
                                                 uint16_t start_index,
                                                 P2VAR(uint8_t, AUTOMATIC,
                                                       KSTACK_APPL_DATA) data
                                                 );
FUNC(void, KSTACK_CODE) A_PropertyValue_Read_Res_NoData(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                        Knx_AddressType dest, uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem,
                                                        uint16_t start_index
                                                        );
FUNC(void, KSTACK_CODE) A_PropertyValue_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                 Knx_AddressType dest,  uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem,
                                                 uint16_t start_index
                                                 );

/* todo: Prop_Write!!! */

FUNC(void, KSTACK_CODE) A_PropertyDescription_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                       Knx_AddressType dest,  uint8_t obj_index, uint8_t prop_id, uint8_t prop_index,
                                                       uint8_t type, uint16_t nr_of_elem,
                                                       uint8_t access
                                                       );
FUNC(void, KSTACK_CODE) A_PropertyDescription_Read_Res_NoData(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                              Knx_AddressType dest, uint8_t obj_index, uint8_t prop_id,
                                                              uint8_t prop_index
                                                              );
FUNC(void, KSTACK_CODE) A_PropertyDescription_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                       Knx_AddressType dest, uint8_t obj_index, uint8_t prop_id, uint8_t prop_index
                                                       );
#else
/* Broadcast-Services. */
void A_IndividualAddress_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source);
void A_IndividualAddress_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source);
void A_IndividualAddress_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType newaddress);
void A_IndividualAddressSerialNumber_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                              Knx_SerialNumberType serial_number, Knx_AddressType domain_addr
                                              );
void A_IndividualAddressSerialNumber_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                              Knx_SerialNumberType serial_number
                                              );
void A_IndividualAddressSerialNumber_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                               Knx_SerialNumberType serial_number, Knx_AddressType new_addr
                                               );

void A_ServiceInformation_Indication_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, uint8_t info);

void A_DomainAddress_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source);
void A_DomainAddress_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source);
void A_DomainAddress_Write_req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType domain_ddress);
void A_DomainAddressSelective_Read_req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                          Knx_AddressType domain_ddress, Knx_AddressType start_address, uint8_t range
                                          );

void A_PropertyValue_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                              uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem, uint16_t start_index, uint8_t * data
                              );
void A_PropertyValue_Read_Res_NoData(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                     uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem, uint16_t start_index
                                     );
void A_PropertyValue_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                              uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem, uint16_t start_index
                              );


/* todo: Prop_Write!!! */

void A_PropertyDescription_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                    uint8_t obj_index, uint8_t prop_id, uint8_t prop_index, uint8_t type, uint16_t nr_of_elem, uint8_t access
                                    );
void A_PropertyDescription_Read_Res_NoData(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                           Knx_AddressType dest, uint8_t obj_index, uint8_t prop_id, uint8_t prop_index
                                           );
void A_PropertyDescription_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                    uint8_t obj_index, uint8_t prop_id, uint8_t prop_index
                                    );


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_LAYER_APPLICATION_H */

