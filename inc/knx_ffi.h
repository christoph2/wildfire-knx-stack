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
void Ffi_SetIndividual_Address_ResCB(void * callback);
void Ffi_Individual_Address_Res(uint16_t address);

void Ffi_SetProperty_Description_Read_IndCB(void * callback);
void Ffi_Property_Description_Read_Ind(uint16_t source, uint8_t object_index,
    uint8_t property_id, uint8_t property_index, uint8_t type, uint16_t max_nr_of_elem, uint8_t access
);

#define KNX_CALLBACK_INDIVIDUAL_ADDRESS_RES(address)            Ffi_Individual_Address_Res((address))
#define KNX_CALLBACK_PROPERTYDESCRIPTION_READ_IND(source,   \
    object_index, property_id, property_index, type,        \
    max_nr_of_elem, access)                                       Ffi_Property_Description_Read_Ind((source), (object_index), (property_id), \
                                                                    (property_index), (type), (max_nr_of_elem), (access))


#else

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
