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
#include "knx_messaging.h"

#define HOP_COUNT ((uint8_t)6)

/* check: 'static' ??? */
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMsg_BufferPtr, KSTACK_CODE) GetBufferAddress(uint8_t buf_num);
FUNC(uint8_t, KSTACK_CODE)                  GetBufferNumber(const KnxMsg_BufferPtr buffer);
FUNC(void, KSTACK_CODE)                  ClearMessageBuffer(uint8_t buf_num);
#else
KnxMsg_BufferPtr    GetBufferAddress(uint8_t buf_num);
uint8_t             GetBufferNumber(const KnxMsg_BufferPtr buffer);
void                ClearMessageBuffer(uint8_t buf_num);


#endif /* KSTACK_MEMORY_MAPPING */

STATIC const uint8_t KnxMsg_MessageRedirectionTable[16] = {
    TASK_FREE_ID, TASK_LL_ID,   TASK_NL_ID,   TASK_TL_ID,   TASK_TC_ID,     TASK_FREE_ID,     TASK_FREE_ID,   TASK_AL_ID,
    TASK_MG_ID,   TASK_MG_ID,   TASK_PM_ID,   TASK_LC_ID,   TASK_FREE_ID,   TASK_US_ID,       TASK_US_ID,     TASK_US_ID
};

STATIC uint8_t            KnxMsg_Queues[MSG_NUM_TASKS];
STATIC KnxMsg_Buffer    KnxMsg_Buffers[MSG_NUM_BUFFERS];

/*  Get Destination Queue from Message-Code. */
#define KnxMsg_GetQueueForService(service) ((uint8_t)KnxMsg_MessageRedirectionTable[(service) >> 4])

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMsg_BufferPtr, KSTACK_CODE) GetBufferAddress(uint8_t buf_num)
#else
KnxMsg_BufferPtr GetBufferAddress(uint8_t buf_num)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (buf_num >= MSG_NUM_BUFFERS) {
        return (KnxMsg_BufferPtr)NULL;
    } else {
        return &KnxMsg_Buffers[buf_num];
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) GetBufferNumber(const KnxMsg_BufferPtr buffer)
#else
uint8_t GetBufferNumber(const KnxMsg_BufferPtr buffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_BufferPtr    tmp_buf;
    uint8_t               idx;

    for (idx = (uint8_t)0; idx < MSG_NUM_BUFFERS; idx++) {
        tmp_buf = &KnxMsg_Buffers[idx];

        if (tmp_buf == buffer) {
            return idx;
        }
    }

    return MSG_INVALID_BUFFER;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) ClearMessageBuffer(uint8_t buf_num)
#else
void ClearMessageBuffer(uint8_t buf_num)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMsg_BufferPtr    ptr;
    uint8_t *             pb;

    ptr = GetBufferAddress(buf_num);

    if (ptr == (KnxMsg_BufferPtr)NULL) {
        return;
    }

    pb = (uint8_t *)ptr;
    pb++;

    Utl_MemSet(pb, '\0', (uint16_t)sizeof(KnxMsg_Buffer) - (uint16_t)1);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_StatusType, KSTACK_CODE) KnxMsg_AllocateBuffer(KnxMsg_Buffer ** buffer)
#else
Knx_StatusType KnxMsg_AllocateBuffer(KnxMsg_Buffer ** buffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t fp;
    KnxMsg_BufferPtr ptr;
    KnxMsg_BufferPtr result;

    ASSERT(buffer != NULL);

    //printf("enter KnxMsg_AllocateBuffer [%p]\n", buffer);

    DISABLE_ALL_INTERRUPTS();

    if ((fp = KnxMsg_Queues[TASK_FREE_ID]) == MSG_NO_NEXT) {
        ENABLE_ALL_INTERRUPTS();
        buffer = (KnxMsg_Buffer **)NULL;
        return KNX_E_NOT_OK;       /* no Buffer available. */
    }

    ptr = &KnxMsg_Buffers[fp];

    if (ptr->next == MSG_NO_NEXT) {
        KnxMsg_Queues[TASK_FREE_ID] = MSG_NO_NEXT;
    } else {
        KnxMsg_Queues[TASK_FREE_ID]    = ptr->next;
        ptr->next                      = MSG_NO_NEXT;
    }

    ENABLE_ALL_INTERRUPTS();
    result = &KnxMsg_Buffers[fp];
    *buffer = result;

    //printf("leave KnxMsg_AllocateBuffer\n");
    return KNX_E_OK;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMsg_Buffer *, KSTACK_CODE) KnxMsg_AllocateBufferWrapper(void)
#else
KnxMsg_Buffer * KnxMsg_AllocateBufferWrapper(void)
#endif
{
    KnxMsg_Buffer * buffer;

    KnxMsg_AllocateBuffer(&buffer);

    return buffer;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_ReleaseBuffer(KnxMsg_BufferPtr ptr)
#else
void KnxMsg_ReleaseBuffer(KnxMsg_BufferPtr ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   buf_num;
    uint8_t   old_fp;
    uint8_t   t_fp;

    ASSERT_IS_NOT_NULL(ptr);

    DISABLE_ALL_INTERRUPTS();

    if ((buf_num = GetBufferNumber(ptr)) == MSG_INVALID_BUFFER) {
        /* TODO: call error-handler */
        ENABLE_ALL_INTERRUPTS();
        return;
    }

    old_fp = KnxMsg_Queues[TASK_FREE_ID];
    t_fp   = old_fp;

    while (t_fp != MSG_NO_NEXT) {
        if (t_fp == buf_num) {
            ENABLE_ALL_INTERRUPTS();
            /* TODO: call error-handler */
            return;   /* not allocated. */
        }

        t_fp = KnxMsg_Buffers[t_fp].next;
    }

    KnxMsg_Queues[TASK_FREE_ID]    = buf_num;
    KnxMsg_Buffers[buf_num].next   = old_fp;
    ClearMessageBuffer(buf_num);

    ptr = (KnxMsg_BufferPtr)NULL;  /* invalidate Buffer. */
    ENABLE_ALL_INTERRUPTS();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxMsg_ClearBuffer(KnxMsg_BufferPtr ptr)
#else
boolean KnxMsg_ClearBuffer(KnxMsg_BufferPtr ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t * pb;

    if (ptr == (KnxMsg_BufferPtr)NULL) {
        return FALSE;
    }

    pb = (uint8_t *)ptr;
    pb++;

    Utl_MemSet(pb, '\0', sizeof(KnxMsg_Buffer) - 1);

    return TRUE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxMsg_Post(KnxMsg_BufferPtr ptr)
#else
boolean KnxMsg_Post(KnxMsg_BufferPtr ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   queue;
    uint8_t   buf_num;
    uint8_t   qp;

    if ((buf_num = GetBufferNumber(ptr)) == MSG_INVALID_BUFFER) {
        return FALSE;
    }

    if ((queue = KnxMsg_GetQueueForService(ptr->service)) == TASK_FREE_ID) {
        return FALSE;
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

    return TRUE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMsg_BufferPtr, KSTACK_CODE) KnxMsg_Get(uint8_t task)
#else
KnxMsg_BufferPtr KnxMsg_Get(uint8_t task)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t qp;

    if ((task < 1) || (task > MSG_NUM_TASKS)) {
        return (KnxMsg_BufferPtr)NULL;
    }

    if ((qp = KnxMsg_Queues[task]) == MSG_QUEUE_EMPTY) {
        return (KnxMsg_BufferPtr)NULL;   /* no message for task. */
    }

    if (KnxMsg_Buffers[qp].next != MSG_QUEUE_EMPTY) {
        KnxMsg_Queues[task]        = KnxMsg_Buffers[qp].next;
        KnxMsg_Buffers[qp].next    = MSG_NO_NEXT; /* unlink Message-Buffer. */
    } else {
        KnxMsg_Queues[task] = MSG_QUEUE_EMPTY;
    }

/*     */
    return &KnxMsg_Buffers[qp];
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_Init(void)
#else
void KnxMsg_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t t;

    for (t = (uint8_t)0; t < MSG_NUM_BUFFERS; t++) {
        ClearMessageBuffer(t);
    }

    KnxMsg_Queues[TASK_FREE_ID] = (uint8_t)0x00;      /* the first Queue contains the Freelist. */

    for (t = (uint8_t)0; t <= MSG_NUM_BUFFERS; t++) { /* Setup Freelist. */
        KnxMsg_Buffers[t].next = t + (uint8_t)1;
    }

    KnxMsg_Buffers[MSG_NUM_BUFFERS - 1].next = MSG_NO_NEXT;

    for (t = (uint8_t)1; t < MSG_NUM_TASKS; t++) {
        KnxMsg_Queues[t] = MSG_QUEUE_EMPTY;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_SetLen(KnxMsg_BufferPtr pBuffer, uint8_t len)
#else
void KnxMsg_SetLen(KnxMsg_BufferPtr pBuffer, uint8_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{
    pBuffer->len                           = len;
    KnxMsg_GetMessagePtr(pBuffer)->npci   |= ((len - (uint8_t)7) & (uint8_t)0x0f);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxMsg_GetLen(const KnxMsg_BufferPtr pBuffer)
#else
uint8_t KnxMsg_GetLen(const KnxMsg_BufferPtr pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
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
FUNC(void, KSTACK_CODE) KnxMsg_SetRoutingCount(KnxMsg_BufferPtr pBuffer)
#else
void KnxMsg_SetRoutingCount(KnxMsg_BufferPtr pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   ctrl;
    uint8_t   hop_count;

    ctrl = KnxMsg_GetMessagePtr(pBuffer)->ctrl;

    if ((ctrl & (uint8_t)0x02) == (uint8_t)0x02) {
        hop_count                              = MSG_NO_ROUTING_CTRL;
        ctrl                                  &= ~(uint8_t)0x02;
        KnxMsg_GetMessagePtr(pBuffer)->ctrl    = ctrl;
    } else {
        hop_count = HOP_COUNT;
    }

    KnxMsg_GetMessagePtr(pBuffer)->npci |= ((hop_count & (uint8_t)0x07) << 4);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxMsg_GetRoutingCount(const KnxMsg_BufferPtr pBuffer)
#else
uint8_t KnxMsg_GetRoutingCount(const KnxMsg_BufferPtr pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return ((KnxMsg_GetMessagePtr(pBuffer)->npci) & (uint8_t)0x70) >> 4;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMsg_SetRoutingCtrl(KnxMsg_BufferPtr pBuffer, boolean on)
#else
void KnxMsg_SetRoutingCtrl(KnxMsg_BufferPtr pBuffer, boolean on)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t r;

    (on == TRUE) ? (r = (uint8_t)0x02) : (r = (uint8_t)0x00);
    KnxMsg_GetMessagePtr(pBuffer)->ctrl |= r;
}


/*
   boolean MSG_GetRoutingCtrl(const PMSG_Buffer pBuffer)
   {
    return ((MSG_GetMessagePtr(pBuffer)->ctrl & 0x02)==0x02);
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
