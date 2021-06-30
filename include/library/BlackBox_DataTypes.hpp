#pragma once

#include <cnl/fixed_point.h>
#include <cstdint>

namespace BlackBox {


class Angle {
public:
    using Number = cnl::scaled_integer<std::int32_t, cnl::power<-16>>;

private:
    Number _rads;
    Angle(Number r)
        : _rads(r) {}

public:
    static const Angle Pi;

    Angle()
        : _rads(0) {}

    static Angle rad(Number r) { return Angle(r); }
    static Angle deg(Number d) { return Angle(d * Number(M_PI / 180)); }

    Angle& operator+=(Angle a) {
        _rads += a._rads;
        return *this;
    }
    Angle& operator-=(Angle a) {
        _rads -= a._rads;
        return *this;
    }
    Angle operator-() const { return Angle(-_rads); }
    Angle& operator*=(Number c) {
        _rads *= c;
        return *this;
    }
    Angle& operator/=(Number c) {
        _rads /= c;
        return *this;
    }

    Number deg() const { return _rads * Number(180.0 / M_PI); }
    Number rad() const { return _rads; }
};

inline Angle operator+(Angle a, Angle b) {
    a += b;
    return a;
}

inline Angle operator-(Angle a, Angle b) {
    a -= b;
    return a;
}

inline Angle operator*(Angle a, Angle::_T c) {
    a *= c;
    return a;
}

inline Angle operator/(Angle a, Angle::_T c) {
    a /= c;
    return a;
}

inline Angle operator"" _deg(long double d) {
    return Angle::deg(Angle::_T(d));
}

inline Angle operator"" _rad(long double r) {
    return Angle::rad(Angle::_T(r));
}

inline Angle operator"" _deg(unsigned long long int d) {
    return Angle::deg(d);
}

inline Angle operator"" _rad(unsigned long long int r) {
    return Angle::rad(r);
}

struct Quaternion {
    using Number = cnl::scaled_integer<std::int32_t, cnl::power<-16>>;
    Number w, x, y, z;

    Quaternion(const Quaternion& other) = default;
    Quaternion& operator=(const Quaternion& other) = default;

    Quaternion(Number i_w, Number i_x, Number i_y, Number i_z)
        : w(i_w)
        , x(i_x)
        , y(i_y)
        , z(i_z) {
    }

    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            (w * other.w) - (x * other.x) - (y * other.y) - (z * other.z), // new w
            (w * other.x) + (x * other.w) + (y * other.z) - (z * other.y), // new x
            (w * other.y) - (x * other.z) + (y * other.w) + (z * other.x), // new y
            (w * other.z) + (x * other.y) - (y * other.x) + (z * other.w)); // new z
    };

    Quaternion& operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    };
};
} // namespace BlackBox
