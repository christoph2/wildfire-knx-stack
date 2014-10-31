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
#if !defined(__KNX_IOS_H)
#define __KNX_IOS_H

#include "knx_address.h"
#include "knx_messaging.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
**
** Global defines.
**
*/
#define PROP_RO                             ((uint8_t)0)
#define PROP_RW                             ((uint8_t)1)
#define PROP_NO_ARR                         ((uint8_t)0)
#define PROP_ARR                            ((uint8_t)1)
#define PROP_VALUE                          ((uint8_t)0)
#define PROP_PTR                            ((uint8_t)1)
#define PROP_NO_FUNC                        ((uint8_t)0)
#define PROP_FUNC                           ((uint8_t)1)

/*
**  System Interface Objects.
*/
typedef enum tagKnx_ObjectTypeType {
    KNX_OT_DEVICE_OBJECT                = ((uint16_t)0),
    KNX_OT_ADDRESSTABLE_OBJECT          = ((uint16_t)1),
    KNX_OT_ASSOCIATIONTABLE_OBJECT      = ((uint16_t)2),
    KNX_OT_APPLICATIONPROGRAM_OBJECT    = ((uint16_t)3),
    KNX_OT_INTERFACEPROGRAM_OBJECT      = ((uint16_t)4),
    KNX_OT_POLLING_MASTER               = ((uint16_t)10)
} Knx_ObjectTypeType;

#define KNX_OT_USER_OBJECT0                 ((uint16_t)50001)

/*
** Interface Object Type independent Standardized Property Identifiers.
*/
typedef enum tagKnx_PropertyIdentifierType {
    KNX_PID_OBJECT_TYPE             = ((uint8_t)1),
    KNX_PID_OBJECT_NAME             = ((uint8_t)2),
    KNX_PID_SEMAPHOR                = ((uint8_t)3),
    KNX_PID_GROUP_OBJECT_REFERENCE  = ((uint8_t)4),
    KNX_PID_LOAD_STATE_CONTROL      = ((uint8_t)5),
    KNX_PID_RUN_STATE_CONTROL       = ((uint8_t)6),
    KNX_PID_TABLE_REFERENCE         = ((uint8_t)7),
    KNX_PID_SERVICE_CONTROL         = ((uint8_t)8),
    KNX_PID_FIRMWARE_REVISION       = ((uint8_t)9),
    KNX_PID_SERVICES_SUPPORTED      = ((uint8_t)10),
    KNX_PID_SERIAL_NUMBER           = ((uint8_t)11),
    KNX_PID_MANUFACTURER_ID         = ((uint8_t)12),
    KNX_PID_PROGRAM_VERSION         = ((uint8_t)13),
    KNX_PID_DEVICE_CONTROL          = ((uint8_t)14),
    KNX_PID_ORDER_INFO              = ((uint8_t)15),
    KNX_PID_PEI_TYPE                = ((uint8_t)16),
    KNX_PID_PORT_CONFIGURATION      = ((uint8_t)17),
    KNX_PID_POLL_GROUP_SETTINGS     = ((uint8_t)18),
    KNX_PID_MANUFACTURER_DATA       = ((uint8_t)19),
    KNX_PID_ENABLE                  = ((uint8_t)20),
    KNX_PID_DESCRIPTION             = ((uint8_t)21),
    KNX_PID_FILE                    = ((uint8_t)22),
    KNX_PID_GROUP_ADDRESS_LIST      = ((uint8_t)23),

/*
** Interface Object Type specific Standardized Property Identifiers.
*/
    KNX_PID_POLLING_STATE           = ((uint8_t)51),
    KNX_PID_POLLING_SLAVE_ADDR      = ((uint8_t)52),
    KNX_PID_POLL_CYCLE              = ((uint8_t)53),

/* Extension: */
    KNX_PID_SYSTEM_OBJECT_EXTENSION = ((uint8_t)(0xfe))
} Knx_PropertyIdentifierType;

/*
**  Property Data Types Identifiers.
*/

typedef enum tagKnx_PropertyDataTypeType {
    KNX_PDT_CONTROL             = ((uint8_t)0x00),
    KNX_PDT_CHAR                = ((uint8_t)0x01),
    KNX_PDT_UNSIGNED_CHAR       = ((uint8_t)0x02),
    KNX_PDT_INT                 = ((uint8_t)0x03),
    KNX_PDT_UNSIGNED_INT        = ((uint8_t)0x04),
    KNX_PDT_KNX_FLOAT           = ((uint8_t)0x05),
    KNX_PDT_DATE                = ((uint8_t)0x06),
    KNX_PDT_TIME                = ((uint8_t)0x07),
    KNX_PDT_LONG                = ((uint8_t)0x08),
    KNX_PDT_UNSIGNED_LONG       = ((uint8_t)0x09),
    KNX_PDT_FLOAT               = ((uint8_t)0x0A),
    KNX_PDT_DOUBLE              = ((uint8_t)0x0B),
    KNX_PDT_CHAR_BLOCK          = ((uint8_t)0x0C),
    KNX_PDT_POLL_GROUP_SETTINGS = ((uint8_t)0x0D),
    KNX_PDT_SHORT_CHAR_BLOCK    = ((uint8_t)0x0E),
    /* 0x0f */
    /* 0x10 */
    KNX_PDT_GENERIC_01          = ((uint8_t)0x11),
    KNX_PDT_GENERIC_02          = ((uint8_t)0x12),
    KNX_PDT_GENERIC_03          = ((uint8_t)0x13),
    KNX_PDT_GENERIC_04          = ((uint8_t)0x14),
    KNX_PDT_GENERIC_05          = ((uint8_t)0x15),
    KNX_PDT_GENERIC_06          = ((uint8_t)0x16),
    KNX_PDT_GENERIC_07          = ((uint8_t)0x17),
    KNX_PDT_GENERIC_08          = ((uint8_t)0x18),
    KNX_PDT_GENERIC_09          = ((uint8_t)0x19),
    KNX_PDT_GENERIC_10          = ((uint8_t)0x1A),

    /* following from 'BIM_M13x.h': */
    KNX_PDT_GENERIC_11          = ((uint8_t)0x1B),
    KNX_PDT_GENERIC_12          = ((uint8_t)0x1C),
    KNX_PDT_GENERIC_13          = ((uint8_t)0x1D),
    KNX_PDT_GENERIC_14          = ((uint8_t)0x1E),
    KNX_PDT_GENERIC_15          = ((uint8_t)0x1F),
    KNX_PDT_GENERIC_16          = ((uint8_t)0x20),
    KNX_PDT_GENERIC_17          = ((uint8_t)0x21),
    KNX_PDT_GENERIC_18          = ((uint8_t)0x22),
    KNX_PDT_GENERIC_19          = ((uint8_t)0x23),
    KNX_PDT_GENERIC_20          = ((uint8_t)0x24),
    KNX_PDT_VERSION             = ((uint8_t)0x30),
    KNX_PDT_ALARM_INFO          = ((uint8_t)0x31),
    KNX_PDT_BINARY_INFORMATION  = ((uint8_t)0x32),
    KNX_PDT_BITSET8             = ((uint8_t)0x33),
    KNX_PDT_BITSET16            = ((uint8_t)0x34),
    KNX_PDT_ENUM8               = ((uint8_t)0x35),
    KNX_PDT_SCALING             = ((uint8_t)0x36),
    KNX_PDT_FUNCTION            = ((uint8_t)0x3E)
    /* PDT_SERVICE_CONTROL      = ((uint8_t)0x54) // UNDOCUMENTED, BUT OBSERVED (BCU2).*/
    /* PDT_DEVICE_CONTROL       = ((uint8_t)0x5b) */
} Knx_PropertyDataTypeType;


   #define PDT_WR_EN            0x80
   #define FUNCTION_FLAG        0x40
// */

/*
** Global function-like macros.
*/
#define MAKE_PROP_CTL(write, array, ptr, \
                      type)             (((write) & 0x01) << 7) | (((array) & 0x01) << 6) | (((ptr) & 0x01) << 5) | ((type) & 0x1f)
#define MAKE_OBJ_ACCESS(read, write)    (((read) & 0x0f) << 4) | ((write) & 0x0f)

/*
** Global types.
*/
typedef enum tagKnx_IOS_ServiceType {
    IOS_PROP_READ,
    IOS_PROP_WRITE,
    IOS_PROP_DESC_READ
} Knx_IOS_ServiceType;

typedef struct tagKnx_PropertyType {
    uint8_t               property_id;
    uint8_t               property_ctrl;
    uint8_t               property_func;
    void const * const  property_var;
} Knx_PropertyType;

typedef struct tagKNX_InterfaceObjectType {
    uint8_t                       access_level; /* access per object. */
    uint8_t                       property_count;
    const Knx_PropertyType *    properties;
} Knx_InterfaceObjectType;

/* folgendes aus 'BIM_M13x.h': */
/*
   typedef struct PROPERTY                         //! structure for a property
   {
    uint8_t PropertyID;                            //!< possible: 1...255
    uint8_t PropertyType;                          //!< defines type and writeability
    USHORT MaxLength;                           //!< number of max. elements
    USHORT *pActualLength;                      //!< pointer to actual numbers of elem or NULL pointer if fix length
    const void *pData;                          //!< void pointer to data(s) or function
    enum PropertyInternalType InternalType;     //!< indicates if property is normal memory, eeval or otp
   } PROPERTY;

   typedef struct INTERFACE_OBJECT                 //! structure for a interface object
   {
    USHORT ObjType;                             //!< object type (2 byte: higbyte,lowbyte)
    USHORT PropCount;                           //!< max 256 properties possible
    PROPERTY *PropTable;                        //!< pointer to propertytable (first property in table!)
   } INTERFACE_OBJECT;

   typedef struct INTERFACE_ROOT                   //! root of the interface objects
   {
    USHORT ObjCount;                            //!< max 65536 objects possible
    const INTERFACE_OBJECT *ObjTable;           //!< pointer to objectable
   } INTERFACE_ROOT;
 */

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_InterfaceObjectType const *, KSTACK_CODE)  IOS_GetInterfaceObjectByIndex(uint16_t object_index);
FUNC(Knx_PropertyType const *, KSTACK_CODE)     IOS_FindProperty(
    P2CONST(Knx_InterfaceObjectType, AUTOMATIC, KSTACK_APPL_DATA) pobj,
    uint16_t prop_id
    );
FUNC(Knx_PropertyType const *, KSTACK_CODE)         IOS_GetPropertyByIndex(
    P2CONST(Knx_InterfaceObjectType, AUTOMATIC, KSTACK_APPL_DATA) pobj,
    uint16_t prop_index
    );
FUNC(void, KSTACK_CODE)                 IOS_Dispatch(const KnxMsg_BufferPtr pBuffer,
                                                     uint8_t service, boolean connected
                                                     );
#else
Knx_InterfaceObjectType const * IOS_GetInterfaceObjectByIndex(uint16_t object_index);
Knx_PropertyType const *        IOS_FindProperty(Knx_InterfaceObjectType const * pobj, uint16_t prop_id);
Knx_PropertyType const *        IOS_GetPropertyByIndex(Knx_InterfaceObjectType const * pobj, uint16_t prop_index);
void                            IOS_Dispatch(const KnxMsg_BufferPtr pBuffer, uint8_t service, boolean connected);


#endif /* KSTACK_MEMORY_MAPPING */

/*
** void IOS_SetUserObjTable(KNX_USER_OBJ_DESCR descr);
** void IOS_GetUserObjTable(KNX_USER_OBJ_DESCR *pdescr);
*/

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_IOS_H */

