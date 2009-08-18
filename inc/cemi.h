
///
/// cEMI
///
#ifndef __CEMI_H
#define __CEMI_H

#include "target.h"

#define L_BUSMON_IND        ((BYTE)0x2B)
#define L_RAW_IND           ((BYTE)0x2D)
#define L_RAW_REQ           ((BYTE)0x10)
#define L_RAW_CON           ((BYTE)0x2F)
#define L_DATA_REQ          ((BYTE)0x11)
#define L_DATA_CON          ((BYTE)0x2E)
#define L_DATA_IND          ((BYTE)0x29)
#define L_POLL_DATA_REQ     ((BYTE)0x13)
#define L_POLL_DATA_CON     ((BYTE)0x25)
#define M_PROP_READ_REQ     ((BYTE)0xFC)
#define M_PROP_READ_CON     ((BYTE)0xFB)
#define M_PROP_WRITE_REQ    ((BYTE)0xF6)
#define M_PROP_WRITE_CON    ((BYTE)0xF5)
#define M_PROP_INFO_IND     ((BYTE)0xF7)
#define M_RESET_REQ         ((BYTE)0xF1)
#define M_RESET_IND         ((BYTE)0xF0)

#endif  // __CEMI_H

