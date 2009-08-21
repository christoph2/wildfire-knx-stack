/*****************************************************
      start12.c - standard startup code
   The startup code may be optimized to special user requests
 ----------------------------------------------------
   Copyright (c) HIWARE AG, Basel, Switzerland
               All rights reserved
                  Do not modify!

Note: ROM libraries are not implemented in this startup code
Note: C++ destructors of global objects are NOT yet supported in the HIWARE Object File Format.
      To use this feature, please build your application with the ELF object file format.
 *****************************************************/

#include "hidef.h"
#include "start12.h"

#if defined(__PROCESSOR_X4__) || defined(__HC12S__)    /* In some early version DBNE/... did not work. */
#define DO_NOT_USE_TEST_BRANCHES 1 /* Do not use DBNE/TBEQ,.... */
#else
#define DO_NOT_USE_TEST_BRANCHES 0 /* Do use DBNE/TBEQ,.... */
#endif

#if defined(__HC12S__) && (defined(FAR_DATA) || defined(__BANKED__))
#error /* no banked support */
#endif

#pragma DATA_SEG __NEAR_SEG STARTUP_DATA /* _startupData can be accessed using 16 bit accesses. This is needed because it contains the stack top, and without stack, far data cannot be accessed */
struct _tagStartup _startupData;  //   read-only:
                                  //   _startupData is allocated in ROM and
                                  //   initialized by the linker
#pragma DATA_SEG DEFAULT
#if defined(FAR_DATA)
#include "non_bank.sgm"
// the init function must be in non banked memory if banked variables are used
// because _SET_PAGE is called, which may change any page register.

#ifdef __cplusplus
  extern "C"
#endif
void _SET_PAGE(void);             // the inline assembler needs a prototype
                                  // this is a runtime routine with a special
                                  // calling convention, dont use it in c code!
static void Init(void);
static void Fini(void);
#else
#include "default.sgm"
#if defined( __BANKED__) || defined(__LARGE__)
static void __far Init(void);
static void __far Fini(void);
#endif // defined( __BANKED__) || defined(__LARGE__)
#endif // FAR_DATA

/* define value and bits for Windef Register */

#define WINDEF (*(volatile unsigned char*) 0x37)
#if defined( __BANKED__) || defined(__LARGE__) || defined(__PPAGE__)
#define __ENABLE_PPAGE__ 0x40
#else
#define __ENABLE_PPAGE__ 0x0
#endif

#if defined(__DPAGE__)
#define __ENABLE_DPAGE__ 0x80
#else
#define __ENABLE_DPAGE__ 0x0
#endif

#if defined(__EPAGE__)
#define __ENABLE_EPAGE__ 0x20
#else
#define __ENABLE_EPAGE__ 0x0
#endif


#if !defined(FAR_DATA) && (defined( __BANKED__) || defined(__LARGE__))
static void __far Init(void)
#else
static void Init(void)
#endif
 {
// purpose:     1) zero out RAM-areas where data is allocated
//              2) copy initialization data from ROM to RAM
//              3) call global constructors in C++
//   called from: _Startup, LibInits
   __asm {
ZeroOut:
#if defined(__HIWARE_OBJECT_FILE_FORMAT__) && defined(__LARGE__)
             LDX   _startupData.pZeroOut:1  ; in the large memory model in the HIWARE format, pZeroOut is a 24 bit pointer
#else
             LDX   _startupData.pZeroOut    ; *pZeroOut
#endif
             LDY   _startupData.nofZeroOuts ; nofZeroOuts
             BEQ   CopyDown                 ; if nothing to zero out

NextZeroOut: PSHY                           ; save nofZeroOuts
#ifdef __HC12S__
             LDY   0,X                      ; start address and advance *pZeroOut (X = X+4)
             LDD   2,X                      ; byte count
             LEAX  4,X
NextWord:    CLR   0,Y                      ; clear memory byte
             INY
#else  /* __HC12S__ */
#ifdef FAR_DATA
             LDAB  1,X+                     ; load page of destination address
             LDY   2,X+                     ; load offset of destination address
             __PIC_JSR(_SET_PAGE)                ; sets the page in the correct page register
#else   /* FAR_DATA */
             LDY   2,X+                     ; start address and advance *pZeroOut (X = X+4)
#endif  /* FAR_DATA */
             LDD   2,X+                     ; byte count
NextWord:    CLR   1,Y+                     ; clear memory byte
#endif /* __HC12S__ */
#if DO_NOT_USE_TEST_BRANCHES
             SUBD  #1                       ; for processor version which does not support DBNE
             BNE NextWord
#else
             DBNE  D, NextWord              ; dec byte count
#endif
             PULY                           ; restore nofZeroOuts
             DEY                            ; dec nofZeroOuts
             BNE  NextZeroOut
CopyDown:
#ifdef __ELF_OBJECT_FILE_FORMAT__
             LDX   _startupData.toCopyDownBeg ; load address of copy down desc.
#else
             LDX   _startupData.toCopyDownBeg:2 ; load address of copy down desc.
#endif
NextBlock:
#ifdef __HC12S__
             LDD   0,X                      ; size of init-data -> D
             BEQ   funcInits                ; end of copy down desc.
             LDY   2,X                      ; load destination address
             LEAX  4,X                      ; increment pointer
#else /* __HC12S__ */
             LDD   2,X+                     ; size of init-data -> D
             BEQ   funcInits                ; end of copy down desc.
#ifdef FAR_DATA
             PSHD                           ; save counter
             LDAB  1,X+                     ; load destination page
             LDY   2,X+                     ; destination address
             __PIC_JSR(_SET_PAGE)           ; sets the destinations page register
             PULD                           ; restore counter
#else  /* FAR_DATA */
             LDY   2,X+                     ; load destination address
#endif /* FAR_DATA */
#endif /* __HC12S__ */
Copy:
#ifdef __HC12S__
             PSHA                          ; move a byte from ROM to the data area
             LDAA  0,X
             STAA  0,Y
             PULA
             INX
             INY
#else  /* __HC12S__ */
             MOVB  1,X+,1,Y+                ; move a byte from ROM to the data area
#endif /* __HC12S__ */
#if DO_NOT_USE_TEST_BRANCHES
             SUBD  #1                       ; for processor version which does not support DBNE
             BNE    Copy
#else
             DBNE  D,Copy                   ; copy-byte loop
#endif
             BRA   NextBlock
funcInits:                                  ; call of global construtors is only in c++ necessary
#if defined(__cplusplus)
#if defined(__ELF_OBJECT_FILE_FORMAT__)
#if defined( __BANKED__) || defined(__LARGE__)

             LDY   _startupData.nofInitBodies; load number of cpp.
             BEQ   done                     ; if cppcount == 0, goto done
             LDX   _startupData.initBodies  ; load address of first module to initialize
nextInit:
             LEAX   3,X                     ; increment to next init
             PSHX                           ; save address of next function to initialize
             PSHY                           ; save cpp counter
             CALL  [-3,X]                   ; use double indirect call to load the page register also
             PULY                           ; restore cpp counter
             PULX                           ; restore actual address
             DEY                            ; decrement cpp counter
             BNE    nextInit
#else  /* defined( __BANKED__) || defined(__LARGE__) */

             LDD   _startupData.nofInitBodies; load number of cpp.
             BEQ   done                     ; if cppcount == 0, goto done
             LDX   _startupData.initBodies  ; load address of first module to initialize
nextInit:
#ifdef __HC12S__
             LDY   0,X                      ; load address of first module to initialize
             LEAX  2,X                      ; increment pointer
#else  /* __HC12S__ */
             LDY   2,X+                     ; load address of first module to initialize
#endif /* __HC12S__ */
             PSHD
             PSHX                           ; save actual address
             JSR   0,Y                      ; call initialization function
             PULX                           ; restore actual address
             PULD                           ; restore cpp counter
#if DO_NOT_USE_TEST_BRANCHES
             SUBD   #1                      ; decrement cpp counter
             BNE    nextInit
#else
             DBNE D, nextInit
#endif
#endif /* defined( __BANKED__) || defined(__LARGE__) */
#else /* __ELF_OBJECT_FILE_FORMAT__  */
             LDX   _startupData.mInits      ; load address of first module to initialize
#if defined( __BANKED__) || defined(__LARGE__)
nextInit:    LDY   3,X+                     ; load address of initialization function
             BEQ   done                     ; stop when address  == 0
                                            ; in common environments the offset of a function is never 0, so this test could be avoided
#ifdef __InitFunctionsMayHaveOffset0__
             BRCLR -1,X, done, 0xff         ; stop when address  == 0
#endif  /* __InitFunctionsMayHaveOffset0__ */
             PSHX                           ; save address of next function to initialize
             CALL  [-3,X]                   ; use double indirect call to load the page register also
#else  /* defined( __BANKED__) || defined(__LARGE__) */
nextInit:
#ifdef __HC12S__
             LDY   0,X                      ; load address of first module to initialize
             LEAX  2,X                      ; increment pointer
#else  /* __HC12S__ */
             LDY   2,X+                     ; load address of first module to initialize
#endif /* __HC12S__ */
             BEQ   done                     ; stop when address of function == 0
             PSHX                           ; save actual address
             JSR   0,Y                      ; call initialization function
#endif /* defined( __BANKED__) || defined(__LARGE__) */
             PULX                           ; restore actual address
             BRA   nextInit
#endif  /* __ELF_OBJECT_FILE_FORMAT__  */
done:
#endif /* __cplusplus */
   }
}

#if defined( __ELF_OBJECT_FILE_FORMAT__) && defined(__cplusplus )

#if !defined(FAR_DATA) && (defined( __BANKED__) || defined(__LARGE__))
static void __far Fini(void)
#else
static void Fini(void)
#endif
{
// purpose:     1) call global destructors in C++
   __asm {
#if defined( __BANKED__) || defined(__LARGE__)

             LDY   _startupData.nofFiniBodies; load number of cpp.
             BEQ   done                     ; if cppcount == 0, goto done
             LDX   _startupData.finiBodies  ; load address of first module to finalize
nextInit2:
             LEAX   3,X                     ; increment to next init
             PSHX                           ; save address of next function to finalize
             PSHY                           ; save cpp counter
             CALL  [-3,X]                   ; use double indirect call to load the page register also
             PULY                           ; restore cpp counter
             PULX                           ; restore actual address
             DEY                            ; decrement cpp counter
             BNE    nextInit2
#else  /* defined( __BANKED__) || defined(__LARGE__) */

             LDD   _startupData.nofFiniBodies; load number of cpp.
             BEQ   done                     ; if cppcount == 0, goto done
             LDX   _startupData.finiBodies  ; load address of first module to finalize
nextInit2:
#ifdef __STAR12__
             LDY   0,X                      ; load address of first module to finalize
             LEAX  2,X                      ; increment pointer
#else  /* __STAR12__ */
             LDY   2,X+                     ; load address of first module to finalize
#endif /* __STAR12__ */
             PSHD
             PSHX                           ; save actual address
             JSR   0,Y                      ; call finalize function
             PULX                           ; restore actual address
             PULD                           ; restore cpp counter
#if DO_NOT_USE_TEST_BRANCHES
             SUBD   #1                      ; decrement cpp counter
             BNE    nextInit2
#else
             DBNE D, nextInit2
#endif
#endif /* defined( __BANKED__) || defined(__LARGE__) */
done:;
   }
}
#endif


#include "non_bank.sgm"

#pragma MESSAGE DISABLE C12053 /* Stack-pointer change not in debugging-information */
#pragma NO_FRAME
#pragma NO_ENTRY
#pragma NO_EXIT

#ifdef __cplusplus
  extern "C"
#endif

/* The function _Startup must be called in order to initialize global variables and to call main */
/* You can adapt this function or call it from your startup code to implement a different startup */
/* functionality. */

/* You should also setup the needed IO registers as WINDEF (HC12A4 only) or the COP registers to run */
/* on hardware */

/* to set the reset vector several ways are possible : */
/* 1. define the function with "interrupt 0" as done below in the first case */
/* 2. add the following line to your prm file : VECTOR ADDRESS 0xfffe _Startup */
/* of course, even more posibilities exists */
/* the reset vector must be set so that the application has a defined entry point */

#define STARTUP_FLAGS_NOT_INIT_SP   (1<<1)

#if defined(__SET_RESET_VECTOR__)
void __interrupt 0 _Startup(void) {
#else
void _Startup(void) {
#endif
/*  purpose:    1)  initialize the stack
                2)  initialize the RAM, copy down init data etc (Init)
                3)  call main;
    parameters: NONE
    called from: _PRESTART-code generated by the Linker */
  for(;;) { /* forever: initialize the program; call the root-procedure */
      if (!(_startupData.flags&STARTUP_FLAGS_NOT_INIT_SP)) {
        /* initialize the stack pointer */
        INIT_SP_FROM_STARTUP_DESC(); /* HLI macro definition in hidef.h */
      }
      /* Here user defined code could be inserted, the stack could be used */


      /* Example : Set up WinDef Register to allow Paging */
#ifndef DG128 /* HC12 DG128 derivative has no WINDEF. Instead PPAGE is always enabled */
#if  (__ENABLE_EPAGE__ != 0 ||  __ENABLE_DPAGE__ != 0 || __ENABLE_PPAGE__ != 0)
      WINDEF= __ENABLE_EPAGE__ | __ENABLE_DPAGE__  | __ENABLE_PPAGE__;
#endif
#endif
      Init(); /* zero out, copy down, call constructors */
      /* Here user defined code could be inserted, all global variables are initilized */

      /* call main() */
      (*_startupData.main)();

      /* call destructors. Only done when this file is compiled as C++ and for the ELF object file format */
      /* the HIWARE object file format does not support this */
#if defined( __ELF_OBJECT_FILE_FORMAT__) && defined(__cplusplus )
      Fini();
#endif

   }; /* end loop forever */
}
