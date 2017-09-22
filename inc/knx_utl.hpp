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
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#if !defined(__KNX_UTL_HPP)
#define __KNX_UTL_HPP

#include "knx_types.hpp"
#include "knx_macros.hpp"

namespace knx {

/*
**  Global Types.
*/
enum class Utl_EndianessType {
    BIG,
    LITTLE
};

struct Utl_DivremType {
    uint16_t div;
    uint16_t rem;
};

/*
**  Global Constants.
*/
extern const uint8_t  Utl_SetBitTab8[];
extern const uint8_t  Utl_ClearBitTab8[];
extern const uint16_t Utl_SetBitTab16[];
extern const uint16_t Utl_ClearBitTab16[];

/*
**  Global Functions.
*/
Utl_EndianessType Utl_CheckEndianess(void);

bool Utl_BitGet(uint16_t w, uint8_t num);
uint16_t Utl_BitSet(uint16_t w, uint8_t num);
uint16_t Utl_BitReset(uint16_t w, uint8_t num);
uint16_t Utl_BitToggle(uint16_t w, uint8_t num);
uint16_t Utl_BitGetHighest(uint16_t w);
uint16_t Utl_BitGetLowest(uint16_t w);
uint16_t Utl_BitSetLowest(uint16_t w);
uint16_t Utl_BitResetLowest(uint16_t w);
uint8_t Utl_Log2(uint16_t num);
uint16_t Utl_Sqrt16(uint16_t x);
uint32_t Utl_Sqrt32(uint32_t x);

void Utl_Itoa(int32_t value, uint8_t base, uint8_t * buf);
void Utl_MemCopy(void *  dst, void * src, uint16_t len);
void Utl_MemSet(void * dest, uint8_t fill_char, uint16_t len);
uint16_t Utl_StrLen(const uint8_t * src);
void Utl_StrCat(/*@out@*/ uint8_t * dst, /*@in@*/ const uint8_t * src);
void Utl_StrCpy(/*@out@*/ uint8_t * dst, /*@in@*/ const uint8_t * src);
void Utl_StrNCpy(/*@out@*/ uint8_t * dst, /*@in@*/ const uint8_t * src, uint16_t maxlen);
void Utl_StrRev(/*@in@*//*@out@*/ uint8_t * str);
const uint8_t * Utl_StrChr(/*@in@*/ const uint8_t * str, uint8_t ch);

uint16_t Utl_Ntohs(uint16_t value);
uint16_t Utl_Htons(uint16_t value);


/* void const * Utl_BinSearch(void const * key,void const * base,uint16_t num_elems,uint16_t elem_size,Utl_CompareFuncType compare_func); */
void Utl_Divrem(uint16_t dividend, uint16_t divisor, /*@out@*/ Utl_DivremType * res);
bool Utl_FloatsAreNearlyEqual(float32 lhs, float32 rhs, int32_t max_difference);
void Utl_Randomize(uint16_t seed);
uint16_t Utl_Random(void);
#if 0
void    Utl_LongJump(Utl_JumpBufType * buf, sint16 val);
uint16_t  Utl_SetJump(Utl_JumpBufType * buf);
#endif

/*@nullwhentrue@*/ bool Utl_IsNull(void * Ptr);
uint16_t Utl_Swap16(uint16_t * w);
uint32_t Utl_Swap32(uint32_t * dw);


/*
**  Global Function-like Macros.
*/
#define UTL_BIT_GET8(value, bit)        ((( (value) & Utl_SetBitTab8[(bit)]) != (uint8_t)0x00) ? Std_High : Std_Low)
#define UTL_BIT_SET8(value, bit)        ((value)  |= Utl_SetBitTab8[(bit)])
#define UTL_BIT_RESET8(value, bit)      ((value)  &= Utl_ClearBitTab8[(bit)])
#define UTL_BIT_TOGGLE8(value, bit)     ((value)  ^= Utl_SetBitTab8[(bit)])

#define UTL_BIT_GET16(value, bit)       ((( (value) & Utl_SetBitTab16[(bit)]) != (uint16_t)0x0000) ? Std_High : Std_Low)
#define UTL_BIT_SET16(value, bit)       ((value)  |= Utl_SetBitTab16[(bit)])
#define UTL_BIT_RESET16(value, bit)     ((value)  &= Utl_ClearBitTab16[(bit)])
#define UTL_BIT_TOGGLE16(value, bit)    ((value)  ^= Utl_SetBitTab16[(bit)])

#define ASSERT_IS_NOT_NULL(ptr)         ASSERT(!Utl_IsNull((ptr)))
#if 0
#define ASSERT_IS_NOT_NULL(ptr)   \
    _BEGIN_BLOCK                  \
    if (Utl_IsNull((ptr))) {      \
        CPU_SOFTWARE_INTERRUPT(); \
        return;                   \
    }                             \
    _END_BLOCK
#endif

#define Utl_PlainCharIsSigned()         ((int)((char)0x80) < 0)

template<typename T>
inline T twosComplement(T value)
{
    return (~value) + 1;
}

inline uint8_t loByte(uint16_t value)
{
    return (uint8_t)((uint16_t)((uint16_t)value & 0x00ffU));
}

inline uint8_t hiByte(uint16_t value)
{
    return (uint8_t)((uint16_t)(((uint16_t)value >> 8) & 0x00ffU));
}

inline uint16_t loWord(uint32_t value)
{
    return (uint16_t)((uint32_t)((uint32_t)value & 0xffffU));
}

inline uint16_t hiWord(uint32_t value)
{
    return (uint16_t)((uint32_t)(((uint32_t)value >> 16) & 0xffffU));
}

inline uint16_t makeWord(uint8_t h, uint8_t l)
{
    return (((uint32_t)(h & ((uint16_t)0xffffu))) << (uint32_t)16) | ((uint32_t)(l & ((uint16_t)0xffffu)));
}

inline uint8_t invertNibble(uint8_t value)
{
    return (uint8_t)(((uint8_t) ~value & ((uint8_t)0x0f)));
}


} // namepace knx

#endif  /* __KNX_UTL_HPP */

