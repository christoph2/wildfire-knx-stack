/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
 *                                       cpu12.gems@googlemail.com>
 *
 *   All Rights Reserved
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#include "knx_lsm.h"

/*

   // States.
   #define KNX_LSM_STATE_UNLOADED    0   // no data loaded.
   #define KNX_LSM_STATE_LOADED      1   // data loaded.
   #define KNX_LSM_STATE_LOADING     2   // load process is active.
   #define KNX_LSM_STATE_ERROR       3   // error in data detected.


   // Events.
   #define KNX_LSM_EVENT_NO_OPERATION    0     // nothing to do.
   #define KNX_LSM_EVENT_START_LOAD      1     // the statemachine is change in to Loading.
   #define KNX_LSM_EVENT_LOAD_COMPLETE   2     // the statemachine is change in to Loaded.
   #define KNX_LSM_EVENT_UNLOAD          4     // the statemachine is change in to Unloaded.
   #define KNX_LSM_EVENT_SEGMENT         3     // Allocation / Information Records.

 */

/*
   Load-Event              Value
   ==========              =====
   No Operation    0x00
   Start Load              0x01
   Load Complete   0x02
   Segment                 0x03
        Absolute Code/Data Allocation Record    0x00
        Segment Control Record                                  0x02
        Task Pointer Record                                             0x03
        Task Control Record1                                    0x04
        Task Control Record2                                    0x05
   Unload                  0x04
 */

/* "EEPROM-checksum is set when the load state of an object is set to "loaded". */

/*
** Status "unloaded" does not just mean that no valid data are in the BCU
** (that means every allocated memory is released, but without the event
** "segment" [start load + load complete] it can't reach a valid memory structure,
**  e.g. the status "loaded")
*/

/*
   Load Status             Value   Remark
   ===========             =====   ======
   Unloaded                0x00    no data loaded
   Loaded                  0x01    data loaded
   Loading                 0x02    load process is active
   Error                   0x03    error in data detected
 */

/*
   State           NOP                     START_LOAD              LOAD_COMPLETE   SEGMENT         UNLOAD
   ----------------------------------------------------------------------------
   UNLOADED        UNLOADED*       LOADING                 UNLOADED*               UNLOADED*       UNLOADED
   LOADED          LOADED*         LOADING                 LOADED*                 LOADED*         UNLOADED*
   LOADING         LOADING*        LOADING*                LOADED                  LOADING*        UNLOADED*
   ERROR           ERROR*          ERROR*                  ERROR*                  ERROR*          UNLOADED*
 */

typedef enum tagKNX_LSMEventType {
    LSM_EVENT_NOP, LSM_EVENT_START_LOAD, LSM_EVENT_LOAD_COMPLETE,
    LSM_EVENT_SEGMENT, LSM_EVENT_UNLOAD, LSM_EVENT_LAST
} KNX_LSMEventType;

typedef enum tagKNX_LSMStateType {
    LSM_STATE_UNLOADED, LSM_STATE_LOADED, LSM_STATE_LOADING,
    LSM_STATE_ERROR, LSM_STATE_LAST
} KNX_LSMStateType;

STATIC const uint8_t LS_Table[4][5] = {
/*  NOP                     START_LOAD          LOAD_COMPLETE       SEGMENT             UNLOAD                  */
/*  ----------------------------------------------------------------------------------------------------------- */
    {LSM_STATE_UNLOADED, LSM_STATE_LOADING,   LSM_STATE_UNLOADED,   LSM_STATE_UNLOADED,    LSM_STATE_UNLOADED                                 },    /* UNLOADED */
    {LSM_STATE_LOADED,   LSM_STATE_LOADING,   LSM_STATE_LOADED,     LSM_STATE_LOADED,      LSM_STATE_UNLOADED                                 },    /* LOADED */
    {LSM_STATE_LOADING,  LSM_STATE_LOADING,   LSM_STATE_LOADED,     LSM_STATE_LOADING,     LSM_STATE_UNLOADED                                 },    /* LOADING */
    {LSM_STATE_ERROR,    LSM_STATE_ERROR,     LSM_STATE_ERROR,      LSM_STATE_ERROR,       LSM_STATE_UNLOADED                                 },    /* ERROR */
};

typedef uint8_t LoadEventType[10];

#define LE_EVENT    ((uint8_t)0)
#define LE_SUBCODE  ((uint8_t)1)

/*
   typedef void (*ACTIONFUNC)(void);

   void Nop();

   typedef struct tagAction {
        uint8_t next_state;
        ACTIONFUNC f;
   } ACTION;

   ACTION __LSM[][]=
   {

   };
 */

/*
**  LOAD-/State-Controls.
*/

#if     defined(__HIWARE__)
        #pragma DATA_SEG        EEPROM
#endif
KNX_LSCType KNX_SystemLSC[KNX_NUM_SYS_LSCS];    /* System-Load-/State-Controls. */
#if     defined(__HIWARE__)
        #pragma DATA_SEG DEFAULT
#endif

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

/* todo: als Makro!!! */
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) LSM_IsAppLoaded(void)
#else
boolean LSM_IsAppLoaded(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KNX_SystemLSC[KNX_LSC_APP] == LSM_STATE_LOADED;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) LSM_IsGrATLoaded(void)   /* Address-Table. */
#else
boolean LSM_IsGrATLoaded(void)                      /* Address-Table. */
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KNX_SystemLSC[KNX_LSC_GRAT] == LSM_STATE_LOADED;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(boolean, KSTACK_CODE) LSM_IsGrOATLoaded(void)  /* Assoc-Table. */
#else
boolean LSM_IsGrOATLoaded(void)                     /* Assoc-Table. */
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KNX_SystemLSC[KNX_LSC_GROAT] == LSM_STATE_LOADED;
}


/*
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) LSM_Init(void)
#else
void LSM_Init(void)
#endif /* KSTACK_MEMORY_MAPPING */
{

}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) LSM_Dispatch(P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) record,
                                     /*LSM_State*/ P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) ls_var
                                     )
#else
void LSM_Dispatch(uint8_t * record, /*LSM_State*/ uint8_t * ls_var)
#endif /* KSTACK_MEMORY_MAPPING */
/*
**      record: Pointer auf das 10-Byte-Load-Control.
**      ls_var: Zeiger auf die Load-State-Variable.
*/
{
    /*LSM_State*/ uint8_t new_state;
    /*LSM_Event*/ uint8_t event;

    event = record[0];

    if (event >= LSM_EVENT_LAST) {
        event = LSM_EVENT_NOP;
    }

    new_state = LS_Table[*ls_var][event];

    if (new_state >= LSM_STATE_LAST) {
        new_state = LSM_STATE_ERROR;
    }

    *ls_var = new_state;
}


/* static uint8_t app_rec[10]; */
/*static LSM_Stateuint8 app_lsc; */
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) LSM_Test(void)
#else
void LSM_Test(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t i;

/*
        app_lsc=LSM_STATE_UNLOADED;
        while (1) {
                LSM_Dispatch(app_rec,&app_lsc);
        }
 */
    for (i = (uint8_t)0; i < KNX_NUM_SYS_LSCS; i++) {
        KNX_SystemLSC[i] = LSM_STATE_LOADED;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
