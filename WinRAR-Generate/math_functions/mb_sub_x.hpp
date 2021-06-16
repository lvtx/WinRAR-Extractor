/*********************************************************************
* Filename:   mb_sub_x.hpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
*********************************************************************/
#pragma once
#include <intrin.h>
#include <stdint.h>
#include <stddef.h>

namespace accel::math {

#if defined(_M_X64)

    __forceinline uint8_t mb_sub_u64(uint64_t minuend[], size_t length,
                                     uint64_t subtrahend, uint8_t borrow) {
        if (length == 0)
            return 0xFF;

        borrow = _subborrow_u64(borrow, minuend[0], subtrahend, minuend);
        for (size_t i = 1; borrow != 0 && i < length; ++i)
            borrow = _subborrow_u64(borrow, minuend[i], 0, minuend + i);

        return borrow;
    }

    __forceinline uint8_t mb_sub_mb(uint64_t minuend[], size_t length1,
                                    const uint64_t subtrahend[], size_t length2, uint8_t borrow) {
        if (length1 < length2 || length2 == 0)
            return 0xFF;

        for (size_t i = 0; i < length2; ++i)
            borrow = _subborrow_u64(borrow, minuend[i], subtrahend[i], minuend + i);

        for (size_t i = length2; borrow != 0 && i < length1; ++i)
            borrow = _subborrow_u64(borrow, minuend[i], 0, minuend + i);

        return borrow;
    }

    __forceinline uint8_t mb_sub_mb(const uint64_t minuend[], size_t length1,
                                    const uint64_t subtrahend[], size_t length2,
                                    uint64_t result[], size_t length3, uint8_t borrow) {
        if (length1 == 0 || length2 == 0 || length3 == 0)
            return 0xFF;

        if (length1 < length2) {
            const uint64_t* temp_ptr = subtrahend;
            size_t temp_len = length2;
            subtrahend = minuend;
            length2 = length1;
            minuend = temp_ptr;
            length1 = length2;
        }

        if (length3 < length1)
            return 0xFF;

        for (size_t i = 0; i < length2; ++i)
            borrow = _subborrow_u64(borrow, minuend[i], subtrahend[i], result + i);

        for (size_t i = length2; borrow != 0 && i < length1; ++i)
            borrow = _subborrow_u64(borrow, minuend[i], 0, result + i);

        for (size_t i = length1; borrow != 0 && i < length3; ++i)
            borrow = _subborrow_u64(borrow, 0, 0, result + i);

        return borrow;
    }

#elif defined(_M_IX86)

    __forceinline uint8_t mb_sub_u32(uint32_t minuend[], size_t length,
                                     uint32_t subtrahend, uint8_t borrow) {
        if (length == 0)
            return 0xFF;

        borrow = _subborrow_u32(borrow, minuend[0], subtrahend, minuend);
        for (size_t i = 1; borrow != 0 && i < length; ++i)
            borrow = _subborrow_u32(borrow, minuend[i], 0, minuend + i);

        return borrow;
    }

    __forceinline uint8_t mb_sub_mb(uint32_t minuend[], size_t length1,
                                    const uint32_t subtrahend[], size_t length2, uint8_t borrow) {
        if (length1 < length2 || length2 == 0)
            return 0xFF;

        for (size_t i = 0; i < length2; ++i)
            borrow = _subborrow_u32(borrow, minuend[i], subtrahend[i], minuend + i);

        for (size_t i = length2; borrow != 0 && i < length1; ++i)
            borrow = _subborrow_u32(borrow, minuend[i], subtrahend[i], minuend + i);

        return borrow;
    }

    __forceinline uint8_t mb_sub_u64(uint32_t minuend[], size_t length,
                                     uint64_t subtrahend, uint8_t borrow) {
        if (length < 2)
            return 0xFF;

        borrow = _subborrow_u32(borrow, minuend[0], reinterpret_cast<uint32_t*>(&subtrahend)[0], minuend);
        borrow = _subborrow_u32(borrow, minuend[1], reinterpret_cast<uint32_t*>(&subtrahend)[1], minuend + 1);
        for (size_t i = 2; borrow != 0 && i < length; ++i)
            borrow = _subborrow_u32(borrow, minuend[i], 0, minuend + i);

        return borrow;
    }

    __forceinline uint8_t mb_sub_mb(const uint32_t minuend[], size_t length1,
                                    const uint32_t subtrahend[], size_t length2,
                                    uint32_t result[], size_t length3, uint8_t borrow) {
        if (length1 == 0 || length2 == 0 || length3 == 0)
            return 0xFF;

        if (length1 < length2) {
            const uint32_t* temp_ptr = subtrahend;
            size_t temp_len = length2;
            subtrahend = minuend;
            length2 = length1;
            minuend = temp_ptr;
            length1 = length2;
        }

        if (length3 < length1)
            return 0xFF;

        for (size_t i = 0; i < length2; ++i)
            borrow = _subborrow_u32(borrow, minuend[i], subtrahend[i], result + i);

        for (size_t i = length2; borrow != 0 && i < length1; ++i)
            borrow = _subborrow_u32(borrow, minuend[i], 0, result + i);

        for (size_t i = length1; borrow != 0 && i < length3; ++i)
            borrow = _subborrow_u32(borrow, 0, 0, result + i);

        return borrow;
    }

#endif

}
