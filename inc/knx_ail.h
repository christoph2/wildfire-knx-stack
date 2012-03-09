/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
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
#define KNX_SET_TX_STATUS_IDLE_OK       ((uint8)0x00)
#define KNX_SET_TX_STATUS_IDLE_ERROR    ((uint8)0x01)
#define KNX_SET_TX_STATUS_TRANSMITTING  ((uint8)0x02)
#define KNX_SET_TX_STATUS_TRANSMIT_REQ  ((uint8)0x03)

#define KNX_SET_FLG_DATA_REQUEST        ((uint8)0x44)
#define KNX_RESET_FLG_DATA_REQUEST      ((uint8)0x40)

#define KNX_SET_FLG_UPDATED             ((uint8)0x88)
#define KNX_RESET_FLG_UPDATED           ((uint8)0x80)


/*
** Global function-like macros.
*/
#define KnxAIL_GetTransmissionStatus(objectNr) AL_GetTransmissionStatus((objectNr))

	
/*
**  Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE)  KnxAIL_TestAndGetObject(uint16 objectNr, 
    P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) dst
);
FUNC(boolean, KSTACK_CODE)  KnxAIL_TestObject(uint16 objectNr);
FUNC(boolean, KSTACK_CODE)  KnxAIL_GetObject(uint16 objectNr, 
    P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) dst
);
FUNC(boolean, KSTACK_CODE)  KnxAIL_SetAndTransmitObject(uint16 objectNr, 
    P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) src
);
FUNC(boolean, KSTACK_CODE)  KnxAIL_SetObject(uint16 objectNr, 
    P2VAR(void, AUTOMATIC, KSTACK_APPL_DATA) src
);
FUNC(boolean, KSTACK_CODE)  KnxAIL_TransmitObject(uint16 objectNr);
FUNC(boolean, KSTACK_CODE)  KnxAIL_ReadObject(uint16 objectNr);
FUNC(uint8, KSTACK_CODE)    KnxAIL_SetRAMFlags(uint16 objectNr, uint8 flags);
FUNC(boolean, KSTACK_CODE)  KnxAIL_GetRAMFlags(uint16 objectNr, 
    P2VAR(uint8, AUTOMATIC, KSTACK_APPL_DATA) flags
);
#else	
boolean KnxAIL_TestAndGetObject(uint16 objectNr, void * dst);
boolean KnxAIL_TestObject(uint16 objectNr);
boolean KnxAIL_GetObject(uint16 objectNr, void * dst);
boolean KnxAIL_SetAndTransmitObject(uint16 objectNr, void * src);
boolean KnxAIL_SetObject(uint16 objectNr, void * src);
boolean KnxAIL_TransmitObject(uint16 objectNr);
boolean KnxAIL_ReadObject(uint16 objectNr);
uint8   KnxAIL_SetRAMFlags(uint16 objectNr, uint8 flags);
boolean KnxAIL_GetRAMFlags(uint16 objectNr, uint8 * flags);
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
