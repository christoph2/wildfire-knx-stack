#ifndef __LOADCONTROLS_H
#define __LOADCONTROLS_H

#define LC_ABSCOBJSEG(StartAddr)        // Table-Type-ID for BIM112 is 00 01 00 01 00H.
#define LC_ABSDATASEG(Type,Address,EndAddress,AccAttrs,MemoryType,MemAttrs)
#define LC_ABSSTACKSEG(Address,EndAddress,AccAttrs,MemoryType,MemAttrs)
#define LC_ABSTASKSEG(Type,Address,PEIType,AppID)
#define LC_COMMANDS(Offset)     // sollte 0x0000 sein.
#define LC_COMMANDSEND()
#define LC_COMPAREDIRECT(Start,D1,D2,D3,D4,D5,D6,D7,D8,D9)
#define LC_CONNECT()
#define LC_DELAY(Value)
#define LC_DISCONNECT()
#define LC_EIBOBJDEF()          // Nur BCU2
#define LC_LOAD(Type)
#define LC_LOADCOMPLETED(Type)
#define LC_PROPCOMPARE()        // Nur BCU2
#define LC_PROPWRITE()          // Nur BCU2
#define LC_PROPWRITEVERIFY()    // Nur BCU2
#define LC_RESTART()
#define LC_TASKCTRLREC1()       // Nur BCU2
#define LC_TASKCTRLREC2()       // Nur BCU2
#define LC_TASKPTRRECORD()      // Nur BCU2
#define LC_UNLOAD(Type)
#define LC_WRITE(Start,End)
#define LC_WRITEDIRECT(Start,D1,D2,D3,D4,D5,D6,D7,D8,D9)
#define LC_WRITEWITHVERIFY(Start,End)

#endif  // __LOADCONTROLS_H

