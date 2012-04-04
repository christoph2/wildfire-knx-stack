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
#if !defined(__KNX_ALG_H)
#define __KNX_ALG_H

#include "knx_layer_application.h"
#include "knx_messaging.h"
#include "Appl.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
** Global function-like macros.
*/
#define KnxALG_GetCommObjDescr(objectNumber)    ((Knx_CommObjDescriptorType *)((uint8 *)APP_CommObjTab + (uint8)2) + (objectNumber))
#define KnxALG_GetNumCommObjs()                 ((uint8)APP_CommObjTab[0])

#define KnxALG_ObjCheckEnabled(f, m)            (((f) & (m)) == (m))

#define KnxALG_ObjTransmitEnabled(f)            KnxALG_ObjCheckEnabled((f), (KNX_OBJ_COMM_ENABLE | KNX_OBJ_TRANSMIT_ENABLE))
#define KnxALG_ObjReadEnabled(f)                KnxALG_ObjCheckEnabled((f), (KNX_OBJ_COMM_ENABLE | KNX_OBJ_READ_ENABLE))
#define KnxALG_ObjResponseEnabled(f)            KnxALG_ObjCheckEnabled((f),                                          \
                                                                       (KNX_OBJ_COMM_ENABLE | KNX_OBJ_WRITE_ENABLE | \
                                                                        KNX_OBJ_UPDATE_ENABLE))
#define KnxALG_ObjWriteEnabled(f)               KnxALG_ObjCheckEnabled((f), (KNX_OBJ_COMM_ENABLE | KNX_OBJ_WRITE_ENABLE))

#define KnxALG_GetTransmissionStatus(objectNr)  (KnxALG_GetRAMFlags((objectNr)) & (uint8)0x03)
#define KnxALG_IsObjectTransmitting(objectNr)   ((KnxALG_GetTransmissionStatus((objectNr)) & KNX_OBJ_TRANSMIT_REQ) == \
                                                 KNX_OBJ_TRANSMIT_REQ)
#define KnxALG_IsObjectBusy(objectNr)           (KnxALG_IsObjectTransmitting((objectNr)))

#define KnxALG_GetObjLen(o)                     (KNX_OBJ_LEN_TAB[((o) & (uint8)0x3f)])
#define KnxALG_GetObjPriority(objectNr)         ((Knx_PriorityType)(KnxALG_GetCommObjDescr((objectNr))->Config) & (uint8)0x03)

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8 *, KSTACK_CODE)  KnxALG_GetObjectDataPointer(uint8 objectNr);

FUNC(void, KSTACK_CODE)     KnxALG_SetRAMFlags(uint16 objectNr, uint8 flags);
FUNC(uint8, KSTACK_CODE)    KnxALG_GetRAMFlags(uint16 objectNr);
FUNC(uint8 *, KSTACK_CODE)  KnxALG_GetRAMFlagPointer(void);

FUNC(void, KSTACK_CODE)     KnxALG_UpdateAssociatedASAPs(KnxMSG_BufferPtr pBuffer, uint8 testFlags);

FUNC(void, KSTACK_CODE)     KnxALG_Task(void);
FUNC(void, KSTACK_CODE)     KnxALG_Init(void);

/* Group-Services. */
FUNC(void, KSTACK_CODE)     A_GroupValue_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                  Knx_AddressType dest, uint8 prio);
FUNC(void, KSTACK_CODE)     A_GroupValue_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                   Knx_AddressType dest, Knx_PriorityType prio,
                                                   P2VAR(uint8, AUTOMATIC, KSTACK_APPL_DATA) data, uint8 len);
#else
uint8 * KnxALG_GetObjectDataPointer(uint8 objectNr);

void    KnxALG_SetRAMFlags(uint16 objectNr, uint8 flags);
uint8   KnxALG_GetRAMFlags(uint16 objectNr);
uint8 * KnxALG_GetRAMFlagPointer(void);

void KnxALG_UpdateAssociatedASAPs(KnxMSG_BufferPtr pBuffer, uint8 testFlags);

void    KnxALG_Task(void);
void    KnxALG_Init(void);


/* Group-Services. */
void    A_GroupValue_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8 prio);
void    A_GroupValue_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, Knx_PriorityType prio,
                               uint8 * data,
                               uint8 len);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_ALG_H */
