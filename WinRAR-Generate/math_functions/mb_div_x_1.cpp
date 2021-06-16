/*********************************************************************
* Filename:   mb_div_x.cpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
*********************************************************************/
#include "mb_div_x.hpp"
#include "mb_add_x.hpp"
#include "mb_sub_x.hpp"
#include "mb_mul_x.hpp"
#include "mb_cmp_x.hpp"

namespace accel::math {

#if defined(_M_X64)

    bool mb_div_mb(uint64_t dividend[], size_t length1,
                   const uint64_t divisor[], size_t length2,
                   uint64_t quotient[], size_t length3) {
        if (length1 == 0 || length2 == 0 || length3 == 0)
            return false;

        while (length1 > 0 && dividend[length1 - 1] == 0)
            --length1;

        while (length2 > 0 && divisor[length2 - 1] == 0)
            --length2;

        if (length1 - length2 + 1 > length3)
            return false;

        if (length2 < 2) {
            memcpy_s(quotient, length3 * sizeof(uint64_t), dividend, length1 * sizeof(uint64_t));
            memset(dividend, 0, sizeof(uint64_t));
            dividend[0] = mb_div_u64(dividend, length1, divisor[0]);
            return true;
        }

        if (length1 == 0 || mb_cmp_mb(dividend, length1, 
                                      divisor, length2) == -1) {
            memset(quotient, 0, length3 * sizeof(uint64_t));
            return true;
        }

        uint64_t* buffer0 = new uint64_t[length1 - length2 + 1 + length2 + 1]();
        uint64_t* buffer1 = buffer0 + length1 - length2 + 1;
        size_t block_start = length1 - length2;
        size_t block_end = length1;

        while (block_start != static_cast<size_t>(-1)) {
            if (mb_cmp_mb(dividend + block_start, block_end - block_start, 
                          divisor, length2) == -1) {
                --block_start;
                continue;
            }

            uint8_t borrow;
            uint64_t temp_q[2];
            if (block_end - block_start == length2) {
                u128_div_u128(reinterpret_cast<const uint64_t (&)[2]>(dividend[block_end - 2]),
                              reinterpret_cast<const uint64_t (&)[2]>(divisor[length2 - 2]), 
                              temp_q);

                mb_mul_u64(divisor, length2, 
                           temp_q[0],
                           buffer1, length2 + 1);

                while (buffer1[length2] != 0) {
                    mb_sub_mb(buffer1, length2 + 1, 
                              divisor, length2, 
                              0);
                    --temp_q[0];
                }

                borrow = mb_sub_mb(dividend + block_start, block_end - block_start,
                                   buffer1, length2,
                                   0);
                
            } else {
                temp_q[0] = _udiv128(dividend[block_end - 2], dividend[block_end - 1],
                                     divisor[length2 - 1],
                                     nullptr);

                mb_mul_u64(divisor, length2, 
                           temp_q[0],
                           buffer1, length2 + 1);

                borrow = mb_sub_mb(dividend + block_start, block_end - block_start,
                                   buffer1, length2 + 1,
                                   0);
                
            }

            while (borrow) {
                borrow -= mb_add_mb(dividend + block_start, block_end - block_start,
                                    divisor, length2,
                                    0);
                --temp_q[0];
            }

            buffer0[block_start] = temp_q[0];

            while (block_end > block_start && dividend[block_end - 1] == 0)
                --block_end;

            --block_start;
        }

        memcpy_s(quotient, length3 * sizeof(uint64_t), buffer0, (length1 - length2 + 1) * sizeof(uint64_t));
        if (length1 - length2 + 1 < length3)
            memset(quotient + length1 - length2 + 1, 0, (length3 - length1 + length2 - 1) * sizeof(uint64_t));

        delete[] buffer0;
        return true;
    }

#elif defined(_M_IX86)

    bool mb_div_mb(uint32_t dividend[], size_t length1,
                   const uint32_t divisor[], size_t length2,
                   uint32_t quotient[], size_t length3) {
        if (length1 == 0 || length2 == 0 || length3 == 0)
            return false;

        while (length1 > 0 && dividend[length1 - 1] == 0)
            --length1;

        while (length2 > 0 && divisor[length2 - 1] == 0)
            --length2;

        if (length1 - length2 + 1 > length3)
            return false;

        if (length2 < 2) {
            memcpy_s(quotient, length3 * sizeof(uint32_t), dividend, length1 * sizeof(uint32_t));
            memset(dividend, 0, sizeof(uint32_t));
            dividend[0] = mb_div_u32(dividend, length1, divisor[0]);
            return true;
        }

        if (length1 == 0 || mb_cmp_mb(dividend, length1,
                                      divisor, length2) == -1) {
            memset(quotient, 0, length3 * sizeof(uint32_t));
            return true;
        }

        uint32_t* buffer0 = new uint32_t[length1 - length2 + 1 + length2 + 1]();
        uint32_t* buffer1 = buffer0 + length1 - length2 + 1;
        size_t block_start = length1 - length2;
        size_t block_end = length1;

        while (block_start != static_cast<size_t>(-1)) {
            if (mb_cmp_mb(dividend + block_start, block_end - block_start,
                          divisor, length2) == -1) {
                --block_start;
                continue;
            }

            uint8_t borrow;
            if (block_end - block_start == length2) {
                buffer0[block_start] = static_cast<uint32_t>(
                    *reinterpret_cast<uint64_t*>(dividend + block_end - 2) / 
                    *reinterpret_cast<const uint64_t*>(divisor + length2 - 2)
                    );

                mb_mul_u32(divisor, length2,
                           buffer0[block_start],
                           buffer1, length2 + 1);

                while (buffer1[length2] != 0) {
                    mb_sub_mb(buffer1, length2 + 1,
                              divisor, length2,
                              0);
                    --buffer0[block_start];
                }

                borrow = mb_sub_mb(dividend + block_start, block_end - block_start,
                                   buffer1, length2,
                                   0);
            } else {
                buffer0[block_start]  = static_cast<uint32_t>(
                    *reinterpret_cast<uint64_t*>(dividend + block_end - 2) / 
                    divisor[length2 - 1]
                    );

                mb_mul_u32(divisor, length2,
                           buffer0[block_start],
                           buffer1, length2 + 1);

                borrow = mb_sub_mb(dividend + block_start, block_end - block_start,
                                   buffer1, length2 + 1,
                                   0);
                
            }

            while (borrow) {
                borrow -= mb_add_mb(dividend + block_start, block_end - block_start,
                                    divisor, length2,
                                    0);
                --buffer0[block_start];
            }

            while (block_end > block_start && dividend[block_end - 1] == 0)
                --block_end;

            --block_start;
        }

        memcpy_s(quotient, length3 * sizeof(uint32_t), buffer0, (length1 - length2 + 1) * sizeof(uint32_t));
        if (length1 - length2 + 1 < length3)
            memset(quotient + length1 - length2 + 1, 0, (length3 - length1 + length2 - 1) * sizeof(uint32_t));

        delete[] buffer0;
        return true;
    }

#endif

}
