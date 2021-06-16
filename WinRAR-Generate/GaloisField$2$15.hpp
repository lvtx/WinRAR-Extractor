#pragma once
#include <stdexcept>

template<uint16_t primitive_poly>
class GaloisField$2$15 {
private:
    static uint16_t GeneratorToElement[0x8000];
    static uint16_t ElementToGenerator[0x8000];
    uint16_t Data;
public:

    static void InitTable() {
        GeneratorToElement[0] = 1;
        for (int i = 1; i < 0x8000; ++i) {
            uint32_t temp = GeneratorToElement[i - 1] * 2;
            if (temp & 0x8000)
                temp ^= primitive_poly;
            GeneratorToElement[i] = temp;
        }

        for (int i = 0; i < 0x7fff; ++i)
            ElementToGenerator[GeneratorToElement[i]] = i;
    }

    GaloisField$2$15(uint16_t srcData = 0) : Data(srcData) {
        if (Data >= 0x8000)
            throw std::invalid_argument("srcData is not in GF(2 ^ 15).");
    }

    GaloisField$2$15<primitive_poly>
        operator+(const GaloisField$2$15<primitive_poly>& other) const {

        return GaloisField$2$15(Data ^ other.Data);
    }

    GaloisField$2$15<primitive_poly>&
        operator+=(const GaloisField$2$15<primitive_poly>& other) {

        Data ^= other.Data;
        return *this;
    }

    GaloisField$2$15<primitive_poly>
        operator-(const GaloisField$2$15<primitive_poly>& other) const {

        return GaloisField$2$15(Data ^ other.Data);
    }

    GaloisField$2$15<primitive_poly>&
        operator-=(const GaloisField$2$15<primitive_poly>& other) {
        Data ^= other.Data;
        return *this;
    }

    GaloisField$2$15<primitive_poly>
        operator*(const GaloisField$2$15<primitive_poly>& other) const {

        if (other.Data == 0 || Data == 0) {
            return GaloisField$2$15(0);
        } else if (other.Data == 1) {
            return *this;
        } else if (Data == 1) {
            return other;
        }

        int g = ElementToGenerator[Data] + ElementToGenerator[other.Data];
        if (g >= 0x7fff)
            g -= 0x7fff;
        return GeneratorToElement[g];
    }

    GaloisField$2$15<primitive_poly>&
        operator*=(const GaloisField$2$15<primitive_poly>& other) {

        if (other.Data == 0 || Data == 0) {
            Data = 0;
            return *this;
        } else if (other.Data == 1) {
            return *this;
        } else if (Data == 1) {
            Data = other.Data;
            return *this;
        }

        uint16_t g0 = ElementToGenerator[Data], g1 = ElementToGenerator[other.Data];
        Data = GeneratorToElement[(g0 + g1) % 0x7fff];
        return *this;
    }

    GaloisField$2$15<primitive_poly> Inverse() const {
        if (Data == 0)
            throw std::domain_error("Zero does not have inverse.");

        return GaloisField$2$15(GeneratorToElement[0x7fff - ElementToGenerator[Data]]);
    }

    GaloisField$2$15<primitive_poly>
        operator/(const GaloisField$2$15<primitive_poly>& other) const {

        return *this * other.Inverse();
    }

    GaloisField$2$15<primitive_poly>&
        operator/=(const GaloisField$2$15<primitive_poly>& other) {

        *this *= other.Inverse();
        return *this;
    }

    bool operator==(const GaloisField$2$15<primitive_poly>& other) const {
        return Data == other.Data;
    }

    bool operator!=(const GaloisField$2$15<primitive_poly>& other) const {
        return Data != other.Data;
    }

    uint16_t Get() { return Data; }

    void Set(uint16_t srcData) {
        if (srcData >= 0x8000)
            throw std::invalid_argument("srcData is not in GF(2 ^ 15).");
        Data = srcData;
    }

    __declspec(property(get = Get, put = Set)) uint16_t Value;
};

template <uint16_t primitive_poly>
uint16_t GaloisField$2$15<primitive_poly>::GeneratorToElement[0x8000] = { };

template <uint16_t primitive_poly>
uint16_t GaloisField$2$15<primitive_poly>::ElementToGenerator[0x8000] = { };
