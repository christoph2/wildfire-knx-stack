/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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

boolean U_TestObject(uint16 objectNr)
{
    uint8 flags;

    if (!U_GetRAMFlags(objectNr, &flags)) {
        return FALSE;
    } else {
        (void)U_SetRAMFlags(objectNr, KNX_RESET_FLG_UPDATED);
        return (flags & KNX_OBJ_UPDATED) == KNX_OBJ_UPDATED;
    }
}

boolean U_TestAndGetObject(uint16 objectNr, void * dst)
{
    if (!U_TestObject(objectNr)) {
        return FALSE;
    } else {
        (void)U_GetObject(objectNr, dst);
        return TRUE;
    }
}

boolean U_GetObject(uint16 objectNr, void * dst)
{
    if ((objectNr < AL_GetNumCommObjs()) && (LSM_IsAppLoaded())) {
        Utl_MemCopy(dst, AL_GetObjectDataPointer(objectNr),
                    AL_GetObjLen(AL_GetCommObjDescr(objectNr)->Type));
        return TRUE;
    } else {
        return FALSE;
    }
}

boolean U_TransmitObject(uint16 objectNr)
{
    /* todo: check 'objectNr'? */

    if (AL_IsObjectTransmitting(objectNr)) {
        return FALSE;
    } else {
        AL_SetRAMFlags(objectNr, KNX_OBJ_TRANSMIT_REQ);
        return TRUE;
    }
}

boolean U_SetAndTransmitObject(uint16 objectNr, void * src)
{
    if (!U_SetObject(objectNr, src)) {
        return FALSE;
    }

    if (!U_TransmitObject(objectNr)) {
        return FALSE;
    }

    return TRUE;
}

boolean U_SetObject(uint16 objectNr, void * src)
{
    if ((objectNr < AL_GetNumCommObjs()) && (LSM_IsAppLoaded())) {
        Utl_MemCopy(AL_GetObjectDataPointer(objectNr), src, AL_GetObjLen(AL_GetCommObjDescr(objectNr)->Type));
        return TRUE;
    } else {
        return FALSE;
    }
}

boolean U_ReadObject(uint16 objectNr)
{
    if (objectNr < AL_GetNumCommObjs() && LSM_IsAppLoaded()) {
        if (AL_IsObjectTransmitting(objectNr)) {
            return FALSE;
        } else {
            AL_SetRAMFlags(objectNr, KNX_OBJ_DATA_REQUEST);
            return TRUE;
        }
    } else {
        return FALSE;
    }
}

boolean U_GetRAMFlags(uint16 objectNr, uint8 * flags)
{
    if (objectNr < AL_GetNumCommObjs() && LSM_IsAppLoaded()) {
        *flags = AL_GetRAMFlags(objectNr);
        return TRUE;
    }

    return FALSE;
}

uint8 U_SetRAMFlags(uint16 objectNr, uint8 flags)
{
    uint8   value;
    uint8   mask;
    uint8   tmp = (uint8)0x00;

    if ((objectNr < AL_GetNumCommObjs()) && (LSM_IsAppLoaded())) {
        value  = (flags & (uint8)0x0f);
        mask   = (flags & (uint8)0xf0) >> (uint8)4;
/*        tmp=APP_RAMFlags[objectNr>>1];    */
        tmp = AL_GetRAMFlagPointer()[objectNr >> 1];

        if ((objectNr % (uint16)2) == (uint16)0) {
            tmp &= (uint8)0x0f;
        } else {
            tmp = (tmp & (uint8)0xf0) >> 4;
        }

        tmp = (mask | tmp) ^ (mask & ~value);

        if ((objectNr % (uint16)2) == (uint16)1) {
/*            APP_RAMFlags[objectNr>>1]&=(0x0f | (tmp<<4)); */
            AL_GetRAMFlagPointer()[objectNr >> 1] &= (0x0f | (tmp << 4));
        } else {
/*            APP_RAMFlags[objectNr>>1]&=(0xf0 | tmp);  */
            AL_GetRAMFlagPointer()[objectNr >> 1] &= (0xf0 | tmp);
        }

    }

    return tmp;
}
