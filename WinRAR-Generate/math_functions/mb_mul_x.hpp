/*********************************************************************
* Filename:   mb_mul_x.hpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
*********************************************************************/
#pragma once
#include <intrin.h>
#include <stdint.h>
#include <stddef.h>
#include <memory.h>

// namespace accel::algorithm::math {
// 
// #if defined(_M_X64)
//     extern void MultiplyByKaratsuba(const uint64_t src1[], const uint64_t src2[], size_t srclength,
//                                     uint64_t dest[]);
// #elif defined(_M_IX86)
//     extern void MultiplyByKaratsuba(const uint32_t src1[], const uint32_t src2[], size_t srclength,
//                                     uint32_t dest[]);
// #endif
// 
// }

namespace accel::math {

#if defined(_M_X64)

    __forceinline uint64_t mb_mul_u64(uint64_t multiplier[], size_t length,
                                      uint64_t multiplicand) {
        if (length == 0)
            return 0;

        uint64_t temp[2];
        uint64_t carry = 0;
        for (size_t i = 0; i < length; ++i) {
            temp[0] = _umul128(multiplier[i], multiplicand, temp + 1);
            _addcarry_u64(_addcarry_u64(0, temp[0], carry, multiplier + i), temp[1], 0, &carry);
        }

        return carry;
    }

    __forceinline uint64_t mb_mul_u64(const uint64_t multiplier[], size_t length1,
                                      uint64_t multiplicand, 
                                      uint64_t product[], size_t length2) {
        if (length1 == 0 || length2 == 0 || length2 < length1)
            return 0;

        uint64_t temp[2];
        uint64_t carry = 0;
        for (size_t i = 0; i < length1; ++i) {
            temp[0] = _umul128(multiplier[i], multiplicand, temp + 1);
            _addcarry_u64(_addcarry_u64(0, temp[0], carry, product + i), temp[1], 0, &carry);
        }

        if (length2 > length1) {
            product[length1] = carry;
            memset(product + length1 + 1, 0, (length2 - length1 - 1) * sizeof(uint64_t));
            return 0;
        } else {
            return carry;
        }
    }

    __forceinline bool mb_mul_mb(const uint64_t multiplier[], size_t length1,
                                 const uint64_t multiplicand[], size_t length2,
                                 uint64_t product[], size_t length3) {
        if (length1 == 0 || length2 == 0 || length3 == 0)
            return false;

        while (length1 > 0 && multiplier[length1 - 1] == 0)
            --length1;

        while (length2 > 0 && multiplicand[length2 - 1] == 0)
            --length2;

        if (length1 + length2 > length3)
            return false;

        memset(product, 0, length3 * sizeof(uint64_t));

        uint64_t temp[2];
        for (size_t i = 0; i < length1; ++i) {
            for (size_t j = 0; j < length2; ++j) {
                temp[0] = _umul128(multiplicand[j], multiplier[i], temp + 1);
                if (_addcarry_u64(_addcarry_u64(0, product[i + j], temp[0], product + i + j),
                                  product[i + j + 1],
                                  temp[1],
                                  product + i + j + 1)) {
                    uint64_t* block = product + i + j + 2;
                    while (_addcarry_u64(1, *block, 0, block))
                        ++block;
                }
            }
        }
        return true;
    }

//     __forceinline bool mb_mul_mb_Karatsuba(const uint64_t multiplier[], const uint64_t multiplicand[], size_t length1,
//                                            uint64_t product[], size_t length2) {
//         if (length1 == 0 || length2 == 0)
//             return false;
// 
//         while (length1 > 0 && multiplier[length1 - 1] == 0 && multiplicand[length1 - 1] == 0)
//             --length1;
// 
//         if (length1 * 2 > length2)
//             return false;
// 
//         accel::algorithm::math::MultiplyByKaratsuba(multiplier, multiplicand, length1, product);
//         return true;
//     }

#elif defined(_M_IX86)

    __forceinline uint32_t mb_mul_u32(uint32_t multiplier[], size_t length,
                                      uint32_t multiplicand) {
        if (length == 0)
            return 0;

        uint64_t temp;
        uint32_t carry = 0;
        for (size_t i = 0; i < length; ++i) {
            temp = static_cast<uint64_t>(multiplier[i]) * static_cast<uint64_t>(multiplicand);
            _addcarry_u32(_addcarry_u32(0, reinterpret_cast<uint32_t*>(&temp)[0], carry, multiplier + i), 
                          reinterpret_cast<uint32_t*>(&temp)[1],
                          0, 
                          &carry);
        }

        return carry;
    }

    __forceinline uint32_t mb_mul_u32(const uint32_t multiplier[], size_t length1, 
                                      uint32_t multiplicand, 
                                      uint32_t product[], size_t length2) {
        if (length1 == 0 || length2 == 0 || length2 < length1)
            return 0;

        uint64_t temp;
        uint32_t carry = 0;
        for (size_t i = 0; i < length1; ++i) {
            temp = static_cast<uint64_t>(multiplier[i]) * static_cast<uint64_t>(multiplicand);
            _addcarry_u32(_addcarry_u32(0, reinterpret_cast<uint32_t*>(&temp)[0], carry, product + i),
                          reinterpret_cast<uint32_t*>(&temp)[1],
                          0,
                          &carry);
        }

        if (length2 > length1) {
            product[length1] = carry;
            memset(product + length1 + 1, 0, (length2 - length1 - 1) * sizeof(uint32_t));
            return 0;
        } else {
            return carry;
        }
    }

    __forceinline bool mb_mul_mb(uint32_t multiplier[], size_t length1,
                                 uint32_t multiplicand[], size_t length2,
                                 uint32_t product[], size_t length3) {
        if (length1 == 0 || length2 == 0 || length3 == 0)
            return false;

        while (length1 > 0 && multiplier[length1 - 1] == 0)
            --length1;

        while (length2 > 0 && multiplicand[length2 - 1] == 0)
            --length2;

        if (length1 + length2 > length3)
            return false;

        memset(product, 0, length3 * sizeof(uint32_t));

        uint64_t temp;
        for (size_t i = 0; i < length1; ++i) {
            for (size_t j = 0; j < length2; ++j) {
                temp = static_cast<uint64_t>(multiplicand[j]) * static_cast<uint64_t>(multiplier[i]);
                if (_addcarry_u32(_addcarry_u32(0, product[i + j], reinterpret_cast<uint32_t*>(&temp)[0], product + i + j),
                                  product[i + j + 1],
                                  reinterpret_cast<uint32_t*>(&temp)[1],
                                  product + i + j + 1)) {
                    uint32_t* block = product + i + j + 2;
                    while (_addcarry_u32(1, *block, 0, block))
                        ++block;
                }
            }
        }
        return true;
    }

//     __forceinline bool mb_mul_mb_Karatsuba(const uint32_t multiplier[], const uint32_t multiplicand[], size_t length1,
//                                            uint32_t product[], size_t length2) {
//         if (length1 == 0 || length2 == 0)
//             return false;
// 
//         if (length1 * 2 > length2)
//             return false;
// 
//         accel::algorithm::math::MultiplyByKaratsuba(multiplier, multiplicand, length1, product);
//         return true;
//     }

#endif

}
