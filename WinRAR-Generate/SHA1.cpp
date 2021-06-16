/*********************************************************************
* Filename:   SHA1.cpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
*********************************************************************/
#include "SHA1.hpp"
#include <intrin.h>

namespace accel::algorithm::hash::SHA1 {

    void Initialize(uint32_t hashResult[5]) {
        hashResult[0] = 0x67452301;
        hashResult[1] = 0xEFCDAB89;
        hashResult[2] = 0x98BADCFE;
        hashResult[3] = 0x10325476;
        hashResult[4] = 0xC3D2E1F0;
    }

    void MainCycle(const uint8_t srcBytes[], size_t srcBytesLength, uint32_t hashResult[5]) {
        uint32_t Buffer[80] = { };
        uint32_t a = 0, b = 0, c = 0, d = 0, e = 0;
        const uint8_t(*MessageBlock)[64] = reinterpret_cast<const uint8_t(*)[64]>(srcBytes);

        size_t RoundsOfMainCycle = srcBytesLength / 64;
        for (size_t i = 0; i < RoundsOfMainCycle; ++i) {
            for (int j = 0; j < 64; j += 4)
                Buffer[j / 4] = static_cast<uint32_t>(MessageBlock[i][j + 3])       |
                                static_cast<uint32_t>(MessageBlock[i][j + 2]) << 8  |
                                static_cast<uint32_t>(MessageBlock[i][j + 1]) << 16 |
                                static_cast<uint32_t>(MessageBlock[i][j]) << 24;

            for (int j = 16; j < 80; ++j) {
                uint32_t temp = Buffer[j - 3] ^ Buffer[j - 8] ^ Buffer[j - 14] ^ Buffer[j - 16];
                Buffer[j] = _rotl(temp, 1);
            }
            a = hashResult[0];
            b = hashResult[1];
            c = hashResult[2];
            d = hashResult[3];
            e = hashResult[4];

            for (int j = 0; j < 20; ++j) {
                uint32_t T = _rotl(a, 5);
                T += ((b & c) ^ (~b & d)) + e + 0x5A827999 + Buffer[j];
                e = d;
                d = c;
                c = _rotl(b, 30);
                b = a;
                a = T;
            }
            for (int j = 20; j < 40; ++j) {
                uint32_t T = _rotl(a, 5);
                T += (b ^ c ^ d) + e + 0x6ED9EBA1 + Buffer[j];
                e = d;
                d = c;
                c = _rotl(b, 30);
                b = a;
                a = T;
            }
            for (int j = 40; j < 60; ++j) {
                uint32_t T = _rotl(a, 5);
                T += ((b & c) ^ (b & d) ^ (c & d)) + e + 0x8F1BBCDC + Buffer[j];
                e = d;
                d = c;
                c = _rotl(b, 30);
                b = a;
                a = T;
            }
            for (int j = 60; j < 80; ++j) {
                uint32_t T = _rotl(a, 5);
                T += (b ^ c ^ d) + e + 0xCA62C1D6 + Buffer[j];
                e = d;
                d = c;
                c = _rotl(b, 30);
                b = a;
                a = T;
            }
            hashResult[0] += a;
            hashResult[1] += b;
            hashResult[2] += c;
            hashResult[3] += d;
            hashResult[4] += e;
        }
    }

    void Final(const uint8_t srcBytes[], size_t srcBytesLength, uint32_t hashResult[5]) {
        size_t RoundsOfMainCycle = srcBytesLength / 64;
        size_t LeftoverBytesCount = srcBytesLength % 64;

        uint8_t Extra[128] = { };
        const uint8_t(*MessageBlock)[64] = reinterpret_cast<const uint8_t(*)[64]>(srcBytes);

        for (size_t i = 0; i < LeftoverBytesCount; ++i)
            Extra[i] = MessageBlock[RoundsOfMainCycle][i];

        Extra[LeftoverBytesCount] = 0x80;
        size_t OffsetOfExtra = LeftoverBytesCount >= 56 ? 120 : 56;

        *reinterpret_cast<uint64_t*>(Extra + OffsetOfExtra) = _byteswap_uint64(static_cast<uint64_t>(srcBytesLength) * 8);

        MainCycle(Extra, LeftoverBytesCount >= 56 ? 128 : 64, hashResult);

        for (int i = 0; i < 5; ++i)
            hashResult[i] = _byteswap_ulong(hashResult[i]);
    }

}
