
/* #include <windows.h>  */
/* #include <tchar.h> */
/* #include <conio.h> */

void TPTest(void);

/*
**
**  Hinweis: eine Übersicht der zu implementierenden Server bietet
**           'EIB HB 3/4/1 layer user' !!!
*/

/*
**	todo: Wo sinnvoll, anonyme Enums verwenden!!!
*/

/*
**  XXX GmbH - KONNEX Knowledge Base.   ...Integrated Solutions GmbH.   ...BusTools...
**
**  - ADVANCED PROGRAMMING INSTEAD OF ADVANCED MARKETING -
**
*/

/*	knOSEK ('kn' t.b.d.),	knxOS. */


/*
**  Hinweis: 'virtuelle' KNX-IP-Geräte kommen mit UDP(Multicast)/IP aus!
** certified stacks: SBT, Tapko, Weinzierl.
** RF-Komponenten: Chipcon.
** KNX-RF Generator-Polynom: 2^16+2^13+2^12+2^11+2^10+2^8+2^6+2^5+2^2+2^0
**  ??? Wie wäre es das 'EXTENDED_STATUS'-Konzept von OSEK/VDX zu übernehmen ???
*/

/*  PIXIT = Protocol Implementation eXtra Information for Testing according ISO 9646 */
/*  ‘Information Technology Open Systems Interconnection' Conformance testing methodology and framework’. */

/*  The term Service Access Point (SAP) is defined in ISO/IEC 74981-1 "The Basic Model" */
/*  as "The point at which (N)-services are provided by the (N)-entity to an (N+1)-entity". */
/*
**
**  Versions-Systematik: Updates (check: anderer Name?) sind zertifiziert; Previews hingegen nicht,
**      sie stellen lediglich einen Ausblick auf kommende Versionen/Updates.
**
*/

/*
Parameter-Format / .MAP/SYM.-Dateien (ETS-Hersteller-Tool)
==========================================================

Parsing parameters

Term:   PAR_TYPE_LENGTH_OFFSET_NAME

PAR     Prefix for parameter name
TYPE    Defines the parameter type
LENGTH  Defines the parameter length (bit). Must be a positive value.
OFFSET  Parameter position in the byte (bit). Must be a positive value between 0 and 7.
NAME    Defines the real parameter name

    For TYPE the following abbreviations are defined:

    UI      unsigned integer
    SI      signed integer
    ST      string
    EN      enum
    NO      none

Remark: The "Parameter Default Value" is parsed from the S19 file.

Example:        PAR_EN_16_0_Delay time
                A parameter with a type of enum, a length of 16 bit, an offset of 0 and the name 'Delay time'is defined.
*/

/* INSTA ELEKTRO GmbH & Co. KG: Data-Interface-Modul. */

/* 'PATTERN MINING' !!! */

/* ISO/IEC TR 18015:2004(E) Technical Report on C++ Performance */

void KNXStartSystem(void);
void Scheduler(void);


#include "tpuart.h"
#include "Network.h"
#include "Transport.h"
#include "Application.h"
#include "Timer.h"
#include "Messaging.h"
#include "Address.h"
#include "Appl.h"
#include "KNXConv.h"

#include "AIL.h"

void ios_test(void);
void LSM_Test(void);

/*
void bswap(void)
{
    int x,y;
    x=0x11;
    y=0x33;
        // Here’s a neat programming trick to swap 
        // two values without needing a temp:
        x = x ^ y;
        y = x ^ y;
        x = x ^ y;
}
*/
void UtilSwapBytes(int *x,int *y);

void UtilSwapBytes(int *x,int *y) 
{
    *x^=*y;
    *y^=*x;
    *x^=*y;
}

int main()
{
#if 0    
    KNX_AddressType src,nt;    
    uint8 i,rc;
#endif
    LSM_Test(); /* Die Load-Controls zum Testen auf 'LOADED' setzen. */
/*    
    ios_test();
*/            
    KNXStartSystem();

    while (1) { /* todo: Nur solange bis der Stack heruntergefahren wird!!! */
        Scheduler();
/*        
        if (++cnt==1000*1000) {
            return 0;
        }
*/        
    }

    return 0;
}

void Scheduler(void)
{
    TPTest();
    LL_Task();
    NL_Task();
    TLG_Task();
    TLC_Task();
    ALG_Task();
    ALM_Task();
    ALG_PollCycle();
    /* check: das ganze nochmal in umgekehrter Reihenfolge??? */    
}

void KNXStartSystem(void)
{
    DEV_Init();
    TPUARTInit();
    LSM_Init();
    NL_Init();
    TL_Init();
    MSG_Init();
    TM_Init();
    ALG_Init();
}
