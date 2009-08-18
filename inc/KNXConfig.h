#ifndef __KNXCONFIG_H
#define __KNXCONFIG_H

/*
**
**  Hinweis: Diese Datei ist Projekt-Abhängig!!!
**
*/

#define KNX_STANDALONE  // Standalone im Gegensatz zu: Benutzt miniOSEK.
#define KNX_CLIENT_LIB
#define KNX_USE_CONFIMATIONS
#define KNX_BUSACCESS_TPUART
#define KNX_TL_STYLE    3
#undef  KNX_TL_RATIONALIZE
#define KNX_USER_OBJECTS
#define KNX_MASK_TYPE   21

//#define   KNX_???_SAVE_COPY   _STRICT _CHECK

#endif  // __KNXCONFIG_H

