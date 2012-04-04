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

/*
**
**  Application-Interface-Layer.
**
*/

#include "knx_ail.h"

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

/*
**	Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_TestObject(uint16 objectNr)
#else
boolean KnxAIL_TestObject(uint16 objectNr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8 flags;

    if (!KnxAIL_GetRAMFlags(objectNr, &flags)) {
        return FALSE;
    } else {
        (void)KnxAIL_SetRAMFlags(objectNr, KNX_RESET_FLG_UPDATED);
        return (flags & KNX_OBJ_UPDATED) == KNX_OBJ_UPDATED;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_TestAndGetObject(uint16 objectNr, P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) dst)
#else
boolean KnxAIL_TestAndGetObject(uint16 objectNr, void * dst)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (!KnxAIL_TestObject(objectNr)) {
        return FALSE;
    } else {
        (void)KnxAIL_GetObject(objectNr, dst);
        return TRUE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_GetObject(uint16 objectNr, P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) dst)
#else
boolean KnxAIL_GetObject(uint16 objectNr, void * dst)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if ((objectNr < KnxALG_GetNumCommObjs()) && (LSM_IsAppLoaded())) {
        Utl_MemCopy(dst, KnxALG_GetObjectDataPointer(objectNr),
                    KnxALG_GetObjLen(KnxALG_GetCommObjDescr(objectNr)->Type));
        return TRUE;
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_TransmitObject(uint16 objectNr)
#else
boolean KnxAIL_TransmitObject(uint16 objectNr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    /* todo: check 'objectNr'? */

    if (KnxALG_IsObjectTransmitting(objectNr)) {
        return FALSE;
    } else {
        KnxALG_SetRAMFlags(objectNr, KNX_OBJ_TRANSMIT_REQ);
        return TRUE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_SetAndTransmitObject(uint16 objectNr, P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) src)
#else
boolean KnxAIL_SetAndTransmitObject(uint16 objectNr, void * src)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (!KnxAIL_SetObject(objectNr, src)) {
        return FALSE;
    }

    if (!KnxAIL_TransmitObject(objectNr)) {
        return FALSE;
    }

    return TRUE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_SetObject(uint16 objectNr, P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) src)
#else
boolean KnxAIL_SetObject(uint16 objectNr, void * src)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if ((objectNr < KnxALG_GetNumCommObjs()) && (LSM_IsAppLoaded())) {
        Utl_MemCopy(KnxALG_GetObjectDataPointer(objectNr), src, KnxALG_GetObjLen(KnxALG_GetCommObjDescr(objectNr)->Type));
        return TRUE;
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_ReadObject(uint16 objectNr)
#else
boolean KnxAIL_ReadObject(uint16 objectNr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (objectNr < KnxALG_GetNumCommObjs() && LSM_IsAppLoaded()) {
        if (KnxALG_IsObjectTransmitting(objectNr)) {
            return FALSE;
        } else {
            KnxALG_SetRAMFlags(objectNr, KNX_OBJ_DATA_REQUEST);
            return TRUE;
        }
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_GetRAMFlags(uint16 objectNr, P2VAR(uint8, AUTOMATIC, KSTACK_APPL_DATA) flags)
#else
boolean KnxAIL_GetRAMFlags(uint16 objectNr, uint8 * flags)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (objectNr < KnxALG_GetNumCommObjs() && LSM_IsAppLoaded()) {
        *flags = KnxALG_GetRAMFlags(objectNr);
        return TRUE;
    }

    return FALSE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8 , KSTACK_CODE) KnxAIL_SetRAMFlags(uint16 objectNr, uint8 flags)
#else
uint8 KnxAIL_SetRAMFlags(uint16 objectNr, uint8 flags)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8   value;
    uint8   mask;
    uint8   tmp = (uint8)0x00;

    if ((objectNr < KnxALG_GetNumCommObjs()) && (LSM_IsAppLoaded())) {
        value  = (flags & (uint8)0x0f);
        mask   = (flags & (uint8)0xf0) >> (uint8)4;
/*        tmp=APP_RAMFlags[objectNr>>1];    */
        tmp = KnxALG_GetRAMFlagPointer()[objectNr >> 1];

        if ((objectNr % (uint16)2) == (uint16)0) {
            tmp &= (uint8)0x0f;
        } else {
            tmp = (tmp & (uint8)0xf0) >> 4;
        }

        tmp = (mask | tmp) ^ (mask & ~value);

        if ((objectNr % (uint16)2) == (uint16)1) {
/*            APP_RAMFlags[objectNr>>1]&=(0x0f | (tmp<<4)); */
            KnxALG_GetRAMFlagPointer()[objectNr >> 1] &= (0x0f | (tmp << 4));
        } else {
/*            APP_RAMFlags[objectNr>>1]&=(0xf0 | tmp);  */
            KnxALG_GetRAMFlagPointer()[objectNr >> 1] &= (0xf0 | tmp);
        }

    }

    return tmp;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
