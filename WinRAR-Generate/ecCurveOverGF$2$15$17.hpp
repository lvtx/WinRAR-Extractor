#pragma once
#include "GaloisField$2$15$17.hpp"

template<uint16_t base_poly, const GaloisField$2$15<base_poly>(&extend_poly)[18]>
class ecCurveOverGF$2$15$17 {
private:
    GaloisField$2$15$17<base_poly, extend_poly> a;
    GaloisField$2$15$17<base_poly, extend_poly> b;
public:

    class ecPoint {
        friend ecCurveOverGF$2$15$17<base_poly, extend_poly>;
    private:
        const ecCurveOverGF$2$15$17<base_poly, extend_poly>&  ReferenceCurve;
    public:
        GaloisField$2$15$17<base_poly, extend_poly> X;
        GaloisField$2$15$17<base_poly, extend_poly> Y;

        ecPoint(const ecCurveOverGF$2$15$17<base_poly, extend_poly>& ref_curve,
                uint16_t _X = 0,
                uint16_t _Y = 0) : ReferenceCurve(ref_curve), X(_X), Y(_Y) {
        }

        ecPoint(const ecCurveOverGF$2$15$17<base_poly, extend_poly>& ref_curve,
                const uint16_t(&_X)[17],
                const uint16_t(&_Y)[17]) : ReferenceCurve(ref_curve), X(_X), Y(_Y) {
        }

        ecPoint(const ecCurveOverGF$2$15$17<base_poly, extend_poly>& ref_curve,
                const GaloisField$2$15$17<base_poly, extend_poly>& _X,
                const GaloisField$2$15$17<base_poly, extend_poly>& _Y) : ReferenceCurve(ref_curve), X(_X), Y(_Y) {
        }

        ecPoint operator-() {
            return ecPoint(X, X + Y);
        }

        ecPoint& operator=(const ecPoint& other) {
            X = other.X;
            Y = other.Y;
            return *this;
        }

        bool operator==(const ecPoint& other) const {
            return X == other.X && Y == other.Y;
        }

        bool operator!=(const ecPoint& other) const {
            return X != other.X || Y != other.Y;
        }

        ecPoint GetDouble() const {
            ecPoint ret(ReferenceCurve);

            auto m = Y / X + X;
            ret.X = m * m;
            ret.X += m;
            ret.X += ReferenceCurve.a;

            ret.Y = X + ret.X;
            ret.Y *= m;
            ret.Y += ret.X;
            ret.Y += Y;

            return ret;
        }

        ecPoint& SelfDouble() {
            auto m = Y / X + X;
            auto Rx = m * m;
            Rx += m;
            Rx += ReferenceCurve.a;

            auto Ry = X + Rx;
            Ry *= m;
            Ry += Rx;
            Ry += Y;

            X = Rx;
            Y = Ry;

            return *this;
        }

        ecPoint operator+(const ecPoint& other) const {
            ecPoint ret(ReferenceCurve);

            if (*this == ret)
                return other;

            if (X != other.X) {
                auto m = (Y + other.Y) / (X + other.X);
                ret.X = m * m;
                ret.X += m;
                ret.X += X;
                ret.X += other.X;
                ret.X += ReferenceCurve.a;

                ret.Y = X + ret.X;
                ret.Y *= m;
                ret.Y += ret.X;
                ret.Y += Y;

                return ret;
            } else {
                return GetDouble();
            }
        }

        ecPoint& operator+=(const ecPoint& other) {
            ecPoint ret(ReferenceCurve);

            if (*this == ret) {
                *this = other;
                return *this;
            }

            if (X != other.X) {
                auto m = (Y + other.Y) / (X + other.X);
                ret.X = m * m;
                ret.X += m;
                ret.X += X;
                ret.X += other.X;
                ret.X += ReferenceCurve.a;

                ret.Y = X + ret.X;
                ret.Y *= m;
                ret.Y += ret.X;
                ret.Y += Y;

                *this = ret;
                return *this;
            } else {
                return SelfDouble();
            }
        }

        ecPoint operator-(const ecPoint& other) const {
            return *this + -other;
        }

        ecPoint& operator-=(const ecPoint& other) {
            return *this += -other;
        }

        ecPoint GetNP(const uint64_t n[], size_t n_length) const {
            ecPoint ret(ReferenceCurve);
            ecPoint temp = *this;

            uint64_t probe = 1;
            for (size_t i = 0; i < n_length; ++i) {
                while (probe != 0) {
                    if (n[i] & probe)
                        ret += temp;
                    temp.SelfDouble();
                    probe <<= 1;
                }
                probe = 1;
            }

            return ret;
        }


    };

    ecCurveOverGF$2$15$17(uint16_t _a,
                          uint16_t _b) : a(_a), b(_b) { }

    ecCurveOverGF$2$15$17(const GaloisField$2$15$17<base_poly, extend_poly>& _a,
                          const GaloisField$2$15$17<base_poly, extend_poly>& _b) : a(_a), b(_b) { }

    ecCurveOverGF$2$15$17(const uint16_t(&_a)[17],
                          const uint16_t(&_b)[17]) : a(_a), b(_b) { }

    ecPoint GetPoint(uint16_t _X = 0,
                     uint16_t _Y = 0) {
        return ecPoint(*this, _X, _Y);
    }

    ecPoint GetPoint(const uint16_t(&_X)[17],
                     const uint16_t(&_Y)[17]) {
        return ecPoint(*this, _X, _Y);
    }

    ecPoint GetPoint(const GaloisField$2$15$17<base_poly, extend_poly>& _X,
                     const GaloisField$2$15$17<base_poly, extend_poly>& _Y) {
        return ecPoint(*this, _X, _Y);
    }


};
