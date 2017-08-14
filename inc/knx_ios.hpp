/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__KNX_IOS_HPP)
#define __KNX_IOS_HPP

#include "knx_address.hpp"
#include "knx_messaging.hpp"

namespace knx {

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
enum class Knx_ObjectTypeType : uint16_t {
    KNX_OT_DEVICE_OBJECT                = 0,
    KNX_OT_ADDRESSTABLE_OBJECT          = 1,
    KNX_OT_ASSOCIATIONTABLE_OBJECT      = 2,
    KNX_OT_APPLICATIONPROGRAM_OBJECT    = 3,
    KNX_OT_INTERFACEPROGRAM_OBJECT      = 4,
    KNX_OT_POLLING_MASTER               = 10
};

#define KNX_OT_USER_OBJECT0                 ((uint16_t)50001)

/*
** Interface Object Type independent Standardized Property Identifiers.
*/
enum class Knx_PropertyIdentifierType : uint8_t {
    KNX_PID_OBJECT_TYPE             = 1,
    KNX_PID_OBJECT_NAME             = 2,
    KNX_PID_SEMAPHOR                = 3,
    KNX_PID_GROUP_OBJECT_REFERENCE  = 4,
    KNX_PID_LOAD_STATE_CONTROL      = 5,
    KNX_PID_RUN_STATE_CONTROL       = 6,
    KNX_PID_TABLE_REFERENCE         = 7,
    KNX_PID_SERVICE_CONTROL         = 8,
    KNX_PID_FIRMWARE_REVISION       = 9,
    KNX_PID_SERVICES_SUPPORTED      = 10,
    KNX_PID_SERIAL_NUMBER           = 11,
    KNX_PID_MANUFACTURER_ID         = 12,
    KNX_PID_PROGRAM_VERSION         = 13,
    KNX_PID_DEVICE_CONTROL          = 14,
    KNX_PID_ORDER_INFO              = 15,
    KNX_PID_PEI_TYPE                = 16,
    KNX_PID_PORT_CONFIGURATION      = 17,
    KNX_PID_POLL_GROUP_SETTINGS     = 18,
    KNX_PID_MANUFACTURER_DATA       = 19,
    KNX_PID_ENABLE                  = 20,
    KNX_PID_DESCRIPTION             = 21,
    KNX_PID_FILE                    = 22,
    KNX_PID_GROUP_ADDRESS_LIST      = 23,

/*
** Interface Object Type specific Standardized Property Identifiers.
*/
    KNX_PID_POLLING_STATE           = 51,
    KNX_PID_POLLING_SLAVE_ADDR      = 52,
    KNX_PID_POLL_CYCLE              = 53,

/* Extension: */
    KNX_PID_SYSTEM_OBJECT_EXTENSION = (0xfe)
};

/*
**  Property Data Types Identifiers.
*/

enum class Knx_PropertyDataTypeType : uint8_t {
    KNX_PDT_CONTROL             = 0x00,
    KNX_PDT_CHAR                = 0x01,
    KNX_PDT_UNSIGNED_CHAR       = 0x02,
    KNX_PDT_INT                 = 0x03,
    KNX_PDT_UNSIGNED_INT        = 0x04,
    KNX_PDT_KNX_FLOAT           = 0x05,
    KNX_PDT_DATE                = 0x06,
    KNX_PDT_TIME                = 0x07,
    KNX_PDT_LONG                = 0x08,
    KNX_PDT_UNSIGNED_LONG       = 0x09,
    KNX_PDT_FLOAT               = 0x0A,
    KNX_PDT_DOUBLE              = 0x0B,
    KNX_PDT_CHAR_BLOCK          = 0x0C,
    KNX_PDT_POLL_GROUP_SETTINGS = 0x0D,
    KNX_PDT_SHORT_CHAR_BLOCK    = 0x0E,
    /* 0x0f */
    /* 0x10 */
    KNX_PDT_GENERIC_01          = 0x11,
    KNX_PDT_GENERIC_02          = 0x12,
    KNX_PDT_GENERIC_03          = 0x13,
    KNX_PDT_GENERIC_04          = 0x14,
    KNX_PDT_GENERIC_05          = 0x15,
    KNX_PDT_GENERIC_06          = 0x16,
    KNX_PDT_GENERIC_07          = 0x17,
    KNX_PDT_GENERIC_08          = 0x18,
    KNX_PDT_GENERIC_09          = 0x19,
    KNX_PDT_GENERIC_10          = 0x1A,

    /* following from 'BIM_M13x.h': */
    KNX_PDT_GENERIC_11          = 0x1B,
    KNX_PDT_GENERIC_12          = 0x1C,
    KNX_PDT_GENERIC_13          = 0x1D,
    KNX_PDT_GENERIC_14          = 0x1E,
    KNX_PDT_GENERIC_15          = 0x1F,
    KNX_PDT_GENERIC_16          = 0x20,
    KNX_PDT_GENERIC_17          = 0x21,
    KNX_PDT_GENERIC_18          = 0x22,
    KNX_PDT_GENERIC_19          = 0x23,
    KNX_PDT_GENERIC_20          = 0x24,
    KNX_PDT_VERSION             = 0x30,
    KNX_PDT_ALARM_INFO          = 0x31,
    KNX_PDT_BINARY_INFORMATION  = 0x32,
    KNX_PDT_BITSET8             = 0x33,
    KNX_PDT_BITSET16            = 0x34,
    KNX_PDT_ENUM8               = 0x35,
    KNX_PDT_SCALING             = 0x36,
    KNX_PDT_FUNCTION            = 0x3E
    /* PDT_SERVICE_CONTROL      = 0x54) // UNDOCUMENTED, BUT OBSERVED (BCU2.*/
    /* PDT_DEVICE_CONTROL       = 0x5b */
};


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
enum class Knx_IOS_ServiceType {
    IOS_PROP_READ,
    IOS_PROP_WRITE,
    IOS_PROP_DESC_READ
};

struct Knx_PropertyType {
    uint8_t property_id;
    uint8_t property_ctrl;
    uint8_t property_func;
    void const * const  property_var;
};

struct KNX_InterfaceObjectType {
    uint8_t access_level; /* access per object. */
    uint8_t property_count;
    const Knx_PropertyType * properties;
};

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
FUNC(void, KSTACK_CODE)                 IOS_Dispatch(const KnxMsg_Buffer * pBuffer,
                                                     uint8_t service, bool connected
                                                     );
#else
Knx_InterfaceObjectType const * IOS_GetInterfaceObjectByIndex(uint16_t object_index);
Knx_PropertyType const *        IOS_FindProperty(Knx_InterfaceObjectType const * pobj, uint16_t prop_id);
Knx_PropertyType const *        IOS_GetPropertyByIndex(Knx_InterfaceObjectType const * pobj, uint16_t prop_index);
void                            IOS_Dispatch(const KnxMsg_Buffer * pBuffer, uint8_t service, bool connected);


#endif /* KSTACK_MEMORY_MAPPING */

/*
** void IOS_SetUserObjTable(KNX_USER_OBJ_DESCR descr);
** void IOS_GetUserObjTable(KNX_USER_OBJ_DESCR *pdescr);
*/

} // namespace knx

#endif  /* __KNX_IOS_HPP */

