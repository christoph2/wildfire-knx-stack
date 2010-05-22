
/* #include <windows.h>  */
/* #include <tchar.h> */
/* #include <conio.h> */

void TPTest(void);

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
    LSM_Test();
/*    
    ios_test();
*/            
    KNXStartSystem();

    while (1) {
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

