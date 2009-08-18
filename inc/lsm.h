#if !defined(__LSM_H)
#define __LSM_H

#include "KNXDefs.h"

#define KNX_NUM_SYS_LSCS    ((uint8)3)

/* todo: Sinnvollere Namen!!! */
#define KNX_LSC_APP         ((uint8)0)
#define KNX_LSC_GRAT        ((uint8)1)
#define KNX_LSC_GROAT       ((uint8)2)

typedef uint8 KNX_LSCType;

extern KNX_LSCType KNX_SystemLSC[]; /* todo: Besserer Namen, gehört außerdem ins EEPROM!!! */

void LSM_Init(void);
void LSM_Dispatch(uint8 *record,/*LSM_State*/uint8 *ls_var);
boolean LSM_IsAppLoaded(void);
boolean LSM_IsGrATLoaded(void);
boolean LSM_IsGrOATLoaded(void);

void LSM_Test(void);

#endif  /* __LSM_H */
