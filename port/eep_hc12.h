#ifndef __EEP_HC12
#define	__EEP_HC12

#include "target.h"

void eep_init(void);
void eep_progbyte(ADDR_T addr,BYTE data);
void eep_erasebyte(ADDR_T addr,BYTE data);
void eep_protect(bool on);

#endif	// __EEP_HC12
