/*
**
**	Hardware-Configuration-Settings (HC(S)12).
**
**
*/

#ifndef __CONFIG_H
#define __CONFIG_H

#define __TARGET    _HC12DG128A                 /* Target-Controller. */

#define __ECLK      8000 * 1000                 /* Bus-Frequency. */

#define __INTERFACE SCI0                        /* TPUART-Interface. */
#define __BAUDRATE  19200                       /* TPUART-Baudrate. */
#define __SCI_PS    (__ECLK / 16) / __BAUDRATE  /* Prescaler for SCI. */

#undef  __DEBUG                                 /* Deactivate Debugging-Code. */

#endif /* __CONFIG_H */
