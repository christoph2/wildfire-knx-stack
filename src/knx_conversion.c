/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
*                                       cpu12.gems@googlemail.com>
*
*   All Rights Reserved
*
*  This program is free softwKNXe; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free SoftwKNXe Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#include "knx_conversion.h"

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
/*
** Global functions.
*/

#if defined(KNX_LITTLE_ENDIAN)
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint16_t, KSTACK_CODE) btohs(uint16_t w)
#else
uint16_t btohs(uint16_t w)                  // TODO: knx_utl!!! auﬂerdem: ByteOrder!!!
#endif /* KSTACK_MEMORY_MAPPING */
{
    return KNX_MAKEWORD(KNX_LOBYTE(w), KNX_HIBYTE(w));
}


#endif  /* defined(KNX_LITTLE_ENDIAN) */

#define COMW(w) ((~(w)) + 1)

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint16_t, KSTACK_CODE) LongToDPT9(sint32 value)
#else
uint16_t LongToDPT9(int32_t value)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint16_t  Mantissa;
    uint8_t   Exponent;
    boolean Sign;
    uint16_t  Res;

    Exponent   = (uint8_t)0;
    Res        = (uint16_t)0U;
    Sign       = (value < 0);

    if (Sign) {
        value = -value;
    }

    while (value > (int32_t)0x07ffL) {
        value    >>= 1;
        Exponent  += (uint8_t)1;
    }

    Mantissa = (uint16_t)value;

    if (Sign) {
        Mantissa   = (COMW(Mantissa)) & (uint16_t)0x07ffU;
        Res       |= (uint16_t)0x8000U;
    }

    Res |= ((((Exponent << 3) & (uint16_t)0x0078U) * (uint16_t)0x0100U) | Mantissa);

    return Res;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint16_t, KSTACK_CODE) FloatToDPT9(float64 value)
#else
uint16_t FloatToDPT9(float64 value)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint16_t  Mantissa;
    uint8_t   Exponent;
    boolean Sign;
    uint16_t  Res;

    Exponent   = (uint8_t)0;
    Res        = (uint16_t)0U;
    Sign       = (value < (float)0.0);

    if (Sign) {
        value *= (float)-1.0;
    }

    while (value > (float)20.47) {
        value     /= (float)2.0;
        Exponent  += (uint8_t)1;
    }

    Mantissa = (uint16_t)(value * (float)100.0);

    if (Sign) {
        Mantissa   = (COMW(Mantissa)) & (uint16_t)0x07ffU;
        Res       |= (uint16_t)0x8000;
    }

    Res |= ((((Exponent << 3) & (uint16_t)0x0078U) * (uint16_t)0x0100U) | Mantissa);

    return Res;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(float64, KSTACK_CODE) DPT9ToFloat(uint16_t value)
#else
float64 DPT9ToFloat(uint16_t value)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint16_t  Mantissa;
    uint8_t   Exponent;
    boolean Sign;
    float   Res;

    Sign = (KNX_HIBYTE(value) & (uint8_t)0x80) == (uint8_t)0x80;
    Mantissa   = value & (uint16_t)0x07ffU;

    if (Sign) {
        Mantissa = (COMW(Mantissa)) & (uint16_t)0x07ffU;
    }

    Exponent = (KNX_HIBYTE(value) & (uint8_t)0x78) >> 3;

    Res = (float)((Mantissa << Exponent) / (float)100.0);

    if (Sign) {
        Res = Res * (float)-1.0;
    }

    return Res;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(sint32, KSTACK_CODE) DPT9ToLong(uint16_t value)
#else
int32_t DPT9ToLong(uint16_t value)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint16_t  Mantissa;
    uint8_t   Exponent;
    boolean Sign;
    int32_t  Res;

    Sign = (KNX_HIBYTE(value) & (uint8_t)0x80) == (uint8_t)0x80;
    Mantissa   = value & 0x7ff;

    if (Sign) {
        Mantissa = (COMW(Mantissa)) & (uint16_t)0x07ffU;
    }

    Exponent = (KNX_HIBYTE(value) & (uint8_t)0x78) >> 3;

    Res = (int32_t)((Mantissa << Exponent) /*/100.0*/);

    if (Sign) {
        Res = -Res;
    }

    return Res;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
