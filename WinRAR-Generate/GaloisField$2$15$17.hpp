#pragma once
#include "GaloisField$2$15.hpp"

template<uint16_t base_poly, const GaloisField$2$15<base_poly>(&extend_poly)[18]>
class GaloisField$2$15$17 {
private:
    GaloisField$2$15<base_poly> Data[17];
public:
    GaloisField$2$15$17(uint16_t srcData = 0) : Data{ srcData } { }

    GaloisField$2$15$17(const uint16_t(&srcData)[17]) {
        for (int i = 0; i < 17; ++i)
            Data[i].Value = srcData[i];
    }

    GaloisField$2$15$17<base_poly, extend_poly>
        operator+(const GaloisField$2$15$17<base_poly, extend_poly>& other) const {

        GaloisField$2$15$17<base_poly, extend_poly> ret;
        for (int i = 0; i < 17; ++i)
            ret.Data[i] = Data[i] + other.Data[i];

        return ret;
    }

    GaloisField$2$15$17<base_poly, extend_poly>&
        operator+=(const GaloisField$2$15$17<base_poly, extend_poly>& other) {

        for (int i = 0; i < 17; ++i)
            Data[i] += other.Data[i];

        return *this;
    }

    GaloisField$2$15$17<base_poly, extend_poly>
        operator-(const GaloisField$2$15$17<base_poly, extend_poly>& other) const {

        return *this + other;
    }

    GaloisField$2$15$17<base_poly, extend_poly>&
        operator-=(const GaloisField$2$15$17<base_poly, extend_poly>& other) {

        return *this += other;
    }

    GaloisField$2$15$17<base_poly, extend_poly>
        operator*(const GaloisField$2$15$17<base_poly, extend_poly>& other) const {

        GaloisField$2$15<base_poly> temp[33] = { };
        for (int i = 0; i < 17; ++i) {
            for (int j = 0; j < 17; ++j) {
                temp[i + j] += Data[i] * other.Data[j];
            }
        }

        for (int i = 33 - 1; i > 17 - 1; --i) {
            if (temp[i].Value != 0) {
                for (int j = 0; j < 17; ++j)
                    temp[i - 17 + j] += temp[i] * extend_poly[j];
            }
        }

        GaloisField$2$15$17 ret;
        for (int i = 0; i < 17; ++i)
            ret.Data[i] = temp[i];

        return ret;
    }

    /*template<uint16_t _base_poly, const GaloisField$2$15<base_poly>(&_extend_poly)[18]>*/
    GaloisField$2$15$17<base_poly, extend_poly>&
        operator*=(const GaloisField$2$15$17<base_poly, extend_poly>& other) {

        GaloisField$2$15<base_poly> temp[33] = { };
        for (int i = 0; i < 17; ++i) {
            for (int j = 0; j < 17; ++j) {
                temp[i + j] += Data[i] * other.Data[j];
            }
        }

        for (int i = 33 - 1; i > 17 - 1; --i) {
            if (temp[i].Value != 0) {
                for (int j = 0; j < 17; ++j)
                    temp[i - 17 + j] += temp[i] * extend_poly[j];
            }
        }

        for (int i = 0; i < 17; ++i)
            Data[i] = temp[i];

        return *this;
    }

//     template<>
//     GaloisField$2$15$17<base_poly, extend_poly>&
//         operator*=(const GaloisField$2$15$17<base_poly, extend_poly>& other) {
// 
//         GaloisField$2$15<_base_poly> temp[33] = { };
//         for (int i = 0; i < 17; ++i) {
//             for (int j = 0; j < 17; ++j) {
//                 temp[i + j] += Data[i] * other.Data[j];
//             }
//         }
// 
//         for (int i = 33 - 1; i > 17 - 1; --i) {
//             if (temp[i].Value != 0) {
//                 for (int j = 0; j < 17; ++j)
//                     temp[i - 17 + j] += temp[i] * extend_poly[j];
//             }
//         }
// 
//         for (int i = 0; i < 17; ++i)
//             Data[i] = temp[i];
// 
//         return *this;
//     }

    GaloisField$2$15$17<base_poly, extend_poly> Inverse() const {
        GaloisField$2$15$17<base_poly, extend_poly> ret;
        ret.Data[0].Value = 1;

        GaloisField$2$15$17<base_poly, extend_poly> temp(*this);
        for (int i = 1; i < 15 * 17; ++i) {
            temp *= temp;
            ret *= temp;
        }

        return ret;
    }

    GaloisField$2$15$17<base_poly, extend_poly>
        operator/(const GaloisField$2$15$17<base_poly, extend_poly>& other) const {

        return *this * other.Inverse();
    }

    GaloisField$2$15$17<base_poly, extend_poly>&
        operator/=(const GaloisField$2$15$17<base_poly, extend_poly>& other) const {

        return *this *= other.Inverse();
    }

    bool operator==(const GaloisField$2$15$17<base_poly, extend_poly>& other) const {
        for (int i = 0; i < 17; ++i) {
            if (Data[i] != other.Data[i])
                return false;
        }
        return true;
    }

    bool operator!=(const GaloisField$2$15$17<base_poly, extend_poly>& other) const {
        for (int i = 0; i < 17; ++i) {
            if (Data[i] != other.Data[i])
                return true;
        }
        return false;
    }

    GaloisField$2$15$17<base_poly, extend_poly>&
        operator=(uint16_t srcData) {
        Data[0].Value = srcData;

        for (int i = 1; i < 17; ++i)
            Data[i].Value = 0;

        return *this;
    }

    GaloisField$2$15$17<base_poly, extend_poly>&
        operator=(const uint16_t(&srcData)[17]) {

        for (int i = 0; i < 17; ++i)
            Data[i].Value = srcData[i];

        return *this;
    }

    void ToDecimal(uint64_t (&src)[4]) {
        memset(src, 0, sizeof(src));

        src[0] |= static_cast<uint64_t>(Data[0].Value);             //  0 - 14
        src[0] |= static_cast<uint64_t>(Data[1].Value) << 15;       //  15 - 29
        src[0] |= static_cast<uint64_t>(Data[2].Value) << 30;       //  30 - 44
        src[0] |= static_cast<uint64_t>(Data[3].Value) << 45;       //  45 - 59

        src[0] |= static_cast<uint64_t>(Data[4].Value) << 60;       //  60 - 63
        src[1] |= static_cast<uint64_t>(Data[4].Value) >> 4;        //  0 - 10

        src[1] |= static_cast<uint64_t>(Data[5].Value) << 11;       //  11 - 25
        src[1] |= static_cast<uint64_t>(Data[6].Value) << 26;       //  26 - 40
        src[1] |= static_cast<uint64_t>(Data[7].Value) << 41;       //  41 - 55

        src[1] |= static_cast<uint64_t>(Data[8].Value) << 56;       //  56 - 63
        src[2] |= static_cast<uint64_t>(Data[8].Value) >> 8;        //  0 - 6

        src[2] |= static_cast<uint64_t>(Data[9].Value) << 7;        //  7 - 21
        src[2] |= static_cast<uint64_t>(Data[10].Value) << 22;      //  22 - 36
        src[2] |= static_cast<uint64_t>(Data[11].Value) << 37;      //  37 - 51

        src[2] |= static_cast<uint64_t>(Data[12].Value) << 52;      //  52 - 63
        src[3] |= static_cast<uint64_t>(Data[12].Value) >> 12;      //  0 - 2

        src[3] |= static_cast<uint64_t>(Data[13].Value) << 3;       //  3 - 17
        src[3] |= static_cast<uint64_t>(Data[14].Value) << 18;      //  18 - 32
        src[3] |= static_cast<uint64_t>(Data[15].Value) << 33;      //  33 - 47
        src[3] |= static_cast<uint64_t>(Data[16].Value) << 48;      //  48 - 62
    }

    uint16_t Get(size_t i) { return Data[i].Value; }

    void Set(size_t i, uint16_t value) {
        Data[i].Value = value;
    }

    __declspec(property(get = Get, put = Set)) uint16_t Value[];

};
