/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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
#include "knx_conversion.hpp"


#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#include <iostream>

using namespace std;


namespace knx {

DPT9::DPT9(int value)
{
    uint16_t mantissa;
    uint8_t exponent;
    bool sign;
    double res;

    value = static_cast<uint16_t>(value);
    _raw_value = value;
    sign = (hiByte(value) & (uint8_t)0x80);
    mantissa = value & (uint16_t)0x07ffU;
    if (sign) {
        mantissa = (twosComplement(mantissa)) & (uint16_t)0x07ffU;
    }
    exponent = (hiByte(value) & (uint8_t)0x78) >> 3;
    res = (double)((mantissa << exponent) / (double)100.0);
    if (sign) {
        res = -res;
    }
    _fp_value = res;
}

DPT9::DPT9(double value)
{
    uint16_t mantissa;
    uint8_t exponent;
    bool sign;
    uint16_t res;

    _fp_value = value;

    exponent   = (uint8_t)0;
    res        = (uint16_t)0U;
    sign       = (value < (double)0.0);
    if (sign) {
        value = -value;
    }
    while (value > (double)20.47) {
        value     /= (double)2.0;
        exponent  += (uint8_t)1;
    }
    mantissa = (uint16_t)(value * (double)100.0);
    if (sign) {
        mantissa   = (twosComplement(mantissa)) & (uint16_t)0x07ffU;
        res       |= (uint16_t)0x8000;
    }
    res |= ((((exponent << 3) & (uint16_t)0x0078U) * (uint16_t)0x0100U) | mantissa);
    _raw_value = res;
}

DPT9::operator double() const
{
    return _fp_value;
}


} // namespace knx
