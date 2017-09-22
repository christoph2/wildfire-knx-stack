/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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
*  but WITHOUT ANY WKNXRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#include "knx_utl.hpp"

namespace knx {

constexpr uint8_t Utl_SetBitTab8[8] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

constexpr uint8_t Utl_ClearBitTab8[8] = {
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F
};

constexpr uint16_t Utl_SetBitTab16[16] = {
    0x0001U, 0x0002U, 0x0004U, 0x0008U, 0x0010U, 0x0020U, 0x0040U, 0x0080U,
    0x0100U, 0x0200U, 0x0400U, 0x0800U, 0x1000U, 0x2000U, 0x4000U, 0x8000U
};

const uint16_t Utl_ClearBitTab16[16] = {
    0xFFFEU, 0xFFFDU, 0xFFFBU, 0xFFF7U, 0xFFEFU, 0xFFDFU, 0xFFBFU, 0xFF7FU,
    0xFEFFU, 0xFDFFU, 0xFBFFU, 0xF7FFU, 0xEFFFU, 0xDFFFU, 0xBFFFU, 0x7FFFU
};

#if 0
#define BIT_8(n) ((const uint8_t[8]) {1, 2, 4, 8, 16, 32, 64, 128})[n];
#endif

static uint32_t NextRandomNumber = 1;

bool Utl_BitGet(uint16_t w, uint8_t num)
{
    return (w & Utl_SetBitTab16[num]) != (uint16_t)0x0000U;
}


uint16_t Utl_BitSet(uint16_t w, uint8_t num)
{
    if (num > 15) {
        return w;
    }
    return w |= Utl_SetBitTab16[num];
}


uint16_t Utl_BitReset(uint16_t w, uint8_t num)
{
    return w &= ~(Utl_SetBitTab16[num]);
}


uint16_t Utl_BitToggle(uint16_t w, uint8_t num)
{
    return w ^= Utl_SetBitTab16[num];
}

uint16_t Utl_BitGetHighest(uint16_t w)
{
    w |= w >> 1;
    w |= w >> 2;
    w |= w >> 4;
    w |= w >> 8;

    return (uint16_t)(w ^ (w >> 1));
}


uint16_t Utl_BitGetLowest(uint16_t w)
{
    return (uint16_t)((~w + 1) & w);
}


uint16_t Utl_BitSetLowest(uint16_t w)
{
    return (uint16_t)(w | (w + 1));
}


uint16_t Utl_BitResetLowest(uint16_t w)
{
    return (uint16_t)(w & (w - 1));
}


uint8_t Utl_Log2(uint16_t num)
{
    uint8_t res = (uint8_t)0x00;

    while (num >>= 1) {
        res++;
    }

    return res;
}


Utl_EndianessType Utl_CheckEndianess(void)
{
    const uint16_t    foo    = 0xaa55U;
    uint8_t *         ptr    = (uint8_t *)&foo;

    if (0[ptr] == (uint8_t)0xaa) {
        return Utl_EndianessType::BIG;
    } else {
        return Utl_EndianessType::LITTLE;
    }
}


void Utl_MemCopy(void * dst, void * src, uint16_t len)
{
    uint8_t * pd = (uint8_t *)dst;
    uint8_t * ps = (uint8_t *)src;

    ASSERT(dst != (void *)NULL);
    ASSERT(pd >= ps + len || ps >= pd + len);
    ASSERT(len != (uint16_t)0);

    while (len--) {
        *pd++ = *ps++;
    }

}


void Utl_MemSet(void * dest, uint8_t fill_char, uint16_t len)
{
    uint8_t * p = (uint8_t *)dest;

    ASSERT(dest != (void *)NULL);

    while (len--) {
        *p++ = fill_char;
    }
}


void Utl_Randomize(uint16_t seed)
{
    NextRandomNumber = (uint32_t)seed;
}


uint16_t Utl_Random(void)
{
    NextRandomNumber = NextRandomNumber * (uint32_t)0x41C64E6DUL + (uint32_t)0x3039UL;
    return (uint16_t)(NextRandomNumber / (uint32_t)0x10000UL) % (uint32_t)0x8000UL;

}


uint16_t Utl_Sqrt16(uint16_t x)
{
   uint16_t res = 1U;
   uint16_t sqr = 1U;
   uint16_t delta = 3U;

   while (sqr <= x) {
      res += 1U;
      sqr += delta;
      delta += 2U;
   }
   return res - 1U;
}


uint32_t Utl_Sqrt32(uint32_t x)
{
   uint32_t res = 1UL;
   uint32_t sqr = 1UL;
   uint32_t delta = 3UL;

   while (sqr <= x) {
      res += 1UL;
      sqr += delta;
      delta += 2UL;
   }
   return res - 1UL;
}


uint16_t Utl_StrLen(const uint8_t * src)
{
    uint16_t len = (uint16_t)0x0000;

    while (*(src++)) {
        len++;
    }

    return len;
}


void Utl_StrCat(uint8_t * dst, const uint8_t * src)
{
    ASSERT(dst != (void *)NULL);

    while (*(dst++)) {
    }

    --dst;

    while ((*(dst++) = *(src++))) {
    }
}


void Utl_StrCpy(uint8_t * dst, const uint8_t * src)
{
    ASSERT(dst != (void *)NULL);

    while ((*(dst++) = *(src++))) {
    }
}


uint16_t Utl_Ntohs(uint16_t value)
{
#if KNX_ENDIANESS == KNX_LITTLE_ENDIAN
    return makeWord(loByte(value), hiByte(value));
#elif KNX_ENDIANESS == KNX_BIG_ENDIAN
    return value;
#endif
}

uint16_t Utl_Htons(uint16_t value)
{
#if KNX_ENDIANESS == KNX_LITTLE_ENDIAN
    return makeWord(loByte(value), hiByte(value));
#elif KNX_ENDIANESS == KNX_BIG_ENDIAN
    return value;
#endif
}

#if 0
void Utl_StrNCpy(uint8_t * RESTRICT dst, const uint8_t * RESTRICT src, SizeType maxlen)
{
    ASSERT(dest != (void *)NULL);

    while ((*(dst++) = *(src++)) && (maxlen--)) {
    }
}


#endif

void Utl_StrRev(uint8_t * str)
{
    uint16_t    idx;
    uint16_t    dpos;
    uint8_t       ch;

    ASSERT(str != (void *)NULL);

    if (Utl_StrLen(str) < (uint16_t)2U) {
        return;
    }

    for (idx = (uint16_t)0, dpos = Utl_StrLen(str) - 1; dpos > idx; idx++, dpos--) {
        ch         = dpos[str];
        dpos[str]  = idx[str];
        idx[str]   = ch;
    }
}


const uint8_t * Utl_StrChr(const uint8_t * str, uint8_t ch)
{
    ASSERT(str != (void *)NULL);

    while (*str) {
        if (*(str++) == ch) {
            return str;
        }
    }

    return (const uint8_t *)NULL;
}


void Utl_Itoa(int32_t value, uint8_t base, uint8_t * buf)
{
    uint32_t mod;
    uint32_t div;
    uint8_t pos = (uint8_t)0x00;

    ASSERT(buf != (void *)NULL);

    if (((int32_t)value) < 0L && base == (uint8_t)10) {
        value      = (uint32_t)((int32_t)value * -1L);
        buf[0]     = '-';
        pos        = 1;
    }

    if (value == 0L) {
        buf[pos++] = '0';
        return;
    }
    div = (uint32_t)value;
    while (div) {
        mod = div % base;

        if (mod < 10) {
            buf[pos++] = '0' + mod;
        } else {
            buf[pos++] = 'A' + mod - (uint8_t)10;
        }

        div /= base;
    }

    buf[pos--] = '\0';
    Utl_StrRev(buf);
}


#if 0
void const * Utl_BinSearch(void const * key, void const * base, uint16_t num_elems, uint16_t elem_size,
                           Utl_CompareFuncType compare_func)
{
    uint16_t  mid;
    sint16  left, right;
    uint8_t * cmp_key;
    sint8   res;

    left   = (sint16)0x0000;
    right  = (sint16)(num_elems - 1);

    do {
        mid        = (uint16_t)(left + right) >> 1;
        cmp_key    = (uint8_t *)((uint8_t *)base + (mid * elem_size));
        res        = compare_func(key, cmp_key);

        if (res == (sint16)0) {
            return cmp_key;
        } else if (res < (uint8_t)0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    } while (left <= right);

    return (void const *)NULL;
}


#endif

bool Utl_FloatsAreNearlyEqual(float32 lhs, float32 rhs, int32_t max_difference)
{
    int32_t  lhs_int    = *(int32_t *)&lhs;
    int32_t  rhs_int    = *(int32_t *)&rhs;
    int32_t  int_difference;

    if (lhs_int < (int32_t)0) {
        lhs_int = (int32_t)(0x80000000 - lhs_int);
    }

    if (rhs_int < (int32_t)0) {
        rhs_int = (int32_t)(0x80000000 - rhs_int);
    }

    int_difference = ABS(lhs_int - rhs_int);

    return (int_difference <= max_difference) ? true : false;
}


bool Utl_IsNull(void * Ptr)
{
    return (Ptr == (void *)NULL) ? true : false;
}


uint16_t Utl_Swap16(uint16_t * w)
{
    return makeWord(loByte(*w), hiByte(*w));
}


uint32_t Utl_Swap32(uint32_t * dw)
{
    return makeWord(makeWord(loByte(loWord(*dw)), hiByte(loWord(*dw))),
        makeWord(loByte(hiWord(*dw)), hiByte(hiWord(*dw)))
    );
}

} // namespace knx

