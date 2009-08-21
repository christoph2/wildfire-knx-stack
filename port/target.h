//
//
//  todo:   Diese Datei (natürlich) für jedes Target getrennt anlegen!!! 
//          Vorallem gehören hier die Datentyp-Definitionen hin!!!
//
//          !!! MIT OSEK/AUTOSAR ABGLEICHEN!!!
//

#ifndef __TARGET_H
#define __TARGET_H

#define KNX_LITTLE_ENDIAN
#undef  KNX_BIG_ENDIAN

#ifndef FALSE
#define FALSE   ((BOOL)0)
#endif  // FALSE

#ifndef TRUE
#define TRUE    ((BOOL)1)
#endif  // TRUE

#if !defined(NULL)
    #define NULL    0
#endif

// CriticalSection cs;
//  InitializeCriticalSection(&cs); // Muss irgendwo hin!!!


// todo: falls miniOSEK verwendet wird, 'DISABLE_ALL_INTERRUPTS()/ENABLE_ALL_INTERRUPTS()' auf 'DisableAllInterrupts()/EnableAllInterrupts' abbilden!!!


#if defined(_MSC_VER) || defined(__WATCOMC__)
        #define DISABLE_ALL_INTERRUPTS()
        #define ENABLE_ALL_INTERRUPTS()
#endif

// todo: Aenderungen gem. miniOSEK!!!
// Disable All Interrupts.
#if defined(__IMAGECRAFT__)
#define DISABLE_ALL_INTERRUPTS()                        \
        do {                                                            \
                asm("pshc"); asm("sei");        \
        } while(0)
#elif defined(__HIWARE__)
#define DISABLE_ALL_INTERRUPTS()                        \
        do {                                                            \
                __asm pshc; __asm sei;          \
        } while(0)
#endif

// Enable All Interrupts.
#if defined(__IMAGECRAFT__)
#define ENABLE_ALL_INTERRUPTS()                         \
        do {                                                            \
                asm("pulc"); asm("nop");        \
        } while(0)      
#elif defined(__HIWARE__)
#define ENABLE_ALL_INTERRUPTS()                         \
        do {                                                            \
                __asm pulc; __asm nop;          \
        } while(0)      
#endif  




//#define ENABLE_ALL_INTERRUPTS()   asm("cli");
//#define DISABLE_ALL_INTERRUPTS()   asm("sti");

typedef unsigned char BYTE,*PBYTE;
typedef unsigned short WORD,*PWORD;
typedef unsigned long   DWORD,*PDWORD;


typedef signed char SBYTE,*PSBYTE;
typedef signed short SWORD,*PSWORD;
typedef signed long     SDWORD,*PSDWORD;

typedef BYTE BOOL,*PBOOL;

typedef void *ADDR_T;   // todo: weg damit!!!

#define UNREFERENCED_PARAMETER(p)   (p)=(p)

#define MIN(a,b)        ((a)>(b)) ? (b) : (a)
#define MAX(a,b)        ((a)>(b)) ? (a) : (b)

#define LOBYTE(w)       (BYTE)((WORD)((w) & 0x00ff))
#define HIBYTE(w)       (BYTE)((WORD)(((w)>>8) & 0x00ff))

#define MAKEWORD(h,l)   ((h) & 0xff)<<8 | ((l) & 0xff)


#define DIMENSION_OF(arr) (sizeof((arr))/sizeof((arr)[0]))
/* sehr gut f. 'for' Schleifen! (idx<BEYOND(arr)) */
#define BEYOND(arr)        ((arr) + DIMENSION_OF((arr)))

/*

// Hinweis: Variante: _ASSERT()-Makros auch im laufenden Betrieb (Reset, Critical-Error-Hook).

    BOOL STK_CriticalErrorHandler(BYTE severity,BYTE cause);  // User-supplied-Handler,
            RESET if returns TRUE (in Abhängigkeit von 'severity').

#define _ASSERT(cond)   \
#ifdef  DEBUG           \


#endif  \\ DEBUG

#define _DBG_PRINT(msg)    \
#ifdef  DEBUG              \


#endif  \\ DEBUG
*/
#define BIT0    0x01
#define BIT1    0x02
#define BIT2    0x04
#define BIT3    0x08
#define BIT4    0x10
#define BIT5    0x20
#define BIT6    0x40
#define BIT7    0x80
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000

#define _GETBIT(port,bit)     (((port) & (bit))==(bit)) ? (TRUE) : (FALSE)
#define _SETBIT(port,bit)     (port) |= (bit)
#define _CLRBIT(port,bit)     (port) &= (~(bit))

#define __TARGET_ECLK   8000*1000

#endif  // __TARGET_H

