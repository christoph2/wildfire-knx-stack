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
#if !defined(__KNX_MESSAGING_HPP)
#define __KNX_MESSAGING_HPP

#include "knx_address.hpp"
#include "knx_macros.hpp"
#include "knx_et.hpp"
#include "Wildfire_Config.hpp"

namespace knx{


/*
**  Service-IDs.
*/
#define AR_SERVICE_MSG_INIT                     ((uint8_t)0x00)
#define AR_SERVICE_MSG_ALLOCATE_BUFFER          ((uint8_t)0x01)
#define AR_SERVICE_MSG_ALLOCATE_BUFFER_WRAPPER  ((uint8_t)0x02)
#define AR_SERVICE_MSG_RELEASE_BUFFER           ((uint8_t)0x03)
#define AR_SERVICE_MSG_CLEAR_BUFFER             ((uint8_t)0x04)
#define AR_SERVICE_MSG_POST                     ((uint8_t)0x05)
#define AR_SERVICE_MSG_GET                      ((uint8_t)0x06)
#define AR_SERVICE_MSG_SET_LEN                  ((uint8_t)0x07)
#define AR_SERVICE_MSG_GET_LEN                  ((uint8_t)0x08)
#define AR_SERVICE_MSG_SET_ROUTING_COUNT        ((uint8_t)0x09)
#define AR_SERVICE_MSG_GET_ROUTING_COUNT        ((uint8_t)0x0a)
#define AR_SERVICE_MSG_GET_BUFFER_ADDRESS       ((uint8_t)0x0b)
#define AR_SERVICE_MSG_GET_BUFFER_NUMBER        ((uint8_t)0x0c)
#define AR_SERVICE_MSG_CLEAR_MESSAGE_BUFFER     ((uint8_t)0x0d)

/*
**  Module-Errors.
*/
#define MSG_E_UNINIT                            ((uint8_t)0x01)
#define MSG_E_NULL_PTR                          ((uint8_t)0x02)
#define MSG_E_NO_BUFFER_AVAIL                   ((uint8_t)0x03)
#define MSG_E_INVALID_BUFFER                    ((uint8_t)0x04)
#define MSG_E_NOT_ALLOCATED                     ((uint8_t)0x05)

/*
** Global defines.
*/
#define MSG_NO_ROUTING_CTRL ((uint8_t)7)

#define MSG_NUM_TASKS       ((uint8_t)10)
#define MSG_NUM_BUFFERS     ((uint8_t)4)

#define MSG_QUEUE_EMPTY     ((uint8_t)0xff)
#define MSG_INVALID_BUFFER  ((uint8_t)0xff)
#define MSG_NO_NEXT         ((uint8_t)0xff)

#define TPCI_INVALID        ((uint8_t)0xbf)
#define APCI_INVALID        ((uint16_t)0x3FF)

#define TASK_FREE_ID        ((uint8_t)0)          /* Free message buffers.                   */
#define TASK_LL_ID          ((uint8_t)1)          /* Link-Layer.                             */
#define TASK_NL_ID          ((uint8_t)2)          /* Network-Layer.                          */
#define TASK_TL_ID          ((uint8_t)3)          /* Transport-Layer/Multicast.              */
#define TASK_TC_ID          ((uint8_t)4)          /* Transport-Layer/Multicast&one-to-one.   */
#define TASK_LC_ID          ((uint8_t)5)          /* Transport-Layer/Local.                  */
#define TASK_AL_ID          ((uint8_t)6)          /* Application-Layer/Group-Oriented.       */
#define TASK_MG_ID          ((uint8_t)7)          /* Application-Layer/Managment.            */
#define TASK_PM_ID          ((uint8_t)8)          /* PEI-System.                             */
#define TASK_US_ID          ((uint8_t)9)          /* User.                                   */
#define TASK_INVALID_ID     ((uint8_t)0xff)       /* Invalid Queue.                          */

/* todo: Support f. Extended-Frames!!! */
#define MSG_LEN             ((uint8_t)22)
#define MAX_ADPU_LEN        ((uint8_t)14)
#define MAX_PROP_DATA_LEN   ((uint8_t)10)


/*
** Global function-like macros.
*/
#define KnxMsg_GetRawFramePtr(pBuffer)          ((pBuffer)->msg.raw)
#define KnxMsg_GetStandardFramePtr(pBuffer)     ((pBuffer)->msg.standard)
#define KnxMsg_GetProperyFramePtr(pBuffer)      ((pBuffer)->msg.property)
#define KnxMsg_GetPollingFramePtr(pBuffer)      ((pBuffer)->msg.polling)

/**************/
#define KnxMsg_GetFrameType(pBuffer)            ((KNX_FrameTypeType)(KnxMsg_GetStandardFramePtr((pBuffer)).ctrl) & (uint8_t)0xc0)
#define KnxMsg_SetFrameType(pBuffer, type)      (KnxMsg_GetStandardFramePtr((pBuffer)).ctrl |= ((type) & (uint8_t)0xc0))

#define KnxMsg_GetSourceAddress(pBuffer)        ((Knx_AddressType)btohs(*(uint16_t *)KnxMsg_GetStandardFramePtr((pBuffer)).source))
#define KnxMsg_GetDestAddress(pBuffer)          ((Knx_AddressType)btohs(*(uint16_t *)KnxMsg_GetStandardFramePtr((pBuffer)).dest))
#define KnxMsg_SetSourceAddress(pBuffer, addr)  (*(uint16_t *)&KnxMsg_GetStandardFramePtr((pBuffer)).source = Utl_Htons((addr)))
#define KnxMsg_SetDestAddress(pBuffer, addr)    (*(uint16_t *)&KnxMsg_GetStandardFramePtr((pBuffer)).dest = Utl_Htons((addr)))

#define KnxMsg_GetPriority(pBuffer)             ((KNX_PriorityType)(KnxMsg_GetStandardFramePtr((pBuffer)).ctrl & (uint8_t)0x0C) >> 2)
#define KnxMsg_SetPriority(pBuffer, priority)   (KnxMsg_GetStandardFramePtr((pBuffer)).ctrl |= (((priority) & (uint8_t)0x03) << 2))

#define KnxMsg_GetAddressType(pBuffer)          ((uint8_t)KnxMsg_GetStandardFramePtr((pBuffer)).npci & (uint8_t)0x80)
#define KnxMsg_SetAddressType(pBuffer, at)      (KnxMsg_GetStandardFramePtr((pBuffer)).npci |= ((at) & (uint8_t)0x80))

#define KnxMsg_IsMulticastAddressed(pBuffer)    (KnxMsg_GetAddressType((pBuffer)) == KNX_ADDR_MULTICAST)
#define KnxMsg_IsIndividualAddressed(pBuffer)   (KnxMsg_GetAddressType((pBuffer)) == KNX_ADDR_INDIVIDUAL)

#define KnxMsg_GetLSDULen(pBuffer)              (KnxMsg_GetStandardFramePtr((pBuffer).npci & (uint8_t)0x0f)
#define KnxMsg_SetLSDULen(pBuffer, len_lsdu)    (KnxMsg_GetStandardFramePtr((pBuffer)).npci |= ((len_lsdu) & (uint8_t)0x0f))

#define KnxMsg_GetTPCI(pBuffer)                 ((uint8_t)KnxMsg_GetStandardFramePtr((pBuffer)).tpci)
#define KnxMsg_SetTPCI(pBuffer, tp)             (KnxMsg_GetStandardFramePtr((pBuffer)).tpci |= (tp))

#define KnxMsg_GetSeqNo(pBuffer)                ((uint8_t)((KnxMsg_GetStandardFramePtr((pBuffer)).tpci) & (uint8_t)0x3c) >> 2)
#define KnxMsg_SetSeqNo(pBuffer, SeqNo)         (KnxMsg_GetStandardFramePtr((pBuffer)).tpci |= (((SeqNo) & (uint8_t)0x0f) << 2))

#define KnxMsg_GetAPCI(pBuffer)                 ((uint16_t)(KnxMsg_GetStandardFramePtr((pBuffer)).tpci << \
                                                          8) | (KnxMsg_GetStandardFramePtr((pBuffer)).apci))
#define KnxMsg_SetAPCI(pBuffer, apci)           (*(uint16_t *)&(pBuffer)->msg.raw[6] = Utl_Htons((apci)))

/*
** Global Types.
*/
using KnxMsg_MessageType = uint8_t[MSG_LEN];

struct KnxMsg_StandardFrameType {
    uint8_t   ctrl;
    uint8_t   source[2];
    uint8_t   dest[2];
    uint8_t   npci;
    uint8_t   tpci;
    uint8_t   apci;
    uint8_t   data[MAX_ADPU_LEN];
};

struct KnxMsg_PropertyFrameType {
    uint8_t   ctrl;
    uint8_t   source[2];
    uint8_t   dest[2];
    uint8_t   npci;
    uint8_t   tpci;
    uint8_t   apci;
    uint8_t   obj_id;
    uint8_t   prop_id;
    uint8_t   num_elems;
    uint8_t   start_index;
    uint8_t   data[MAX_PROP_DATA_LEN];
}; 

struct KnxMsg_PollingFrameType {
    uint8_t   ctrl;
    uint8_t   source[2];
    uint8_t   poll_addr[2];
    uint8_t   num_slots;
    uint8_t   slot[MAX_ADPU_LEN];
};

struct KnxMsg_Buffer {
    uint8_t next;
    uint8_t len;
    Knx_ServiceTypeType service;
    uint8_t sap;
    Knx_StatusType status;
    union {
        KnxMsg_MessageType raw;
        KnxMsg_StandardFrameType standard;
        KnxMsg_PropertyFrameType property;
        KnxMsg_PollingFrameType polling;
    } msg;
};

struct KnxMsg_DebugBufferCounters {
    uint8_t used;
    uint8_t free;
};

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_Init(void);
FUNC(void, KSTACK_CODE) KnxMsg_Deinit(void);
FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_AllocateBuffer(KnxMsg_Buffer ** buffer);
FUNC(KnxMsg_Buffer *, KSTACK_CODE) KnxMsg_AllocateBufferWrapper(void);
FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_ReleaseBuffer(KnxMsg_Buffer * ptr);
FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_ClearBuffer(KnxMsg_Buffer * ptr);
FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_Post(KnxMsg_Buffer * ptr);
FUNC(KnxMsg_Buffer *, KSTACK_CODE) KnxMsg_Get(uint8_t task);

FUNC(void, KSTACK_CODE) KnxMsg_SetLen(KnxMsg_Buffer * pBuffer, uint8_t len);
FUNC(uint8_t, KSTACK_CODE) KnxMsg_GetLen(const KnxMsg_Buffer * pBuffer);

FUNC(uint8_t, KSTACK_CODE) KnxMsg_GetRoutingCount(const KnxMsg_Buffer * pBuffer);
FUNC(void, KSTACK_CODE) KnxMsg_SetRoutingCount(KnxMsg_Buffer * pBuffer);

FUNC(void, KSTACK_CODE) KnxMsg_SetRoutingCtrl(KnxMsg_Buffer * pBuffer, bool on);
FUNC(bool, KSTACK_CODE) KnxMsg_GetRoutingCtrl(const KnxMsg_Buffer * pBuffer);

FUNC(void, KSTACK_CODE) KnxMsg_DebugGetBufferCounters(KnxMsg_DebugBufferCounters * counters);
FUNC(void, KSTACK_CODE) KnxMsg_DebugGetBuffers(KnxMsg_Buffer * buffers[]);
#else
void KnxMsg_Init(void);
void KnxMsg_Deinit(void);
Knx_StatusType KnxMsg_AllocateBuffer(KnxMsg_Buffer ** buffer);
KnxMsg_Buffer * KnxMsg_AllocateBufferWrapper(void);
Knx_StatusType KnxMsg_ReleaseBuffer(KnxMsg_Buffer * ptr);
Knx_StatusType KnxMsg_ClearBuffer(KnxMsg_Buffer * ptr);
Knx_StatusType KnxMsg_Post(KnxMsg_Buffer * ptr);
KnxMsg_Buffer * KnxMsg_Get(uint8_t task);

void KnxMsg_SetLen(KnxMsg_Buffer * pBuffer, uint8_t len);
uint8_t KnxMsg_GetLen(const KnxMsg_Buffer * pBuffer);

uint8_t KnxMsg_GetRoutingCount(const KnxMsg_Buffer * pBuffer);
void KnxMsg_SetRoutingCount(KnxMsg_Buffer * pBuffer);

void KnxMsg_SetRoutingCtrl(KnxMsg_Buffer * pBuffer, bool on);
bool KnxMsg_GetRoutingCtrl(const KnxMsg_Buffer * pBuffer);

void KnxMsg_DebugGetBufferCounters(KnxMsg_DebugBufferCounters * counters);
void KnxMsg_DebugGetBuffers(KnxMsg_Buffer * buffers[]);

#endif /* KSTACK_MEMORY_MAPPING */

/* uint8_t GetQueueForService(uint8_t service); */
/* PMSG_Buffer GetBufferAddress(uint8_t buf_num); */
/* uint8_t GetBufferNumber(PMSG_Buffer addr); */
/* void ClearMessageBuffer(uint8_t buf_num); */
/* void MSG_Test(void); */
/* void MSG_SetHopCount(PMSG_Buffer pBuffer,uint8_t hop_count); */
/* void MSG_GetHopCount(PMSG_Buffer pBuffer,uint8_t *hop_count); */

} // namespace knx

#endif  /*__KNX_MESSAGING_HPP */

