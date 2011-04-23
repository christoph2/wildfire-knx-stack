/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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

#include "KNXConv.h"

#if defined(KNX_LITTLE_ENDIAN)
uint16 btohs(uint16 w)
{
    return MAKEWORD(LOBYTE(w), HIBYTE(w));
}

#endif  /* defined(KNX_LITTLE_ENDIAN) */

#define COMW(w) ((~(w)) + 1)

uint16 LongToDPT9(sint32 value)
{
    uint16  Mantisse, Res;
    uint8   Exponent;
    boolean Sign;

    Exponent   = (uint8)0;
    Res        = (uint16)0U;
    Sign       = (value < 0);

    if (Sign) {
        value = -value;
    }

    while (value > (sint32)0x07ffL) {
        value    >>= 1;
        Exponent  += (uint8)1;
    }

    Mantisse = (uint16)value;

    if (Sign) {
        Mantisse   = (COMW(Mantisse)) & (uint16)0x07ffU;
        Res       |= (uint16)0x8000U;
    }

    Res |= ((((Exponent << 3) & (uint16)0x0078U) * (uint16)0x0100U) | Mantisse);

    return Res;
}

uint16 FloatToDPT9(float value)
{
    uint16  Mantisse, Res;
    uint8   Exponent;
    boolean Sign;

    Exponent   = (uint8)0;
    Res        = (uint16)0U;
    Sign       = (value < (float)0.0);

    if (Sign) {
        value *= (float)-1.0;
    }

    while (value > (float)20.47) {
        value     /= (float)2.0;
        Exponent  += (uint8)1;
    }

    Mantisse = (uint16)(value * (float)100.0);

    if (Sign) {
        Mantisse   = (COMW(Mantisse)) & (uint16)0x07ffU;
        Res       |= (uint16)0x8000;
    }

    Res |= ((((Exponent << 3) & (uint16)0x0078U) * (uint16)0x0100U) | Mantisse);

    return Res;
}

float DPT9ToFloat(uint16 value)
{
    uint16  Mantisse;
    uint8   Exponent;
    boolean Sign;
    float   Res;

    Sign       = (HIBYTE(value) & (uint8)0x80) == (uint8)0x80;
    Mantisse   = value & (uint16)0x07ffU;

    if (Sign) {
        Mantisse = (COMW(Mantisse)) & (uint16)0x07ffU;
    }

    Exponent = (HIBYTE(value) & (uint8)0x78) >> 3;

    Res = (float)((Mantisse << Exponent) / (float)100.0);

    if (Sign) {
        Res = Res * (float)-1.0;
    }

    return Res;
}

sint32 DPT9ToLong(uint16 value)
{
    uint16  Mantisse;
    uint8   Exponent;
    boolean Sign;
    sint32  Res;

    Sign       = (HIBYTE(value) & (uint8)0x80) == (uint8)0x80;
    Mantisse   = value & 0x7ff;

    if (Sign) {
        Mantisse = (COMW(Mantisse)) & (uint16)0x07ffU;
    }

    Exponent = (HIBYTE(value) & (uint8)0x78) >> 3;

    Res = (sint32)((Mantisse << Exponent) /*/100.0*/);

    if (Sign) {
        Res = -Res;
    }

    return Res;
}

