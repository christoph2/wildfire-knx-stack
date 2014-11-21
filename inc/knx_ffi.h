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
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/

#if !defined(__KNX_FFI_H)
#define __KNX_FFI_H

#include "knx_defs.h"
#include "knx_platform.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


#if defined(SWIG)

void Ffi_ApiError(uint8_t ModuleId, uint8_t ApiId, uint8_t ErrorCode);
void Ffi_SetApiErrorCB(void * callback);

void Ffi_SetConnect_IndCB(void * callback);
void Ffi_SetConnect_ConCB(void * callback);
void Ffi_Connect_Ind(void);
void Ffi_Connect_Con(boolean status);

void Ffi_SetDisconnect_IndCB(void * callback);
void Ffi_SetDisconnect_ConCB(void * callback);
void Ffi_Disconnect_Ind(void);
void Ffi_Disconnect_Con(boolean status);

void Ffi_SetLocalConfirmationCB(void * callback);
void Ffi_LocalConfirmation(boolean confirmed);

void Ffi_SetIndividual_Address_ResCB(void * callback);
void Ffi_Individual_Address_Res(uint16_t address);

void Ffi_SetProperty_Description_Read_IndCB(void * callback);
void Ffi_Property_Description_Read_Ind(uint16_t source, uint8_t object_index,
    uint8_t property_id, uint8_t property_index, uint8_t type, uint16_t max_nr_of_elem, uint8_t access
);

#define KNX_API_ERROR(ModuleId, ApiId, ErrorCode)               (PORT_LOCK_TASK_LEVEL(), Ffi_ApiError((ModuleId), (ApiId), (ErrorCode)), PORT_UNLOCK_TASK_LEVEL())

#define KNX_CALLBACK_L_CON(confirmed)                           (PORT_LOCK_TASK_LEVEL(), Ffi_LocalConfirmation((confirmed)), PORT_UNLOCK_TASK_LEVEL())

#define KNX_CALLBACK_T_CONNECT_IND()                            (PORT_LOCK_TASK_LEVEL(), Ffi_Connect_Ind(), PORT_UNLOCK_TASK_LEVEL())
#define KNX_CALLBACK_T_CONNECT_CON(status)                      (PORT_LOCK_TASK_LEVEL(), Ffi_Connect_Con((status)), PORT_UNLOCK_TASK_LEVEL())
#define KNX_CALLBACK_T_DISCONNECT_IND()                         (PORT_LOCK_TASK_LEVEL(), Ffi_Disconnect_Ind(), PORT_UNLOCK_TASK_LEVEL())
#define KNX_CALLBACK_T_DISCONNECT_CON(status)                   (PORT_LOCK_TASK_LEVEL(), Ffi_Disconnect_Con((status)), PORT_UNLOCK_TASK_LEVEL())

#define KNX_CALLBACK_INDIVIDUAL_ADDRESS_RES(address)            (PORT_LOCK_TASK_LEVEL(), Ffi_Individual_Address_Res((address)), PORT_UNLOCK_TASK_LEVEL())
#define KNX_CALLBACK_PROPERTYDESCRIPTION_READ_IND(source,   \
    object_index, property_id, property_index, type,        \
    max_nr_of_elem, access)                                     (PORT_LOCK_TASK_LEVEL(), Ffi_Property_Description_Read_Ind((source), \
                                                                    (object_index), (property_id),                                  \
                                                                    (property_index), (type), (max_nr_of_elem), (access)), PORT_UNLOCK_TASK_LEVEL())


#else

#define KNX_API_ERROR(ModuleId, ApiId, ErrorCode)

#define KNX_CALLBACK_L_CON(confirmed)

#define KNX_CALLBACK_T_CONNECT_IND()
#define KNX_CALLBACK_T_CONNECT_CON(status)
#define KNX_CALLBACK_T_DISCONNECT_IND()
#define KNX_CALLBACK_T_DISCONNECT_CON(status)

#define KNX_CALLBACK_INDIVIDUAL_ADDRESS_RES(address)
#define KNX_CALLBACK_PROPERTYDESCRIPTION_READ_IND(source, object_index, property_id, property_index, type, max_nr_of_elem, access)

#endif

    //#define KNX_CALLBACK_
    //#define KNX_CALLBACK_
    //#define KNX_CALLBACK_


#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif /* __KNX_FFI_H */
