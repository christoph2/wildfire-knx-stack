#if !defined(__KNXCONV_H)
#define __KNXCONV_H

#include "KNXDefs.h"

uint16 LongToDPT9(sint32 value);
/* todo: double!!! */
uint16 FloatToDPT9(float value);
float DPT9ToFloat(uint16 value);
sint32 DPT9ToLong(uint16 value);

#if defined(__HIWARE__)
#define KNX_BIG_ENDIAN
#undef  KNX_LITTLE_ENDIAN
#elif defined(_MSC_VER)
#undef  KNX_BIG_ENDIAN
#define KNX_LITTLE_ENDIAN
#endif

#if defined(KNX_BIG_ENDIAN)
#define btohs(w) (w)
#define htobs(w) (w)

#elif defined(KNX_LITTLE_ENDIAN)

/* #define btohs(w)    MAKEWORD(LOBYTE((w)),HIBYTE((w))) */

uint16 btohs(uint16 w);
#define htobs(w)    btohs(w)
#else
 #error "No Endianess defined!!!"
#endif

#endif  /* __KNXCONV_H */
