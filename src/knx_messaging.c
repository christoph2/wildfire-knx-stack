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

#include "knx_messaging.h"

#define HOP_COUNT ((uint8_t)6)

/* check: 'static' ??? */
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMSG_BufferPtr, KSTACK_CODE) GetBufferAddress(uint8_t buf_num);
FUNC(uint8_t, KSTACK_CODE)                  GetBufferNumber(const KnxMSG_BufferPtr buffer);
FUNC(void, KSTACK_CODE)                  ClearMessageBuffer(uint8_t buf_num);
#else
KnxMSG_BufferPtr    GetBufferAddress(uint8_t buf_num);
uint8_t             GetBufferNumber(const KnxMSG_BufferPtr buffer);
void                ClearMessageBuffer(uint8_t buf_num);


#endif /* KSTACK_MEMORY_MAPPING */

STATIC const uint8_t KnxMSG_MessageRedirectionTable[16] = {
    TASK_FREE_ID, TASK_LL_ID,   TASK_NL_ID,   TASK_TL_ID,   TASK_TC_ID,     TASK_FREE_ID,     TASK_FREE_ID,   TASK_AL_ID,
    TASK_MG_ID,   TASK_MG_ID,   TASK_PM_ID,   TASK_LC_ID,   TASK_FREE_ID,   TASK_US_ID,       TASK_US_ID,     TASK_US_ID
};

STATIC uint8_t            KnxMSG_Queues[MSG_NUM_TASKS];
STATIC KnxMSG_Buffer    KnxMSG_Buffers[MSG_NUM_BUFFERS];

/*  Get Destination Queue from Message-Code. */
#define KnxMSG_GetQueueForService(service) ((uint8_t)KnxMSG_MessageRedirectionTable[(service) >> 4])

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMSG_BufferPtr, KSTACK_CODE) GetBufferAddress(uint8_t buf_num)
#else
KnxMSG_BufferPtr GetBufferAddress(uint8_t buf_num)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (buf_num >= MSG_NUM_BUFFERS) {
        return (KnxMSG_BufferPtr)NULL;
    } else {
        return &KnxMSG_Buffers[buf_num];
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) GetBufferNumber(const KnxMSG_BufferPtr buffer)
#else
uint8_t GetBufferNumber(const KnxMSG_BufferPtr buffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_BufferPtr    tmp_buf;
    uint8_t               idx;

    for (idx = (uint8_t)0; idx < MSG_NUM_BUFFERS; idx++) {
        tmp_buf = &KnxMSG_Buffers[idx];

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
    KnxMSG_BufferPtr    ptr;
    uint8_t *             pb;

    ptr = GetBufferAddress(buf_num);

    if (ptr == (KnxMSG_BufferPtr)NULL) {
        return;
    }

    pb = (uint8_t *)ptr;
    pb++;

    Utl_MemSet(pb, '\0', (uint16_t)sizeof(KnxMSG_Buffer) - (uint16_t)1);
}


STATIC uint16_t AllocCount = (uint16_t)0, ReleaseCount = (uint16_t)0;

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMSG_BufferPtr, KSTACK_CODE) KnxMSG_AllocateBuffer(void)
#else
KnxMSG_BufferPtr KnxMSG_AllocateBuffer(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t               fp;
    KnxMSG_BufferPtr    ptr;

    DISABLE_ALL_INTERRUPTS();

    AllocCount++;

    if ((fp = KnxMSG_Queues[TASK_FREE_ID]) == MSG_NO_NEXT) {
        ENABLE_ALL_INTERRUPTS();
        return (KnxMSG_BufferPtr)NULL;       /* no Buffer available. */
    }

    ptr = &KnxMSG_Buffers[fp];

    if (ptr->next == MSG_NO_NEXT) {
        KnxMSG_Queues[TASK_FREE_ID] = MSG_NO_NEXT;
    } else {
        KnxMSG_Queues[TASK_FREE_ID]    = ptr->next;
        ptr->next                      = MSG_NO_NEXT;
    }

    ENABLE_ALL_INTERRUPTS();
    return &KnxMSG_Buffers[fp];
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMSG_ReleaseBuffer(KnxMSG_BufferPtr ptr)
#else
void KnxMSG_ReleaseBuffer(KnxMSG_BufferPtr ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   buf_num;
    uint8_t   old_fp;
    uint8_t   t_fp;

    DISABLE_ALL_INTERRUPTS();

    if ((buf_num = GetBufferNumber(ptr)) == MSG_INVALID_BUFFER) {
        /* TODO: call error-handler */
        ENABLE_ALL_INTERRUPTS();
        return;
    }

    ReleaseCount++;

    old_fp = KnxMSG_Queues[TASK_FREE_ID];
    t_fp   = old_fp;

    while (t_fp != MSG_NO_NEXT) {
        if (t_fp == buf_num) {
            ENABLE_ALL_INTERRUPTS();
            /* TODO: call error-handler */
            return;   /* not allocated. */
        }

        t_fp = KnxMSG_Buffers[t_fp].next;
    }

    KnxMSG_Queues[TASK_FREE_ID]    = buf_num;
    KnxMSG_Buffers[buf_num].next   = old_fp;
    ClearMessageBuffer(buf_num);

    ptr = (KnxMSG_BufferPtr)NULL;  /* invalidate Buffer. */
    ENABLE_ALL_INTERRUPTS();
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxMSG_ClearBuffer(KnxMSG_BufferPtr ptr)
#else
boolean KnxMSG_ClearBuffer(KnxMSG_BufferPtr ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t * pb;

    if (ptr == (KnxMSG_BufferPtr)NULL) {
        return FALSE;
    }

    pb = (uint8_t *)ptr;
    pb++;

    Utl_MemSet(pb, '\0', sizeof(KnxMSG_Buffer) - 1);

    return TRUE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxMSG_Post(KnxMSG_BufferPtr ptr)
#else
boolean KnxMSG_Post(KnxMSG_BufferPtr ptr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   queue;
    uint8_t   buf_num;
    uint8_t   qp;

    if ((buf_num = GetBufferNumber(ptr)) == MSG_INVALID_BUFFER) {
        return FALSE;
    }

    if ((queue = KnxMSG_GetQueueForService(ptr->service)) == TASK_FREE_ID) {
        return FALSE;
    }

    qp = KnxMSG_Queues[queue];

    if (qp == MSG_QUEUE_EMPTY) {
        KnxMSG_Queues[queue] = buf_num;
    } else {

        while (KnxMSG_Buffers[qp].next != MSG_QUEUE_EMPTY) {
            qp = KnxMSG_Buffers[qp].next;
        }

        KnxMSG_Buffers[qp].next = buf_num;
    }

    return TRUE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(KnxMSG_BufferPtr, KSTACK_CODE) KnxMSG_Get(uint8_t task)
#else
KnxMSG_BufferPtr KnxMSG_Get(uint8_t task)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t qp;

    if ((task < 1) || (task > MSG_NUM_TASKS)) {
        return (KnxMSG_BufferPtr)NULL;
    }

    if ((qp = KnxMSG_Queues[task]) == MSG_QUEUE_EMPTY) {
        return (KnxMSG_BufferPtr)NULL;   /* no message for task. */
    }

    if (KnxMSG_Buffers[qp].next != MSG_QUEUE_EMPTY) {
        KnxMSG_Queues[task]        = KnxMSG_Buffers[qp].next;
        KnxMSG_Buffers[qp].next    = MSG_NO_NEXT; /* unlink Message-Buffer. */
    } else {
        KnxMSG_Queues[task] = MSG_QUEUE_EMPTY;
    }

/*     */
    return &KnxMSG_Buffers[qp];
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMSG_Init(void)
#else
void KnxMSG_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t t;

    for (t = (uint8_t)0; t < MSG_NUM_BUFFERS; t++) {
        ClearMessageBuffer(t);
    }

    KnxMSG_Queues[TASK_FREE_ID] = (uint8_t)0x00;      /* the first Queue contains the Freelist. */

    for (t = (uint8_t)0; t <= MSG_NUM_BUFFERS; t++) { /* Setup Freelist. */
        KnxMSG_Buffers[t].next = t + (uint8_t)1;
    }

    KnxMSG_Buffers[MSG_NUM_BUFFERS - 1].next = MSG_NO_NEXT;

    for (t = (uint8_t)1; t < MSG_NUM_TASKS; t++) {
        KnxMSG_Queues[t] = MSG_QUEUE_EMPTY;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMSG_SetLen(KnxMSG_BufferPtr pBuffer, uint8_t len)
#else
void KnxMSG_SetLen(KnxMSG_BufferPtr pBuffer, uint8_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{
    pBuffer->len                           = len;
    KnxMSG_GetMessagePtr(pBuffer)->npci   |= ((len - (uint8_t)7) & (uint8_t)0x0f);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxMSG_GetLen(const KnxMSG_BufferPtr pBuffer)
#else
uint8_t KnxMSG_GetLen(const KnxMSG_BufferPtr pBuffer)
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
FUNC(void, KSTACK_CODE) KnxMSG_SetRoutingCount(KnxMSG_BufferPtr pBuffer)
#else
void KnxMSG_SetRoutingCount(KnxMSG_BufferPtr pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   ctrl;
    uint8_t   hop_count;

    ctrl = KnxMSG_GetMessagePtr(pBuffer)->ctrl;

    if ((ctrl & (uint8_t)0x02) == (uint8_t)0x02) {
        hop_count                              = MSG_NO_ROUTING_CTRL;
        ctrl                                  &= ~(uint8_t)0x02;
        KnxMSG_GetMessagePtr(pBuffer)->ctrl    = ctrl;
    } else {
        hop_count = HOP_COUNT;
    }

    KnxMSG_GetMessagePtr(pBuffer)->npci |= ((hop_count & (uint8_t)0x07) << 4);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxMSG_GetRoutingCount(const KnxMSG_BufferPtr pBuffer)
#else
uint8_t KnxMSG_GetRoutingCount(const KnxMSG_BufferPtr pBuffer)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return ((KnxMSG_GetMessagePtr(pBuffer)->npci) & (uint8_t)0x70) >> 4;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxMSG_SetRoutingCtrl(KnxMSG_BufferPtr pBuffer, boolean on)
#else
void KnxMSG_SetRoutingCtrl(KnxMSG_BufferPtr pBuffer, boolean on)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t r;

    (on == TRUE) ? (r = (uint8_t)0x02) : (r = (uint8_t)0x00);
    KnxMSG_GetMessagePtr(pBuffer)->ctrl |= r;
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
