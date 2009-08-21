#if !defined(__APPL_H)
#define __APPL_H

#include "Address.h"
#include "Network.h"
#include "ios.h"

#define APP_NUM_OF_COM_OBJS     ((uint8)3)
/* todo: NUM_ASSOCS */

#define APP_OBJECT_DATA_SIZE    ((uint8)8)   /* Der belegte Speicher aller Kommunikations-Objekte. */

/* todo: EEPROM-Konstanten von den normalen Abgrenzen!!! */

extern const uint8 KNX_DEV_FIRMWARE_REV;

extern const uint8 DEV_EEPROM_HEADER[0x16];

extern const Knx_SerialNumberType KNX_DEV_SERIAL_NUM;
extern const uint8 KNX_DEV_ORDER_INFO[10];  /* todo: Name !!! */

extern const Knx_PollGroupSettingType KNX_DEV_POLLGROUP_SETTINGS;
extern const uint32 KNX_DEV_ACCESS_KEYTABLE[3]; /* todo: Name !!! */

extern uint8 const * const __LOG_EEPROM_START;
extern uint8 const * const __PHYS_EEPROM_START;

extern const uint8 Knx_UserInterfaceObjCount;  /* check: in Hinblick auf die Zukunft besser 'uint16'??? */
extern const Knx_InterfaceObjectType** Knx_UserInterfaceObjPtr;

void DEV_Init(void);

#endif  /* __APPL_H */
