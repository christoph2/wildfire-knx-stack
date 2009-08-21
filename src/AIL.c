/*
**  Application-Interface-Layer.
*/

#include "AIL.h"

/*  Hinweis: In diesem Modul kommen an Verschiedenen Stellen Interrupt-Sperren zum Einsatz (t.b.d.). */

/*
    todo: ein Test auf 'AppLoaded' ist notwendig!!!
*/

boolean U_TestObject(uint16 objectNr)
{
    uint8 flags;
    
    if (!U_GetRAMFlags(objectNr,&flags)) {
        return FALSE;
    } else {
        (void)U_SetRAMFlags(objectNr,KNX_RESET_FLG_UPDATED);  /* Hinweis: 'SetRAMFlags' beibehalten, aber für interne */
                                                                /* Zwecke optimierte 'SET/RESET'-Funktionen verwenden. */
/* AL_SetRAMFlags(objectNr,flags); */

        return ((flags & KNX_OBJ_UPDATED)==KNX_OBJ_UPDATED);
    }
}

boolean U_TestAndGetObject(uint16 objectNr,void* dst)
{
    if (!U_TestObject(objectNr)) {
        return FALSE;
    } else {
/*      AL_GetObjLen(AL_GetCommObjDescr(objectNr)->Type); */
        U_GetObject(objectNr,dst);  /* todo: 'AL_GetObject()' verwenden!!! */
        return TRUE;
    }
}

/*
**  todo: AL_Get/Set-Object implementieren (ohne Return-Values).
*/
boolean U_GetObject(uint16 objectNr,void* dst)
{
    if (objectNr<AL_GetNumCommObjs() && LSM_IsAppLoaded()) {
        CopyRAM(dst,AL_GetObjectDataPointer(objectNr),AL_GetObjLen(AL_GetCommObjDescr(objectNr)->Type)); /* CopyMem() verwenden!!! */
        return TRUE;
    } else {
        return FALSE;
    }
}

boolean U_TransmitObject(uint16 objectNr)
{
    /* todo: Objekt-Nr. prüfen!? */
    
    if (AL_IsObjectTransmitting(objectNr)) {
        return FALSE;
    } else {    
        AL_SetRAMFlags(objectNr,KNX_OBJ_TRANSMIT_REQ);
        return TRUE;
    }        
}

boolean U_SetAndTransmitObject(uint16 objectNr,void* src)
{
    /* todo: Zunächst Testen, ob das Objekt bereits gesendet wird!? */
    if (!U_SetObject(objectNr,src)) {
        return FALSE;
    }

    if (!U_TransmitObject(objectNr)) {
        return FALSE;
    }

    return TRUE;
}


boolean U_SetObject(uint16 objectNr,void* src)
{
    if (objectNr<AL_GetNumCommObjs() && LSM_IsAppLoaded()) {
        CopyRAM(AL_GetObjectDataPointer(objectNr),src,AL_GetObjLen(AL_GetCommObjDescr(objectNr)->Type)); /* CopyMem() verwenden!!! */
        return TRUE;
    } else {
        return FALSE;
    }
}

boolean U_ReadObject(uint16 objectNr)
{
   if (objectNr<AL_GetNumCommObjs() && LSM_IsAppLoaded()) {
        if (AL_IsObjectTransmitting(objectNr)) {
            return FALSE;
        } else {
            AL_SetRAMFlags(objectNr,KNX_OBJ_DATA_REQUEST);
            return TRUE;
        }        
   } else {
        return FALSE;
   }
}

boolean U_GetRAMFlags(uint16 objectNr,uint8 *flags)
{   
    if (objectNr<AL_GetNumCommObjs() && LSM_IsAppLoaded()) {
        *flags=AL_GetRAMFlags(objectNr);
        return TRUE;
    }
    return FALSE;
}

uint8 U_SetRAMFlags(uint16 objectNr,uint8 flags)
{
    uint8 value,mask,tmp;
    
    if (objectNr<AL_GetNumCommObjs() && LSM_IsAppLoaded()) {
        value=(flags & 0x0f);        
        mask=(flags & 0xf0)>>4;
/*        tmp=APP_RAMFlags[objectNr>>1];    */
        tmp=AL_GetRAMFlagPointer()[objectNr>>1];
        
        if ((objectNr % 2)==0) {
            tmp&=0x0f;
        } else {
            tmp=(tmp & 0xf0)>>4;            
        }
                
        tmp=(mask | tmp) ^ (mask & ~value);

        if ((objectNr % 2)==1) {
/*            APP_RAMFlags[objectNr>>1]&=(0x0f | (tmp<<4)); */
            AL_GetRAMFlagPointer()[objectNr>>1]&=(0x0f | (tmp<<4));
        } else {
/*            APP_RAMFlags[objectNr>>1]&=(0xf0 | tmp);  */
            AL_GetRAMFlagPointer()[objectNr>>1]&=(0xf0 | tmp);
        }
        
    }
    return tmp;    
}
