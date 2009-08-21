#if !defined(__MEMORY_H)
#define __MEMORY_H

/* #include "target.h" */
#include <Std_Types.h>

typedef enum tagMemoryType { mtROM,mtZPRAM,mtRAM,mtEEPROM } MemoryType;

typedef struct tagMemoryControlBlock {
    uint16 OriginalAdress;
    void *MappedAdress;
    uint16 len;
    MemoryType mt;
    uint8 access;    /* todo: enum. */
        
} MemoryControlBlock;

void MM_ClearMCBs(void);

uint16 MM_SetByte(uint16 Address,uint8 value);
uint16  MM_GetByte(uint16 Address,uint8 *value);
uint16 MM_MapAddressToTarget(uint16 Address);
uint16 MM_MapAddressFromTarget(uint16 Address);


void CopyMem(void *d,void *s,uint16 len);
void FillMem(void *p,uint8 b,uint16 len);
void ZeroMem(void *p,uint16 len);
boolean CompMem(void *p1,void *p2,uint16 len);

void CopyRAM(void *d,void *s,uint16 len);
void FillRAM(void *p,uint8 b,uint16 len);
void ZeroRAM(void *p,uint16 len);

#define CompRAM	CompMem
#define CopyMem	CopyRAM

#endif /* __MEMORY_H */
