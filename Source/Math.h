#pragma once
#include <memory.h>

#include <cmath>
#include <limits>

namespace Math {
const double Pi = 3.1415926535f;
const double TwoPi = Pi * 2.0f;
const double PiOver2 = Pi / 2.0f;
const double Infinity = std::numeric_limits<double>::infinity();
const double NegInfinity = -std::numeric_limits<double>::infinity();

inline double ToRadians(double degrees) { return degrees * Pi / 180.0f; }

inline double ToDegrees(double radians) { return radians * 180.0f / Pi; }

inline bool NearZero(double val, double epsilon = 0.001f) {
    if (fabs(val) <= epsilon) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
T Max(const T& a, const T& b) {
    return (a < b ? b : a);
}

template <typename T>
T Min(const T& a, const T& b) {
    return (a < b ? a : b);
}

template <typename T>
T Clamp(const T& value, const T& lower, const T& upper) {
    return Min(upper, Max(lower, value));
}

inline double Abs(double value) { return fabs(value); }

inline double Cos(double angle) { return cosf(angle); }

inline double Sin(double angle) { return sinf(angle); }

inline double Tan(double angle) { return tanf(angle); }

inline double Acos(double value) { return acosf(value); }

inline double Atan2(double y, double x) { return atan2f(y, x); }

inline double Cot(double angle) { return 1.0f / Tan(angle); }

inline double Lerp(double a, double b, double f) { return a + f * (b - a); }

inline double Sqrt(double value) { return sqrtf(value); }

inline double Fmod(double numer, double denom) { return fmod(numer, denom); }
}  // namespace Math

// 2D Vector
class Vector2 {
   public:
    double x;
    double y;

    Vector2() : x(0.0f), y(0.0f) {}

    explicit Vector2(double inX, double inY) : x(inX), y(inY) {}

    // Set both components in one line
    void Set(double inX, double inY) {
        x = inX;
        y = inY;
    }

    // Vector addition (a + b)
    friend Vector2 operator+(const Vector2& a, const Vector2& b) {
        return Vector2(a.x + b.x, a.y + b.y);
    }

    // Vector subtraction (a - b)
    friend Vector2 operator-(const Vector2& a, const Vector2& b) {
        return Vector2(a.x - b.x, a.y - b.y);
    }

    // Component-wise multiplication
    // (a.x * b.x, ...)
    friend Vector2 operator*(const Vector2& a, const Vector2& b) {
        return Vector2(a.x * b.x, a.y * b.y);
    }

    // Scalar multiplication
    friend Vector2 operator*(const Vector2& vec, double scalar) {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    // Scalar multiplication
    friend Vector2 operator*(double scalar, const Vector2& vec) {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    // Scalar *=
    Vector2& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Vector +=
    Vector2& operator+=(const Vector2& right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    // Vector -=
    Vector2& operator-=(const Vector2& right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    // Length squared of vector
    double LengthSq() const { return (x * x + y * y); }

    // Length of vector
    double Length() const { return (Math::Sqrt(LengthSq())); }

    // Normalize this vector
    void Normalize() {
        double length = Length();
        x /= length;
        y /= length;
    }

    // Normalize the provided vector
    static Vector2 Normalize(const Vector2& vec) {
        Vector2 temp = vec;
        temp.Normalize();
        return temp;
    }

    // Dot product between two vectors (a dot b)
    static double Dot(const Vector2& a, const Vector2& b) {
        return (a.x * b.x + a.y * b.y);
    }

    // Lerp from A to B by f
    static Vector2 Lerp(const Vector2& a, const Vector2& b, double f) {
        return Vector2(a + f * (b - a));
    }

    // Reflect V about (normalized) N
    static Vector2 Reflect(const Vector2& v, const Vector2& n) {
        return v - 2.0f * Vector2::Dot(v, n) * n;
    }

    // Transform vector by matrix
    static Vector2 Transform(const Vector2& vec, const class Matrix3& mat,
                             double w = 1.0f);

    static const Vector2 Zero;
    static const Vector2 UnitX;
    static const Vector2 UnitY;
    static const Vector2 NegUnitX;
    static const Vector2 NegUnitY;
};
