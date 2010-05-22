/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
#include "sys_objs.h"


/*
** Supplement S08 "Distributed Address Assignment", ein 'A_NetworkParameter_Write.req' wird
** als AddressRegistration verwendet.
** (Interface Object Type=0 / Property Identifier = PID_ADDR_CHECK (3Dh))
*/

/*
4.1 Property for Device Object : SubnetAddress
    PID_SUBNET_ADDRESS(57d)
        This property includes the Subnetwork Address of the Device (size: 1 octet).

4.2 Property for Device Object : AddressReport
    PID_ADDRESS_REPORT(60d)
        This property includes the serial number of the Device (size: 6 octets).
        This value shall in datagrams be set to 000000000000h if the device has no valid serial number.
4.3 Property for Device Object : Address_Check
    PID_ADDRESS_CHECK (61d)
        This property is void (size: 1 octet; value = 00h always).
        Having no effect, it is used for checking the presence of a device at a given Individual Address.
*/

/*
#if     defined(_MSC_VER)
        #pragma data_seg(push,".CONST")
#endif

//DEFAULT_ROM EEPROM
#if     defined(__HIWARE__)
        #pragma CONST_SEG        DEFAULT_ROM
#endif
*/

typedef SizeType ADDR_T;

const Knx_PropertyType Knx_DeviceProperties[]=
{
{KNX_PID_OBJECT_TYPE,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_VALUE,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(ADDR_T)KNX_OT_DEVICE_OBJECT},
{KNX_PID_DEVICE_CONTROL,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_GENERIC_01),PROP_NO_FUNC,(ADDR_T)NULL},
{KNX_PID_SERVICE_CONTROL,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(ADDR_T)NULL},
{KNX_PID_FIRMWARE_REVISION,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_PTR,KNX_PDT_UNSIGNED_CHAR),PROP_NO_FUNC,(const void*)&KNX_DEV_FIRMWARE_REV},
{KNX_PID_SERIAL_NUMBER,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_GENERIC_06),PROP_NO_FUNC,(const void*)KNX_DEV_SERIAL_NUM},
{KNX_PID_MANUFACTURER_ID,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(ADDR_T)NULL},
{KNX_PID_ORDER_INFO,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_GENERIC_10),PROP_NO_FUNC,(const void*)KNX_DEV_ORDER_INFO},
{KNX_PID_PEI_TYPE,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_VALUE,KNX_PDT_UNSIGNED_CHAR),PROP_NO_FUNC,(ADDR_T)0x00},   /* measured PEI-Type.*/
{KNX_PID_POLL_GROUP_SETTINGS,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_POLL_GROUP_SETTINGS),PROP_NO_FUNC,(const void*)&KNX_DEV_POLLGROUP_SETTINGS},
{KNX_PID_PORT_CONFIGURATION,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_UNSIGNED_CHAR),PROP_NO_FUNC,(ADDR_T)NULL},
{KNX_PID_MANUFACTURER_DATA,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_GENERIC_04),PROP_NO_FUNC,(ADDR_T)NULL},
};

const Knx_PropertyType Knx_AddrTabProperties[]=
{
{KNX_PID_OBJECT_TYPE,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_VALUE,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(const void*)KNX_OT_ADDRESSTABLE_OBJECT},
{KNX_PID_LOAD_STATE_CONTROL,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_CONTROL),PROP_FUNC,(const void*)&KNX_SystemLSC[KNX_LSC_GRAT]},
{KNX_PID_TABLE_REFERENCE,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_PTR,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(const void*)APP_AddressTable/*__PHYS_EEPROM_START*/},
};

const Knx_PropertyType Knx_AssocTabProperties[]=
{
{KNX_PID_OBJECT_TYPE,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_VALUE,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(const void*)KNX_OT_ASSOCIATIONTABLE_OBJECT},
{KNX_PID_LOAD_STATE_CONTROL,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_CONTROL),PROP_FUNC,(const void*)&KNX_SystemLSC[KNX_LSC_GROAT]},
{KNX_PID_TABLE_REFERENCE,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_PTR,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(const void*)APP_AssociationTable},
};

const Knx_PropertyType Knx_AppPrgProperties[]=
{
{KNX_PID_OBJECT_TYPE,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_VALUE,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(const void*)KNX_OT_APPLICATIONPROGRAM_OBJECT},
{KNX_PID_LOAD_STATE_CONTROL,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_CONTROL),PROP_FUNC,(const void*)&KNX_SystemLSC[KNX_LSC_APP]},
{KNX_PID_RUN_STATE_CONTROL,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_CONTROL),PROP_FUNC,(ADDR_T)NULL},     /* todo: Adresse der RSM-Variablen!!! */
{KNX_PID_PEI_TYPE,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_UNSIGNED_CHAR),PROP_NO_FUNC,(ADDR_T)NULL},   /* required PEI-Type. */
{KNX_PID_PROGRAM_VERSION,MAKE_PROP_CTL(PROP_RW,PROP_NO_ARR,PROP_PTR,KNX_PDT_GENERIC_05),PROP_NO_FUNC,(ADDR_T)NULL},
{KNX_PID_TABLE_REFERENCE,MAKE_PROP_CTL(PROP_RO,PROP_NO_ARR,PROP_PTR,KNX_PDT_UNSIGNED_INT),PROP_NO_FUNC,(ADDR_T)NULL},   /* check: CommObjs??? */
};

/* Hinweis: wo ist eigentlich die ObjectID ??? */
const Knx_InterfaceObjectType Knx_IO_Device={
    MAKE_OBJ_ACCESS(BCU20_PRIVILEGE_NO,BCU20_PRIVILEGE_CONFIGURATION),
    SIZEOF_ARRAY(Knx_DeviceProperties),
    Knx_DeviceProperties
};

const Knx_InterfaceObjectType Knx_IO_AddrTable={
    MAKE_OBJ_ACCESS(BCU20_PRIVILEGE_NO,BCU20_PRIVILEGE_SERVICE),
    SIZEOF_ARRAY(Knx_AddrTabProperties),
    Knx_AddrTabProperties
};

const Knx_InterfaceObjectType Knx_IO_AssocTable={
    MAKE_OBJ_ACCESS(BCU20_PRIVILEGE_NO,BCU20_PRIVILEGE_SERVICE),
    SIZEOF_ARRAY(Knx_AssocTabProperties),
    Knx_AssocTabProperties
};

const Knx_InterfaceObjectType Knx_IO_AppPrg={
    MAKE_OBJ_ACCESS(BCU20_PRIVILEGE_NO,BCU20_PRIVILEGE_CONFIGURATION),
    SIZEOF_ARRAY(Knx_AppPrgProperties),
    Knx_AppPrgProperties
};


Knx_InterfaceObjectType const * const Knx_SystemInterfaceObjs[]=
{
    &Knx_IO_Device,
    &Knx_IO_AddrTable,
    &Knx_IO_AssocTable,
    &Knx_IO_AppPrg
};

const uint8 Knx_SystemInterfaceObjCount=SIZEOF_ARRAY(Knx_SystemInterfaceObjs);

/*
#if     defined(__HIWARE__)
        #pragma DATA_SEG DEFAULT
#endif

#if     defined(_MSC_VER)
        #pragma data_seg(pop)
#endif

*/

