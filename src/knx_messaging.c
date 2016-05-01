/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
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
#include "knx_messaging.h"
#include <stdio.h>

#define HOP_COUNT ((uint8_t)6)


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(KnxMsg_Buffer *, KSTACK_CODE) KnxMsg_GetBufferAddress(uint8_t buf_num);
STATIC FUNC(uint8_t, KSTACK_CODE) KnxMsg_GetBufferNumber(const KnxMsg_Buffer * buffer);
STATIC FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_ClearMessageBuffer(uint8_t buf_num);
#else
STATIC KnxMsg_Buffer * KnxMsg_GetBufferAddress(uint8_t buf_num);
STATIC uint8_t KnxMsg_GetBufferNumber(const KnxMsg_Buffer * buffer);
STATIC Knx_StatusType KnxMsg_ClearMessageBuffer(uint8_t buf_num);
#endif /* KSTACK_MEMORY_MAPPING */

/*
** Debug-API
*/
#if defined(KNX_BUILD_DEBUG)
#define INC_USED_BUFFERS()  KnxMsg_BuffersUsed += 1;
#define INC_FREE_BUFFERS()  KnxMsg_BuffersFree += 1;
#define DEC_USED_BUFFERS()  KnxMsg_BuffersUsed -= 1;
#define DEC_FREE_BUFFERS()  KnxMsg_BuffersFree -= 1;

STATIC uint8_t KnxMsg_BuffersFree = MSG_NUM_BUFFERS;
STATIC uint8_t KnxMsg_BuffersUsed = 0;

#else
#define INC_USED_BUFFERS()
#define INC_FREE_BUFFERS()
#define DEC_USED_BUFFERS()
#define DEC_FREE_BUFFERS()
#endif  /* KNX_BUILD_DEBUG */


STATIC const uint8_t KnxMsg_MessageRedirectionTable[16] = {
    TASK_FREE_ID, TASK_LL_ID,   TASK_NL_ID,   TASK_TL_ID,   TASK_TC_ID,     TASK_FREE_ID,     TASK_FREE_ID,   TASK_AL_ID,
    TASK_MG_ID,   TASK_MG_ID,   TASK_PM_ID,   TASK_LC_ID,   TASK_FREE_ID,   TASK_US_ID,       TASK_US_ID,     TASK_US_ID
};

STATIC uint8_t KnxMsg_Queues[MSG_NUM_TASKS];
STATIC KnxMsg_Buffer KnxMsg_Buffers[MSG_NUM_BUFFERS];

KNX_IMPLEMENT_MODULE_STATE_VAR(MSG);

/*  Get Destination Queue from Message-Code. */
#define KnxMsg_GetQueueForService(service) ((uint8_t)KnxMsg_MessageRedirectionTable[(service) >> 4])


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_Init(void)
#else
void KnxMsg_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t idx;

//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_INIT);

    KNX_MODULE_INITIALIZE(MSG);

    for (idx = (uint8_t)0; idx < MSG_NUM_BUFFERS; ++idx) {
        (void)KnxMsg_ClearMessageBuffer(idx);
    }

    KnxMsg_Queues[TASK_FREE_ID] = (uint8_t)0x00;      /* The first queue contains the free-list. */

    for (idx = (uint8_t)0; idx < MSG_NUM_BUFFERS; ++idx) { /* Setup free-list. */
        KnxMsg_Buffers[idx].next = (idx + (uint8_t)1);
    }

    KnxMsg_Buffers[MSG_NUM_BUFFERS - 1].next = MSG_NO_NEXT;

    for (idx = (uint8_t)1; idx < MSG_NUM_TASKS; ++idx) {
        KnxMsg_Queues[idx] = MSG_QUEUE_EMPTY;
    }
#if defined(KNX_BUILD_DEBUG)
    KnxMsg_BuffersFree = MSG_NUM_BUFFERS;
    KnxMsg_BuffersUsed = 0;
#endif

//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_INIT);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_Deinit(void)
#else
void KnxMsg_Deinit(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KNX_MODULE_UNINITIALIZE(MSG);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_AllocateBuffer(KnxMsg_Buffer ** buffer)
#else
Knx_StatusType KnxMsg_AllocateBuffer(KnxMsg_Buffer ** buffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t fp;
    KnxMsg_Buffer * ptr;
    KnxMsg_Buffer * result;

//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER);

    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER, KNX_E_NOT_OK);
    if (buffer == NULL) {
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER, MSG_E_NULL_PTR);
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER);
        return KNX_E_NOT_OK;
    }

    DISABLE_ALL_INTERRUPTS();

    if ((fp = KnxMsg_Queues[TASK_FREE_ID]) == MSG_NO_NEXT) {
        ENABLE_ALL_INTERRUPTS();
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER, MSG_E_NO_BUFFER_AVAIL);
        buffer = (KnxMsg_Buffer **)NULL;
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER);
        return KNX_E_NOT_OK;       /* No buffer available. */
    }

    ptr = &KnxMsg_Buffers[fp];

    if (ptr->next == MSG_NO_NEXT) {
        KnxMsg_Queues[TASK_FREE_ID] = MSG_NO_NEXT;
    } else {
        KnxMsg_Queues[TASK_FREE_ID] = ptr->next;
        ptr->next = MSG_NO_NEXT;
    }

    ENABLE_ALL_INTERRUPTS();
    result = &KnxMsg_Buffers[fp];
    *buffer = result;

    INC_USED_BUFFERS();
    DEC_FREE_BUFFERS();

//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER);
    return KNX_E_OK;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMsg_Buffer *, KSTACK_CODE) KnxMsg_AllocateBufferWrapper(void)
#else
KnxMsg_Buffer * KnxMsg_AllocateBufferWrapper(void)
#endif
{
    KnxMsg_Buffer * buffer = NULL;

    if (KnxMsg_AllocateBuffer(&buffer) == KNX_E_OK) {

        return buffer;
    } else {
        return NULL;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_ReleaseBuffer(KnxMsg_Buffer * ptr)
#else
Knx_StatusType KnxMsg_ReleaseBuffer(KnxMsg_Buffer * ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   buf_num;
    uint8_t   old_fp;
    uint8_t   t_fp;

//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_RELEASE_BUFFER);

    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_RELEASE_BUFFER, KNX_E_NOT_OK);
    if (ptr == NULL) {
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_RELEASE_BUFFER, MSG_E_NULL_PTR);
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_RELEASE_BUFFER);
        return KNX_E_NOT_OK;
    }

    DISABLE_ALL_INTERRUPTS();

    if ((buf_num = KnxMsg_GetBufferNumber(ptr)) == MSG_INVALID_BUFFER) {
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_RELEASE_BUFFER, MSG_E_INVALID_BUFFER);
        ENABLE_ALL_INTERRUPTS();
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_RELEASE_BUFFER);
        return KNX_E_NOT_OK;
    }

    old_fp = KnxMsg_Queues[TASK_FREE_ID];
    t_fp   = old_fp;

    while (t_fp != MSG_NO_NEXT) {
        if (t_fp == buf_num) {
            ENABLE_ALL_INTERRUPTS();
            KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_RELEASE_BUFFER, MSG_E_NOT_ALLOCATED);
//            Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_RELEASE_BUFFER);
            return KNX_E_NOT_OK;   /* Not allocated. */
        }

        t_fp = KnxMsg_Buffers[t_fp].next;
    }

    KnxMsg_Queues[TASK_FREE_ID]    = buf_num;
    KnxMsg_Buffers[buf_num].next   = old_fp;
    (void)KnxMsg_ClearMessageBuffer(buf_num);

    ptr = (KnxMsg_Buffer *)NULL;  /* Invalidate Buffer. */
    ENABLE_ALL_INTERRUPTS();
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_RELEASE_BUFFER);

    INC_FREE_BUFFERS();
    DEC_USED_BUFFERS();

    return KNX_E_OK;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_ClearBuffer(KnxMsg_Buffer * ptr)
#else
Knx_StatusType KnxMsg_ClearBuffer(KnxMsg_Buffer * ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t * pb;

//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_CLEAR_BUFFER);

    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER, KNX_E_NOT_OK);
    if (ptr == NULL) {
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_CLEAR_BUFFER, MSG_E_NULL_PTR);
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_CLEAR_BUFFER);
        return KNX_E_NOT_OK;
    }

    pb = (uint8_t *)ptr;
    pb++;

    Utl_MemSet(pb, (uint8_t)'\0', (uint16_t)(sizeof(KnxMsg_Buffer) - 1));
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_CLEAR_BUFFER);
    return KNX_E_OK;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_Post(KnxMsg_Buffer * ptr)
#else
Knx_StatusType KnxMsg_Post(KnxMsg_Buffer * ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   queue;
    uint8_t   buf_num;
    uint8_t   qp;

//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_POST);

    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_POST, KNX_E_NOT_OK);

    if ((buf_num = KnxMsg_GetBufferNumber(ptr)) == MSG_INVALID_BUFFER) {
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_POST, MSG_E_INVALID_BUFFER);
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_POST);
        return KNX_E_NOT_OK;
    }

    if ((queue = KnxMsg_GetQueueForService(ptr->service)) == TASK_FREE_ID) {
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_POST, MSG_E_NO_BUFFER_AVAIL);
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_POST);
        return KNX_E_NOT_OK;
    }

    qp = KnxMsg_Queues[queue];

    if (qp == MSG_QUEUE_EMPTY) {
        KnxMsg_Queues[queue] = buf_num;
    } else {

        while (KnxMsg_Buffers[qp].next != MSG_QUEUE_EMPTY) {
            qp = KnxMsg_Buffers[qp].next;
        }
        KnxMsg_Buffers[qp].next = buf_num;
    }
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_POST);
    return KNX_E_OK;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMsg_Buffer *, KSTACK_CODE) KnxMsg_Get(uint8_t task)
#else
KnxMsg_Buffer * KnxMsg_Get(uint8_t task)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t qp;

//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET);
    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_GET, NULL);

    if ((task < 1) || (task > MSG_NUM_TASKS)) {
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_GET, MSG_E_INVALID_BUFFER);
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET);
        return (KnxMsg_Buffer *)NULL;
    }

    if ((qp = KnxMsg_Queues[task]) == MSG_QUEUE_EMPTY) {    // Check: Possibly out of bounds?
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET);
        return (KnxMsg_Buffer *)NULL;          /* No message for task. */
    }

    if (KnxMsg_Buffers[qp].next != MSG_QUEUE_EMPTY) {
        KnxMsg_Queues[task] = KnxMsg_Buffers[qp].next;  // Check: Possibly out of bounds?
        KnxMsg_Buffers[qp].next = MSG_NO_NEXT;  /* Unlink buffer. */
    } else {
        KnxMsg_Queues[task] = MSG_QUEUE_EMPTY;  // Check: Possibly out of bounds?
    }
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET);
    return &KnxMsg_Buffers[qp];
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_SetLen(KnxMsg_Buffer * pBuffer, uint8_t len)
#else
void KnxMsg_SetLen(KnxMsg_Buffer * pBuffer, uint8_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{
//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_SET_LEN);
    KNX_ASSERT_MODULE_IS_INITIALIZED(MSG, AR_SERVICE_MSG_SET_LEN);

    pBuffer->len = len;
    KnxMsg_GetStandardFramePtr(pBuffer).npci |= ((len - (uint8_t)7) & (uint8_t)0x0f);
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_SET_LEN);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxMsg_GetLen(const KnxMsg_Buffer * pBuffer)
#else
uint8_t KnxMsg_GetLen(const KnxMsg_Buffer * pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_LEN);
    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER, 0);
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_LEN);
    return pBuffer->len;
}


/*
   void MSG_SetHopCount(PMSG_Buffer pBuffer,uint8_t hop_count)
   {
    MSG_GetMessagePtr(pBuffer)->npci|=((hop_count & 0x07)<<4);
   }

   void MSG_GetHopCount(PMSG_Buffer pBuffer,uint8_t *hop_count)
   {
   *hop_count=((MSG_GetMessagePtr(pBuffer)->npci) & 0x70)>>4;
   }
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_SetRoutingCount(KnxMsg_Buffer * pBuffer)
#else
void KnxMsg_SetRoutingCount(KnxMsg_Buffer * pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   ctrl;
    uint8_t   hop_count;

//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_SET_ROUTING_COUNT);
    KNX_ASSERT_MODULE_IS_INITIALIZED(MSG, AR_SERVICE_MSG_SET_ROUTING_COUNT);

    ctrl = KnxMsg_GetStandardFramePtr(pBuffer).ctrl;

    if ((ctrl & (uint8_t)0x02) == (uint8_t)0x02) {
        hop_count = MSG_NO_ROUTING_CTRL;
        ctrl &= ~(uint8_t)0x02;        // TODO: Check!!!
        KnxMsg_GetStandardFramePtr(pBuffer).ctrl    = ctrl;
    } else {
        hop_count = HOP_COUNT;
    }

    KnxMsg_GetStandardFramePtr(pBuffer).npci |= ((hop_count & (uint8_t)0x07) << 4);
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_SET_ROUTING_COUNT);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxMsg_GetRoutingCount(const KnxMsg_Buffer * pBuffer)
#else
uint8_t KnxMsg_GetRoutingCount(const KnxMsg_Buffer * pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_ROUTING_COUNT);
    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_GET_ROUTING_COUNT, 0x00);
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_ROUTING_COUNT);

    return ((KnxMsg_GetStandardFramePtr(pBuffer).npci) & (uint8_t)0x70) >> 4;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_SetRoutingCtrl(KnxMsg_Buffer * pBuffer, boolean on)
#else
void KnxMsg_SetRoutingCtrl(KnxMsg_Buffer * pBuffer, boolean on)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t r;

    //Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_ROUTING_COUNT);
//    KNX_ASSERT_MODULE_IS_INITIALIZED(MSG, AR_SERVICE_MSG_SET_ROUTING_COUNT);

    (on == TRUE) ? (r = (uint8_t)0x02) : (r = (uint8_t)0x00);
    KnxMsg_GetStandardFramePtr(pBuffer).ctrl |= r;
    //Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_ROUTING_COUNT);
}

/*
**
**  Local Functions.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(KnxMsg_Buffer *, KSTACK_CODE) KnxMsg_GetBufferAddress(uint8_t buf_num)
#else
STATIC KnxMsg_Buffer * KnxMsg_GetBufferAddress(uint8_t buf_num)
#endif /* KSTACK_MEMORY_MAPPING */
{
//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_BUFFER_ADDRESS);
    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_GET_BUFFER_ADDRESS, NULL);

    if (buf_num >= MSG_NUM_BUFFERS) {
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_GET_BUFFER_ADDRESS, MSG_E_INVALID_BUFFER);
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_BUFFER_ADDRESS);
        return (KnxMsg_Buffer *)NULL;
    }
    else {
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_BUFFER_ADDRESS);
        return &KnxMsg_Buffers[buf_num];
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(uint8_t, KSTACK_CODE) KnxMsg_GetBufferNumber(const KnxMsg_Buffer * buffer)
#else
STATIC uint8_t KnxMsg_GetBufferNumber(const KnxMsg_Buffer * buffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_Buffer * tmp_buf;
    uint8_t idx;

//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_BUFFER_NUMBER);

    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_GET_BUFFER_NUMBER, MSG_INVALID_BUFFER);

    for (idx = (uint8_t)0; idx < MSG_NUM_BUFFERS; idx++) {
        tmp_buf = &KnxMsg_Buffers[idx];

        if (tmp_buf == buffer) {
//            Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_BUFFER_NUMBER);
            return idx;
        }
    }
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_GET_BUFFER_NUMBER);
    return MSG_INVALID_BUFFER;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_ClearMessageBuffer(uint8_t buf_num)
#else
STATIC Knx_StatusType KnxMsg_ClearMessageBuffer(uint8_t buf_num)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_Buffer * ptr;
    uint8_t * pb;

//    Dbg_TraceFunctionEntry(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_CLEAR_MESSAGE_BUFFER);
    KNX_ASSERT_MODULE_IS_INITIALIZED_RETURN(MSG, AR_SERVICE_MSG_ALLOCATE_BUFFER, KNX_E_NOT_OK);

    ptr = KnxMsg_GetBufferAddress(buf_num);

    if (ptr == (KnxMsg_Buffer *)NULL) {
        KNX_RAISE_DEV_ERROR(MSG, AR_SERVICE_MSG_CLEAR_MESSAGE_BUFFER, MSG_E_NULL_PTR);
//        Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_CLEAR_MESSAGE_BUFFER);
        return KNX_E_NOT_OK;
    }

    pb = (uint8_t *)ptr;
    pb++;

    Utl_MemSet(pb, '\0', (uint16_t)sizeof(KnxMsg_Buffer) - (uint16_t)1);
//    Dbg_TraceFunctionExit(KNX_MODULE_ID_MSG, AR_SERVICE_MSG_CLEAR_MESSAGE_BUFFER);
    return KNX_E_OK;
}

/*
**  Debugging API.
*/
#if defined(KNX_BUILD_DEBUG)
void KnxMsg_DebugGetBufferCounters(KnxMsg_DebugBufferCounters * counters)
{
    counters->free = KnxMsg_BuffersFree;
    counters->used = KnxMsg_BuffersUsed;
}
#endif

/*
   boolean MSG_GetRoutingCtrl(const PMSG_Buffer pBuffer)
   {
    return ((MSG_GetMessagePtr(pBuffer)->ctrl & 0x02) == 0x02);
   }
 */

/*
   uint8_t MSG_GetLSDULen(PMSG_Buffer pBuffer)
   {
    return MSG_GetMessagePtr(pBuffer)->npci & 0x0f;
   }

   void MSG_SetLSDULen(PMSG_Buffer pBuffer,uint8_t len_lsdu)
   {
    MSG_GetMessagePtr(pBuffer)->npci=(len_lsdu & 0x0f);
   }
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

