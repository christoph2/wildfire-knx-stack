/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 * (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>
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

#if 0
#pragma message "Compiling " __FILE__ "..."
#define DO_PRAGMA(x)    _Pragma(# x)
#define TODO(x)         DO_PRAGMA(message("TODO - " # x))

#endif

#include "Appl.h"

uint8 DEV_GetHopCount(void);


uint8   DEV_Device_Control;
uint8   DEV_Current_Accesslevel;
uint8   DEV_SystemState;

const uint8                 KNX_DEV_FIRMWARE_REV   = 0x00;
const Knx_SerialNumberType  KNX_DEV_SERIAL_NUM     = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
const uint8                 KNX_DEV_ORDER_INFO[10] = {0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};

#if defined(_MSC_VER)
    #pragma data_seg(push, ".__EEPROM")
    #pragma pack(push,1)
#endif

#if defined(__HIWARE__)
    #pragma CONST_SEG        EEPROM
#endif

/*
   // BIM13X-Application-Information-Block.
   typedef struct
   {
   USHORT AIBVersion;            // At the moment 0x0001 has to be specified.
   uint8 ApplFirmwareVersion;      // Property (3/204).
   uint8 ApplFirmwareSubVersion;   // Property (3/205). - both Versions free to App.
   void (*AppMain) (void);
   void (*AppSave) (void);
   void (*AppUnload) (void);
   const CObjPtr* pCObjects;
   uint8* pRAMFlags;
   TIMER_TAB* pUserTimerTab;      // if no User-Timers ==> NULL.
   const INTERFACE_ROOT* pUsrIntObjRoot;   // User-Interface-Objects.
   ParamMgmt* pUsrParamMgmt;
   USHORT WatchDogTime;         // if no Watchdog ==> 0x0000.
   } AppInfoBlock;
 */

/*static*/ const uint8 DEV_EEPROM_HEADER[0x16] = {
    0xff,   /* OptionReg (HC05 EEPROM Option Register).         */
    0x00,   /* ManData (2 Bytes).   */
    0x00,
    0x00,   /* Manufacturer (2 Bytes).   */
    0x00,
    0x00,   /* DevTyp (2 Bytes).   */
    0x00,
    0x00,   /* Version.       */
    0xff,   /* CheckLimit.  */
    0x00,   /* PEI_Type expected.   */
    0x00,   /* SyncRate. (t.b.d).   */
    0x00,   /* PortCDDR.    */
    0x00,   /* PortADDR.    */
    0xff,   /* RunError (A flag is set if the corresponding bit = 0).   */
    0x60,   /* RouteCnt (Bits #6-#4).   */
    0x63,   /* MxRstCnt.    */
    0xE7,   /* ConfigDes.   */
    0x00,   /* AssocTabPtr. */
    0x00,   /* CommsTabPtr. */
    0x00,   /* UserInitPtr. */
    0x00,   /* UserPrgPtr.  */
    0x00    /* UsrSavPtr.   */
};

START_ADDRESS_TABLE(APP_NUM_OF_COM_OBJS)
/* IMPLEMENT_PHYS_ADDR(0x0164) */
IMPLEMENT_PHYS_ADDR(0x1101)         /* todo: Macro 'MAKE_PHYS_ADDR(a,l,d) */

IMPLEMENT_GROUP_ADDR(0x0901)        /* todo: Macro 'MAKE_GROUP_ADDR(main.middle,sub)'. */
IMPLEMENT_GROUP_ADDR(0x5100)
IMPLEMENT_GROUP_ADDR(0x5101)
END_ADDRESS_TABLE()

START_ASSOCIATION_TABLE(5)
IMPLEMENT_ASSOCIATION(0x0100)
IMPLEMENT_ASSOCIATION(0x0201)
IMPLEMENT_ASSOCIATION(0x0302)
IMPLEMENT_ASSOCIATION(0x0403)
IMPLEMENT_ASSOCIATION(0x0504)
END_ASSOCIATION_TABLE()

START_COMMOBJ_TABLE(3, 0xc8)
IMPLEMENT_COMMOBJ(0, (KNX_OBJ_COMM_ENABLE | KNX_OBJ_TRANSMIT_ENABLE | KNX_OBJ_WRITE_ENABLE | KNX_OBJ_PRIO_LOW), otUINT16)
IMPLEMENT_COMMOBJ(2, (KNX_OBJ_COMM_ENABLE | KNX_OBJ_TRANSMIT_ENABLE), otFLOAT)
IMPLEMENT_COMMOBJ(6, (KNX_OBJ_COMM_ENABLE | KNX_OBJ_TRANSMIT_ENABLE), otUINT1)
END_COMMOBJ_TABLE()

const Knx_PollGroupSettingType KNX_DEV_POLLGROUP_SETTINGS = {0xffff, 0xff};

/*

   KNX_INTERFACE_OBJ* Knx_SystemInterfaceObjs[4]=
   {
    &Knx_IO_Device,
    &Knx_IO_AddrTable,
    &Knx_IO_AssocTable,
    &Knx_IO_AppPrg
   };

   const uint8 KNX_SystemInterfaceObjCount=sizeof(KNX_SystemInterfaceObjs)/(sizeof(KNX_INTERFACE_OBJ*));
 */

/*********************************************************************
 **********************************************************************
 **********************************************************************/
/*
   KNX_INTERFACE_OBJ Knx_IO_AddrTable={
    MAKE_OBJ_ACCESS(BCU20_PRIVILEGE_NO,BCU20_PRIVILEGE_SERVICE),
    SIZEOF_ARRAY(Knx_AddrTabProperties),
    KNX_AddrTabProperties
   };
 */

#define START_USER_INTERFACE_OBJECTS()

#define START_IMPLEMENT_USER_INTERFACE_OBJECT(objName, objType, objAccess0, objAccess1) \
    Knx_PropertyType * KNX_PROPS_ ## objName;                                           \
    Knx_InterfaceObjectType KNX_UIO_ ## objName = {                                     \
        MAKE_OBJ_ACCESS((objAccess0), (objAccess1)),                                    \
        0,                                                                              \
        NULL

#define END_IMPLEMENT_USER_INTERFACE_OBJECT() \
    }

#define IMPLEMENT_USER_PROPERY(PropName, propID, propType) \
    {(propID), MAKE_PROP_CTL(PROP_RO, PROP_NO_ARR, PROP_VALUE, (propType)), PROP_NO_FUNC, (Knx_AddressType)KNX_OT_ASSOCIATIONTABLE_OBJECT},

#define END_USER_INTERFACE_OBJECTS()

#define KNX_IMPLEMENT_USER_INTERFACE_OBJECTS
#if 1
const Knx_InterfaceObjectType * * Knx_UserInterfaceObjs = {
    (Knx_InterfaceObjectType const * *)NULL
};
#endif

/**************/

#if 0
START_USER_INTERFACE_OBJECTS()
END_IMPLEMENT_USER_INTERFACE_OBJECT()
END_USER_INTERFACE_OBJECTS()
#endif

/*
   START_OBJECT_TABLE()
    .
    .
    .
   END_OBJECT_TABLE()
 */
/**************/

#if !defined(KNX_IMPLEMENT_USER_INTERFACE_OBJECTS)
    #define Knx_UserInterfaceObjs NULL
#endif

const uint8 Knx_UserInterfaceObjCount = 0; /* sizeof(KNX_UserInterfaceObjs)/(sizeof(KNX_INTERFACE_OBJ*)); */

/* Knx_InterfaceObjectType ** Knx_UserInterfaceObjPtr=(Knx_InterfaceObjectType ** )Knx_UserInterfaceObjs; */

const Knx_InterfaceObjectType * * Knx_UserInterfaceObjPtr = (const Knx_InterfaceObjectType * *)&Knx_UserInterfaceObjs;

/*********************************************************************
 **********************************************************************
 **********************************************************************/

const uint32 KNX_DEV_ACCESS_KEYTABLE[3]; /* check: uint8? */

#if     defined(__HIWARE__)
        #pragma CONST_SEG DEFAULT
#endif

#if     defined(_MSC_VER)
        #pragma pack(pop)
        #pragma data_seg(pop)
#endif

uint8 const * const __LOG_EEPROM_START     = (uint8 *)0x0100;
uint8 const * const __PHYS_EEPROM_START    = (const uint8 *)&DEV_EEPROM_HEADER;

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) DEV_Init(void)
#else
void DEV_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
/*
    KNX_USER_OBJ_DESCR descr;

    IOS_GetUserObjTable(&descr);

   //    KNX_UserObjDescr 0 setzen, falls 0xffff,0xff.
    if ((descr.obj_ptr==(Knx_AddressType)~0) && (descr.obj_count==(uint8)0xff)) {
        descr.obj_ptr=(Knx_AddressType)0;
        descr.obj_count=(uint8)0;
        IOS_SetUserObjTable(descr);
    }
    // APP_Init();
    // UserInit();
 */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8 , KSTACK_CODE) DEV_GetHopCount(void)
#else
uint8 DEV_GetHopCount(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8 hc = DEV_EEPROM_HEADER[0x0e] >> 4;

    return ((hc < 1) || (hc > KNX_DEFAULT_HOP_COUNT)) ? KNX_DEFAULT_HOP_COUNT : hc;
}


