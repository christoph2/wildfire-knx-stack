/*
**
**  Anwendungs-spezifische (auch Implementations??) Daten und Funktionen.
**
**  todo:       'Proj' statt 'Appl'!!!
**                      in 'sys_eeprom' umbenennen!!!
**
*/

#include "Appl.h"

uint8 DEV_GetHopCount(void);

uint8 DEV_Device_Control;
uint8 DEV_Current_Accesslevel;
uint8 DEV_SystemState;

const uint8 KNX_DEV_FIRMWARE_REV=0x00;
const Knx_SerialNumberType KNX_DEV_SERIAL_NUM={0x01,0x02,0x03,0x04,0x05,0x06};
const uint8 KNX_DEV_ORDER_INFO[10]={0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01};

/*
** +++  Projekt-spezifisch.
*/

/*
**  todo: EEPROM-Prolog als Array!!!
**  Hinweis: das ganze EEPROM so gestalten, das es virtualisiert werden kann, d.h. beim Programm-Start geladen
**      und nach dem User-Save gespeichert werden kann, das ermöglicht den (virtuellen) EEPROM-Inhalt in
**      einer Datei zu halten.
*/

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
	USHORT AIBVersion;				// At the moment 0x0001 has to be specified.
	uint8 ApplFirmwareVersion;		// Property (3/204).
	uint8 ApplFirmwareSubVersion;	// Property (3/205). - both Versions free to App.
	void (*AppMain) (void);
	void (*AppSave) (void);
	void (*AppUnload) (void);
	const CObjPtr* pCObjects;
	uint8* pRAMFlags;
	TIMER_TAB* pUserTimerTab;		// if no User-Timers ==> NULL.
	const INTERFACE_ROOT* pUsrIntObjRoot;	// User-Interface-Objects.
	ParamMgmt* pUsrParamMgmt;
	USHORT WatchDogTime;			// if no Watchdog ==> 0x0000.
} AppInfoBlock;
*/

/*
**  Hinweis: AssocTabPtr. kann nur im Bereich 0x19-0xfe liegen.
*/
/*static*/ const uint8 DEV_EEPROM_HEADER[0x16]={  /* // todo: 'IMPLEMENT_EE_HEADER' oder so... */
    0xff,   /* OptionReg (HC05 EEPROM Option Register). */
    0x00,   /* ManData (2 Bytes). */
    0x00,
    0x00,   /* Manufacturer (2 Bytes).  // todo: Makro f. (Manuf,DevType,Version). */
    0x00,
    0x00,   /* DevTyp (2 Bytes). */
    0x00,
    0x00,   /* Version. */
    0xff,   /* CheckLimit. */
    0x00,   /* PEI_Type expected. */
    0x00,   /* SyncRate. (t.b.d). */
    0x00,   /* PortCDDR. */
    0x00,   /* PortADDR. */
    0xff,   /* RunError (A flag is set if the corresponding bit = 0). */
    0x60,   /* RouteCnt (Bits #6-#4). */
    0x63,   /* MxRstCnt. */
    0xE7,   /* ConfigDes.   (Hinweis: wenn Bit #6==0, dann A_EVENT-Generation; Bit #3==0, dann telegram-Rate-Limitation).  */
    0x00,   /* AssocTabPtr. */
    0x00,   /* CommsTabPtr. */
    0x00,   /* UserInitPtr. */
    0x00,   /* UserPrgPtr. */
    0x00    /* UsrSavPtr. */
};

START_ADDRESS_TABLE(APP_NUM_OF_COM_OBJS)
/* IMPLEMENT_PHYS_ADDR(0x0164) */
    IMPLEMENT_PHYS_ADDR(0x1101)    /* todo: Makro 'MAKE_PHYS_ADDR(a,l,d) */

    IMPLEMENT_GROUP_ADDR(0x0901)	/* todo: Makro 'MAKE_GROUP_ADDR(main.middle,sub)'. */
    IMPLEMENT_GROUP_ADDR(0x5100)
    IMPLEMENT_GROUP_ADDR(0x5101)    
END_ADDRESS_TABLE()


START_ASSOCIATION_TABLE(5)      /* todo: 'NumAssocs' oder so... */
    IMPLEMENT_ASSOCIATION(0x0100)
    IMPLEMENT_ASSOCIATION(0x0201)
    IMPLEMENT_ASSOCIATION(0x0302)
    IMPLEMENT_ASSOCIATION(0x0403)
    IMPLEMENT_ASSOCIATION(0x0504)                
END_ASSOCIATION_TABLE()


/* todo: Makros um 'übliche' Flag-Kombinationen zusammenzufassen!!! */
START_COMMOBJ_TABLE(3,0xc8)
    IMPLEMENT_COMMOBJ(0,(KNX_OBJ_COMM_ENABLE|KNX_OBJ_TRANSMIT_ENABLE|KNX_OBJ_WRITE_ENABLE|KNX_OBJ_PRIO_LOW),otUINT16)
    IMPLEMENT_COMMOBJ(2,(KNX_OBJ_COMM_ENABLE|KNX_OBJ_TRANSMIT_ENABLE),otFLOAT)
    IMPLEMENT_COMMOBJ(6,(KNX_OBJ_COMM_ENABLE|KNX_OBJ_TRANSMIT_ENABLE),otUINT1)
END_COMMOBJ_TABLE()

const Knx_PollGroupSettingType KNX_DEV_POLLGROUP_SETTINGS={0xffff,0xff};


/*

KNX_INTERFACE_OBJ* Knx_SystemInterfaceObjs[4]= // todo: Struktur mit Längenbyte verwenden (auch für User-Objects)!
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
    sizeof(Knx_AddrTabProperties)/sizeof(KNX_PROPERTY),
    KNX_AddrTabProperties
};
*/

#define START_USER_INTERFACE_OBJECTS()

#define START_IMPLEMENT_USER_INTERFACE_OBJECT(objName,objType,objAccess0,objAccess1)    \
Knx_PropertyType KNX_PROPS_##objName[];                                          \
Knx_InterfaceObjectType KNX_UIO_##objName={                                       \
    MAKE_OBJ_ACCESS((objAccess0),(objAccess1)),                                 \
    0,                                                                      \
    NULL

/*
** todo: Propertiy-Definition beginnen mit 'ObjectType'.
** KNX_PROPERTY KNX_AssocTabProperties[]={
** {KNX_PID_OBJECT_TYPE,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_VALUE,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(ADDR_T)KNX_OT_ASSOCIATIONTABLE_OBJECT},
*/
    

#define END_IMPLEMENT_USER_INTERFACE_OBJECT()    \
    };

/*
** todo: Forward-Deklaration f. die folgenden Properties!!!
** KNX_IO_???_Properties
*/

#define IMPLEMENT_USER_PROPERY(PropName,propID,propType)    \
{(propID),MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_VALUE,(propType)),PROP_NO_FUNC,(ADDR_T)KNX_OT_ASSOCIATIONTABLE_OBJECT},

#define END_USER_INTERFACE_OBJECTS()


#define KNX_IMPLEMENT_USER_INTERFACE_OBJECTS
Knx_InterfaceObjectType* Knx_UserInterfaceObjs[]=
{
NULL
};


/**************/
START_USER_INTERFACE_OBJECTS()
    START_IMPLEMENT_USER_INTERFACE_OBJECT(ParameterHaushalt,KNX_OT_USER_OBJECT0,BCU20_PRIVILEGE_NO,BCU20_PRIVILEGE_SERVICE)
/*  IMPLEMENT_USER_PROPERY(EinschaltVerzoegerung,51,KNX_PDT_GENERIC_04) */
    END_IMPLEMENT_USER_INTERFACE_OBJECT()
END_USER_INTERFACE_OBJECTS()

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

const uint8 Knx_UserInterfaceObjCount=0; /* sizeof(KNX_UserInterfaceObjs)/(sizeof(KNX_INTERFACE_OBJ*)); */

const Knx_InterfaceObjectType** Knx_UserInterfaceObjPtr=Knx_UserInterfaceObjs;

/*********************************************************************
**********************************************************************
**********************************************************************/

const uint32 KNX_DEV_ACCESS_KEYTABLE[3]; /* check: uint8? */

/*
**  Hinweis: die Größe des EEPROM-Bereiches könnte über eine Variable ermittelt werden,
**      die am Ende des Speicher-Bereichs steht, der User-Bereich würde nach dem
**      EEPROm-Header included werden.
*/

#if     defined(__HIWARE__)
        #pragma CONST_SEG DEFAULT
#endif

#if     defined(_MSC_VER)
        #pragma pack(pop)
        #pragma data_seg(pop)
#endif

const /*ADDR_T*/ uint16 __LOG_EEPROM_START=0x100;
const /*ADDR_T*/ uint16 __PHYS_EEPROM_START=(uint16)&DEV_EEPROM_HEADER;

void DEV_Init(void)
{
/*
    KNX_USER_OBJ_DESCR descr;
    
    IOS_GetUserObjTable(&descr);

//    KNX_UserObjDescr 0 setzen, falls 0xffff,0xff.
    if ((descr.obj_ptr==(ADDR_T)~0) && (descr.obj_count==(uint8)0xff)) {
        descr.obj_ptr=(ADDR_T)0;
        descr.obj_count=(uint8)0;
        IOS_SetUserObjTable(descr);        
    }
    // APP_Init();
    // UserInit();
*/    
}

uint8 DEV_GetHopCount(void)
{
    uint8 hc=DEV_EEPROM_HEADER[0x0e]>>4;

    return ((hc<1) || (hc>KNX_DEFAULT_HOP_COUNT)) ? KNX_DEFAULT_HOP_COUNT : hc;
}

