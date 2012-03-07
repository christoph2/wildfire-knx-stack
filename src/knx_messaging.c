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

#define HOP_COUNT ((uint8)6)

/* check: 'static' ??? */
KnxMSG_BufferPtr    GetBufferAddress(uint8 buf_num);
uint8               GetBufferNumber(const KnxMSG_BufferPtr buffer);
void                ClearMessageBuffer(uint8 buf_num);


static const uint8 KnxMSG_MessageRedirectionTable[16] = {
    TASK_FREE_ID, TASK_LL_ID,   TASK_NL_ID,   TASK_TL_ID,   TASK_TC_ID,     TASK_FREE_ID,   TASK_FREE_ID,   TASK_AL_ID,
    TASK_MG_ID,   TASK_MG_ID,   TASK_PM_ID,   TASK_LC_ID,   TASK_FREE_ID,   TASK_US_ID,     TASK_US_ID,     TASK_US_ID
};

static uint8            KnxMSG_Queues[MSG_NUM_TASKS];
static KnxMSG_Buffer    KnxMSG_Buffers[MSG_NUM_BUFFERS];

/*  Get Destination Queue from Message-Code. */
#define KnxMSG_GetQueueForService(service) ((uint8)KnxMSG_MessageRedirectionTable[(service) >> 4])

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

KnxMSG_BufferPtr GetBufferAddress(uint8 buf_num)
{
    if (buf_num >= MSG_NUM_BUFFERS) {
        return (KnxMSG_BufferPtr)NULL;
    } else {
        return &KnxMSG_Buffers[buf_num];
    }
}


uint8 GetBufferNumber(const KnxMSG_BufferPtr buffer)
{
    KnxMSG_BufferPtr    tmp_buf;
    uint8               idx;

    for (idx = (uint8)0; idx < MSG_NUM_BUFFERS; idx++) {
        tmp_buf = &KnxMSG_Buffers[idx];

        if (tmp_buf == buffer) {
            return idx;
        }
    }

    return MSG_INVALID_BUFFER;
}


void ClearMessageBuffer(uint8 buf_num)
{
    KnxMSG_BufferPtr    ptr;
    uint8 *             pb;

    ptr = GetBufferAddress(buf_num);

    if (ptr == (KnxMSG_BufferPtr)NULL) {
        return;
    }

    pb = (uint8 *)ptr;
    pb++;

    Utl_MemSet(pb, '\0', (uint16)sizeof(KnxMSG_Buffer) - (uint16)1);
}


static uint16 AllocCount = (uint16)0, ReleaseCount = (uint16)0;

KnxMSG_BufferPtr KnxMSG_AllocateBuffer(void)
{
    uint8               fp;
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


boolean KnxMSG_ReleaseBuffer(KnxMSG_BufferPtr ptr)
{
    uint8   buf_num;
    uint8   old_fp;
    uint8   t_fp;

    DISABLE_ALL_INTERRUPTS();

    if ((buf_num = GetBufferNumber(ptr)) == MSG_INVALID_BUFFER) {
        ENABLE_ALL_INTERRUPTS();
        return FALSE;
    }

    ReleaseCount++;

    old_fp = KnxMSG_Queues[TASK_FREE_ID];
    t_fp   = old_fp;

    while (t_fp != MSG_NO_NEXT) {
        if (t_fp == buf_num) {
            ENABLE_ALL_INTERRUPTS();
            return FALSE;   /* not allocated. */
        }

        t_fp = KnxMSG_Buffers[t_fp].next;
    }

    KnxMSG_Queues[TASK_FREE_ID]    = buf_num;
    KnxMSG_Buffers[buf_num].next   = old_fp;
    ClearMessageBuffer(buf_num);

    ptr = (KnxMSG_BufferPtr)NULL;  /* invalidate Buffer. */
    ENABLE_ALL_INTERRUPTS();
    return TRUE;
}


boolean KnxMSG_ClearBuffer(KnxMSG_BufferPtr ptr)
{
    uint8 * pb;

    if (ptr == (KnxMSG_BufferPtr)NULL) {
        return FALSE;
    }

    pb = (uint8 *)ptr;
    pb++;

    Utl_MemSet(pb, '\0', sizeof(KnxMSG_Buffer) - 1);

    return TRUE;
}


boolean KnxMSG_Post(KnxMSG_BufferPtr ptr)
{
    uint8   queue;
    uint8   buf_num;
    uint8   qp;

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


KnxMSG_BufferPtr KnxMSG_Get(uint8 task)
{
    uint8 qp;

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


void KnxMSG_Init(void)
{
    uint8 t;

    for (t = (uint8)0; t < MSG_NUM_BUFFERS; t++) {
        ClearMessageBuffer(t);
    }

    KnxMSG_Queues[TASK_FREE_ID] = (uint8)0x00;      /* the first Queue contains the Freelist. */

    for (t = (uint8)0; t <= MSG_NUM_BUFFERS; t++) { /* Setup Freelist. */
        KnxMSG_Buffers[t].next = t + (uint8)1;
    }

    KnxMSG_Buffers[MSG_NUM_BUFFERS - 1].next = MSG_NO_NEXT;

    for (t = (uint8)1; t < MSG_NUM_TASKS; t++) {
        KnxMSG_Queues[t] = MSG_QUEUE_EMPTY;
    }
}


void KnxMSG_SetLen(KnxMSG_BufferPtr pBuffer, uint8 len)
{
    pBuffer->len                           = len;
    KnxMSG_GetMessagePtr(pBuffer)->ncpi   |= ((len - (uint8)7) & (uint8)0x0f);
}


uint8 KnxMSG_GetLen(const KnxMSG_BufferPtr pBuffer)
{
    return pBuffer->len;
}


/*
   void MSG_SetHopCount(PMSG_Buffer pBuffer,uint8 hop_count)
   {
    MSG_GetMessagePtr(pBuffer)->ncpi|=((hop_count & 0x07)<<4);
   }

   void MSG_GetHopCount(PMSG_Buffer pBuffer,uint8 *hop_count)
   {
   *hop_count=((MSG_GetMessagePtr(pBuffer)->ncpi) & 0x70)>>4;
   }
 */

void KnxMSG_SetRoutingCount(KnxMSG_BufferPtr pBuffer)
{
    uint8   ctrl;
    uint8   hop_count;

    ctrl = KnxMSG_GetMessagePtr(pBuffer)->ctrl;

    if ((ctrl & (uint8)0x02) == (uint8)0x02) {
        hop_count                              = MSG_NO_ROUTING_CTRL;
        ctrl                                  &= ~(uint8)0x02;
        KnxMSG_GetMessagePtr(pBuffer)->ctrl    = ctrl;
    } else {
        hop_count = HOP_COUNT;
    }

    KnxMSG_GetMessagePtr(pBuffer)->ncpi |= ((hop_count & (uint8)0x07) << 4);
}


uint8 KnxMSG_GetRoutingCount(const KnxMSG_BufferPtr pBuffer)
{
    return ((KnxMSG_GetMessagePtr(pBuffer)->ncpi) & (uint8)0x70) >> 4;
}


void KnxMSG_SetRoutingCtrl(KnxMSG_BufferPtr pBuffer, boolean on)
{
    uint8 r;

    (on == TRUE) ? (r = (uint8)0x02) : (r = (uint8)0x00);
    KnxMSG_GetMessagePtr(pBuffer)->ctrl |= r;
}


/*
   boolean MSG_GetRoutingCtrl(const PMSG_Buffer pBuffer)
   {
    return ((MSG_GetMessagePtr(pBuffer)->ctrl & 0x02)==0x02);
   }
 */

/*
   uint8 MSG_GetLSDULen(PMSG_Buffer pBuffer)
   {
    return MSG_GetMessagePtr(pBuffer)->ncpi & 0x0f;
   }

   void MSG_SetLSDULen(PMSG_Buffer pBuffer,uint8 len_lsdu)
   {
    MSG_GetMessagePtr(pBuffer)->ncpi=(len_lsdu & 0x0f);
   }
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
