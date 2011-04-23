/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
#if !defined(__LOADCONTROLS_H)
#define __LOADCONTROLS_H

#define LC_ABSCOBJSEG(StartAddr)    /* Table-Type-ID for BIM112 is 00 01 00 01 00H. */
#define LC_ABSDATASEG(Type, Address, EndAddress, AccAttrs, MemoryType, MemAttrs)
#define LC_ABSSTACKSEG(Address, EndAddress, AccAttrs, MemoryType, MemAttrs)
#define LC_ABSTASKSEG(Type, Address, PEIType, AppID)
#define LC_COMMANDS(Offset)         /* should be 0x0000. */
#define LC_COMMANDSEND()
#define LC_COMPAREDIRECT(Start, D1, D2, D3, D4, D5, D6, D7, D8, D9)
#define LC_CONNECT()
#define LC_DELAY(Value)
#define LC_DISCONNECT()
#define LC_EIBOBJDEF()          /* only BCU2 */
#define LC_LOAD(Type)
#define LC_LOADCOMPLETED(Type)
#define LC_PROPCOMPARE()        /* only BCU2 */
#define LC_PROPWRITE()          /* only BCU2 */
#define LC_PROPWRITEVERIFY()    /* only BCU2 */
#define LC_RESTART()
#define LC_TASKCTRLREC1()       /* only BCU2 */
#define LC_TASKCTRLREC2()       /* only BCU2 */
#define LC_TASKPTRRECORD()      /* only BCU2 */
#define LC_UNLOAD(Type)
#define LC_WRITE(Start, End)
#define LC_WRITEDIRECT(Start, D1, D2, D3, D4, D5, D6, D7, D8, D9)
#define LC_WRITEWITHVERIFY(Start, End)

#endif  /* __LOADCONTROLS_H */

