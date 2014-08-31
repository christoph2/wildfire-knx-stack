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
FUNC(boolean, KSTACK_CODE) KnxAIL_TestObject(uint16_t objectNr)
#else
boolean KnxAIL_TestObject(uint16_t objectNr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t flags;

    if (!KnxAIL_GetRAMFlags(objectNr, &flags)) {
        return FALSE;
    } else {
        (void)KnxAIL_SetRAMFlags(objectNr, KNX_RESET_FLG_UPDATED);
        return (flags & KNX_OBJ_UPDATED) == KNX_OBJ_UPDATED;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_TestAndGetObject(uint16_t objectNr, P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) dst)
#else
boolean KnxAIL_TestAndGetObject(uint16_t objectNr, void * dst)
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
FUNC(boolean, KSTACK_CODE) KnxAIL_GetObject(uint16_t objectNr, P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) dst)
#else
boolean KnxAIL_GetObject(uint16_t objectNr, void * dst)
#endif /* KSTACK_MEMORY_MAPPING */
{
    boolean appLoaded = LSM_IsAppLoaded();

    if ((objectNr < KnxALG_GetNumCommObjs()) && appLoaded) {
        Utl_MemCopy(dst, KnxALG_GetObjectDataPointer(objectNr),
                    KnxALG_GetObjLen(KnxALG_GetCommObjDescr(objectNr)->Type));
        return TRUE;
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_TransmitObject(uint16_t objectNr)
#else
boolean KnxAIL_TransmitObject(uint16_t objectNr)
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
FUNC(boolean, KSTACK_CODE) KnxAIL_SetAndTransmitObject(uint16_t objectNr, P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) src)
#else
boolean KnxAIL_SetAndTransmitObject(uint16_t objectNr, void * src)
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
FUNC(boolean, KSTACK_CODE) KnxAIL_SetObject(uint16_t objectNr, P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) src)
#else
boolean KnxAIL_SetObject(uint16_t objectNr, void * src)
#endif /* KSTACK_MEMORY_MAPPING */
{
    boolean appLoaded = LSM_IsAppLoaded();

    if ((objectNr < KnxALG_GetNumCommObjs()) && appLoaded) {
        Utl_MemCopy(KnxALG_GetObjectDataPointer(objectNr), src, KnxALG_GetObjLen(KnxALG_GetCommObjDescr(objectNr)->Type));
        return TRUE;
    } else {
        return FALSE;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) KnxAIL_ReadObject(uint16_t objectNr)
#else
boolean KnxAIL_ReadObject(uint16_t objectNr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    boolean appLoaded = LSM_IsAppLoaded();

    if (objectNr < KnxALG_GetNumCommObjs() && appLoaded) {
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
FUNC(boolean, KSTACK_CODE) KnxAIL_GetRAMFlags(uint16_t objectNr, P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) flags)
#else
boolean KnxAIL_GetRAMFlags(uint16_t objectNr, uint8_t * flags)
#endif /* KSTACK_MEMORY_MAPPING */
{
    boolean appLoaded = LSM_IsAppLoaded();

    if (objectNr < KnxALG_GetNumCommObjs() && appLoaded) {
        *flags = KnxALG_GetRAMFlags(objectNr);
        return TRUE;
    }

    return FALSE;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t , KSTACK_CODE) KnxAIL_SetRAMFlags(uint16_t objectNr, uint8_t flags)
#else
uint8_t KnxAIL_SetRAMFlags(uint16_t objectNr, uint8_t flags)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t   value;
    uint8_t   mask;
    uint8_t   tmp = (uint8_t)0x00;
    boolean appLoaded = LSM_IsAppLoaded();

    if ((objectNr < KnxALG_GetNumCommObjs()) && appLoaded) {
        value  = (flags & (uint8_t)0x0f);
        mask   = (flags & (uint8_t)0xf0) >> (uint8_t)4;
/*        tmp=APP_RAMFlags[objectNr>>1];    */
        tmp = KnxALG_GetRAMFlagPointer()[objectNr >> 1];

        if ((objectNr % (uint16_t)2) == (uint16_t)0) {
            tmp &= (uint8_t)0x0f;
        } else {
            tmp = (tmp & (uint8_t)0xf0) >> 4;
        }

        tmp = (mask | tmp) ^ (mask & ~value);

        if ((objectNr % (uint16_t)2) == (uint16_t)1) {
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
