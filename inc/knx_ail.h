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
#if !defined(__KNX_AIL_H)
#define __KNX_AIL_H

#include "knx_alg.h"
#include "knx_layer_application.h"
#include "knx_lsm.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
** Global defines.
*/
#define KNX_SET_TX_STATUS_IDLE_OK       ((uint8_t)0x00)
#define KNX_SET_TX_STATUS_IDLE_ERROR    ((uint8_t)0x01)
#define KNX_SET_TX_STATUS_TRANSMITTING  ((uint8_t)0x02)
#define KNX_SET_TX_STATUS_TRANSMIT_REQ  ((uint8_t)0x03)

#define KNX_SET_FLG_DATA_REQUEST        ((uint8_t)0x44)
#define KNX_RESET_FLG_DATA_REQUEST      ((uint8_t)0x40)

#define KNX_SET_FLG_UPDATED             ((uint8_t)0x88)
#define KNX_RESET_FLG_UPDATED           ((uint8_t)0x80)

/*
** Global function-like macros.
*/
#define KnxAil_GetTransmissionStatus(objectNr) AL_GetTransmissionStatus((objectNr))

/*
**  Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE)  KnxAil_TestAndGetObject(uint16_t objectNr,
                                                    P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) dst
                                                    );
FUNC(boolean, KSTACK_CODE)  KnxAil_TestObject(uint16_t objectNr);
FUNC(boolean, KSTACK_CODE)  KnxAil_GetObject(uint16_t objectNr,
                                             P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) dst
                                             );
FUNC(boolean, KSTACK_CODE)  KnxAil_SetAndTransmitObject(uint16_t objectNr,
                                                        P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) src
                                                        );
FUNC(boolean, KSTACK_CODE)  KnxAil_SetObject(uint16_t objectNr,
                                             P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) src
                                             );
FUNC(boolean, KSTACK_CODE)  KnxAil_TransmitObject(uint16_t objectNr);
FUNC(boolean, KSTACK_CODE)  KnxAil_ReadObject(uint16_t objectNr);
FUNC(uint8_t, KSTACK_CODE)    KnxAil_SetRAMFlags(uint16_t objectNr, uint8_t flags);
FUNC(boolean, KSTACK_CODE)  KnxAil_GetRAMFlags(uint16_t objectNr,
                                               P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) flags
                                               );
#else
boolean KnxAil_TestAndGetObject(uint16_t objectNr, void * dst);
boolean KnxAil_TestObject(uint16_t objectNr);
boolean KnxAil_GetObject(uint16_t objectNr, void * dst);
boolean KnxAil_SetAndTransmitObject(uint16_t objectNr, void * src);
boolean KnxAil_SetObject(uint16_t objectNr, void * src);
boolean KnxAil_TransmitObject(uint16_t objectNr);
boolean KnxAil_ReadObject(uint16_t objectNr);
uint8_t   KnxAil_SetRAMFlags(uint16_t objectNr, uint8_t flags);
boolean KnxAil_GetRAMFlags(uint16_t objectNr, uint8_t * flags);


#endif /* KSTACK_MEMORY_MAPPING */

/*
   BIM112-Object-Handling:
   =======================
   U_dataRequest           This macro initiates the reading of an object value of a certain group via the bus.
   U_getCObjVal            This function copies the value of a communication object to a specified variable.
   U_getDataRequestFlag    This macro returns the setting of the data-request-flag of a communication object.
   U_getRamFlags           This macro returns the setting of all ramflags of a communication object.
   U_getTransmissionStatus This macro returns the setting of the transmissionstatus-flags of a communication object.
   U_getUpdateFlag         This macro returns the setting of the update-flag of a communication object.
   U_existCObject          This macro tests the existence of a communication object.
   U_resetUpdateFlag       This macro resets the update-flag of a communication object.
   U_setCObjVal            This function copies the value of a specified variable to a communication object.
   U_setDataRequestFlag    This macro sets the data-request-flag of a communication object.
   U_resetDataRequestFlag  This macro sets the data-request-flag of a communication object.
   U_setRamFlags           This function sets the ramflags of a communication object.
   U_setUpdateFlag         This macro sets the update-flag of a communication object.
   U_testCObj              This function returns the setting of the update-flag of communication object and resets this flag.
   U_transmitRequest       This macro initiates the transmission of an objectvalue via the bus.
 */

 #if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_AIL_H */
