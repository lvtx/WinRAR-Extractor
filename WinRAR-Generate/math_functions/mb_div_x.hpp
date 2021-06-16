/*********************************************************************
* Filename:   mb_div_x.hpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
*********************************************************************/
#pragma once
#include <intrin.h>
#include <stdint.h>
#include <stddef.h>

namespace accel::math {

#if defined(_M_X64)

    uint64_t _udiv128(uint64_t dividend_l, uint64_t dividend_h, uint64_t divisor, 
                      uint64_t* remainder);

    uint64_t u128_div_u64(const uint64_t (&dividend)[2], uint64_t divisor);

    uint64_t u128_div_u64(const uint64_t (&dividend)[2], uint64_t divisor, 
                          uint64_t (&quotient)[2]);

    void u128_div_u128(const uint64_t (&dividend)[2], const uint64_t (&divisor)[2],
                       uint64_t (&quotient)[2], uint64_t (&remainder)[2]);

    void u128_div_u128(const uint64_t(&dividend)[2], const uint64_t(&divisor)[2],
                       uint64_t (&quotient)[2]);

    uint64_t mb_div_u64(uint64_t dividend[], size_t length,
                        uint64_t divisor);

    bool mb_div_mb(uint64_t dividend[], size_t length1,
                   const uint64_t divisor[], size_t length2,
                   uint64_t quotient[], size_t length3);

#elif defined(_M_IX86)

    uint32_t __fastcall mb_div_u32(uint32_t src[], size_t length,
                                   uint32_t divisor);

    bool mb_div_mb(uint32_t dividend[], size_t length1,
                   const uint32_t divisor[], size_t length2,
                   uint32_t quotient[], size_t length3);

#endif

}
