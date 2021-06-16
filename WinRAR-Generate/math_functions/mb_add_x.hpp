/*********************************************************************
* Filename:   mb_add_x.hpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
*********************************************************************/
#pragma once
#include <intrin.h>
#include <stdint.h>
#include <stddef.h>

namespace accel::math {

#if defined(_M_X64)

    __forceinline uint8_t mb_add_u64(uint64_t adder1[], size_t length,
                                     uint64_t adder2, uint8_t carry) {
        if (length == 0)
            return 0xFF;

        carry = _addcarry_u64(carry, adder1[0], adder2, adder1);
        for (size_t i = 1; carry != 0 && i < length; ++i)
            carry = _addcarry_u64(carry, adder1[i], 0, adder1 + i);

        return carry;
    }

    __forceinline uint8_t mb_add_mb(uint64_t adder1[], size_t length1,
                                    const uint64_t adder2[], size_t length2, uint8_t carry) {
        if (length1 < length2 || length2 == 0)
            return 0xFF;

        for (size_t i = 0; i < length2; ++i)
            carry = _addcarry_u64(carry, adder1[i], adder2[i], adder1 + i);
        for (size_t i = length2; carry != 0 && i < length1; ++i)
            carry = _addcarry_u64(carry, adder1[i], 0, adder1 + i);

        return carry;
    }

    __forceinline uint8_t mb_add_mb(const uint64_t adder1[], size_t length1,
                                    const uint64_t adder2[], size_t length2,
                                    uint64_t result[], size_t length3, uint8_t carry) {
        if (length1 == 0 || length2 == 0 || length3 == 0)
            return 0xFF;

        if (length1 < length2) {
            const uint64_t* temp_ptr = adder2;
            size_t temp_len = length2;
            adder2 = adder1;
            length2 = length1;
            adder1 = temp_ptr;
            length1 = length2;
        }

        if (length3 < length1) 
            return 0xFF;

        for (size_t i = 0; i < length2; ++i)
            carry = _addcarry_u64(carry, adder1[i], adder2[i], result + i);

        for (size_t i = length2; carry != 0 && i < length1; ++i)
            carry = _addcarry_u64(carry, adder1[i], 0, result + i);

        for (size_t i = length1; carry != 0 && i < length3; ++i)
            carry = _addcarry_u64(carry, 0, 0, result + i);

        return carry;
    }

#elif defined(_M_IX86)

    __forceinline uint8_t mb_add_u32(uint32_t adder1[], size_t length,
                                     uint32_t adder2, uint8_t carry) {
        if (length == 0)
            return 0xFF;

        carry = _addcarry_u32(carry, adder1[0], adder2, adder1);
        for (size_t i = 1; carry != 0 && i < length; ++i)
            carry = _addcarry_u32(carry, adder1[i], 0, adder1 + i);

        return carry;
    }

    __forceinline uint8_t mb_add_mb(uint32_t adder1[], size_t length1,
                                    const uint32_t adder2[], size_t length2, uint8_t carry) {
        if (length1 < length2 || length2 == 0)
            return 0xFF;

        for (size_t i = 0; i < length2; ++i)
            carry = _addcarry_u32(carry, adder1[i], adder2[i], adder1 + i);

        for (size_t i = length2; carry != 0 && i < length1; ++i)
            carry = _addcarry_u32(carry, adder1[i], 0, adder1 + i);

        return carry;
    }

    __forceinline uint8_t mb_add_u64(uint32_t adder1[], size_t length,
                                     uint64_t adder2, uint8_t carry) {
        return mb_add_mb(adder1, length, reinterpret_cast<uint32_t*>(&adder2), 2, carry);
        if (length < 2)
            return 0xFF;

        carry = _addcarry_u32(carry, adder1[0], reinterpret_cast<uint32_t*>(&adder2)[0], adder1);
        carry = _addcarry_u32(carry, adder1[1], reinterpret_cast<uint32_t*>(&adder2)[1], adder1 + 1);
        for (size_t i = 2; carry != 0 && i < length; ++i)
            carry = _addcarry_u32(carry, adder1[i], 0, adder1 + i);

        return carry;
    }

    __forceinline uint8_t mb_add_mb(const uint32_t adder1[], size_t length1,
                                    const uint32_t adder2[], size_t length2,
                                    uint32_t result[], size_t length3, uint8_t carry) {
        if (length1 == 0 || length2 == 0 || length3 == 0)
            return 0xFF;

        if (length1 < length2) {
            const uint32_t* temp_ptr = adder2;
            size_t temp_len = length2;
            adder2 = adder1;
            length2 = length1;
            adder1 = temp_ptr;
            length1 = length2;
        }

        if (length3 < length1)
            return 0xFF;

        for (size_t i = 0; i < length2; ++i)
            carry = _addcarry_u32(carry, adder1[i], adder2[i], result + i);

        for (size_t i = length2; carry != 0 && i < length1; ++i)
            carry = _addcarry_u32(carry, adder1[i], 0, result + i);

        for (size_t i = length1; carry != 0 && i < length3; ++i)
            carry = _addcarry_u32(carry, 0, 0, result + i);

        return carry;
    }

#endif

}
