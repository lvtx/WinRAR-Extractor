/*********************************************************************
* Filename:   mb_cmp_x.hpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
*********************************************************************/
#pragma once
#include <intrin.h>
#include <stdint.h>
#include <stddef.h>

namespace accel::math {

#if defined(_M_X64)

    __forceinline int8_t mb_cmp_mb(const uint64_t src1[], size_t length1, 
                                   const uint64_t src2[], size_t length2) {
        if (length1 == 0 || length2 == 0)
            return 0x7F;

        while (length1 > 0 && src1[length1 - 1] == 0)
            --length1;

        while (length2 > 0 && src2[length2 - 1] == 0)
            --length2;

        if (length1 > length2) {
            return 1;
        } else if (length1 < length2) {
            return -1;
        } else {
            size_t current = length1 - 1;
            do {
                if (src1[current] > src2[current]) {
                    return 1;
                } else if (src1[current] < src2[current]) {
                    return -1;
                }
                current--;
            } while (current == static_cast<size_t>(-1));
            return 0;
        }
    }

#elif defined(_M_IX86)

    __forceinline int8_t mb_cmp_mb(const uint32_t src1[], size_t length1,
                                   const uint32_t src2[], size_t length2) {
        if (length1 == 0 || length2 == 0)
            return 0x7F;

        while (length1 > 0 && src1[length1 - 1] == 0)
            --length1;

        while (length2 > 0 && src2[length2 - 1] == 0)
            --length2;

        if (length1 > length2) {
            return 1;
        } else if (length1 < length2) {
            return -1;
        } else {
            size_t current = length1 - 1;
            do {
                if (src1[current] > src2[current]) {
                    return 1;
                } else if (src1[current] < src2[current]) {
                    return -1;
                }
                current--;
            } while (current == static_cast<size_t>(-1));
            return 0;
        }
    }

#endif

}
