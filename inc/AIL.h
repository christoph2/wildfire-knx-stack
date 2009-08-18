#if !defined(__AIL_H)
#define __AIL_H

#include "KNXDefs.h"
#include "Application.h"
#include "lsm.h"

#define KNX_SET_TX_STATUS_IDLE_OK       ((uint8)0x00)
#define KNX_SET_TX_STATUS_IDLE_ERROR    ((uint8)0x01)
#define KNX_SET_TX_STATUS_TRANSMITTING  ((uint8)0x02)
#define KNX_SET_TX_STATUS_TRANSMIT_REQ  ((uint8)0x03)

#define KNX_SET_FLG_DATA_REQUEST        ((uint8)0x44)
#define KNX_RESET_FLG_DATA_REQUEST      ((uint8)0x40)

#define KNX_SET_FLG_UPDATED             ((uint8)0x88)
#define KNX_RESET_FLG_UPDATED           ((uint8)0x80)


boolean U_TestAndGetObject(uint16 objectNr,void* dst);
boolean U_TestObject(uint16 objectNr);
boolean U_GetObject(uint16 objectNr,void* dst);

boolean U_SetAndTransmitObject(uint16 objectNr,void* src);
boolean U_SetObject(uint16 objectNr,void* src);
boolean U_TransmitObject(uint16 objectNr);

boolean U_ReadObject(uint16 objectNr);

uint8 U_SetRAMFlags(uint16 objectNr,uint8 flags);
boolean U_GetRAMFlags(uint16 objectNr,uint8 *flags);

#define U_GetTransmissionStatus(objectNr)  AL_GetTransmissionStatus((objectNr))

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


#endif /* __AIL_H */
