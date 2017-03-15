#pragma once

template<class T>
struct Vector3
{

    T x;
    T y;
    T z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(const T &x, const T &y, const T &z) : x(x), y(y), z(z) {}

    Vector3& operator+(const Vector3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vector3& operator+=(const Vector3 &v) { x += v.x; y += v.y; z += v.z; return *this; }

    Vector3& operator-(const Vector3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vector3& operator-=(const Vector3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

    Vector3& operator*(const Vector3 &v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    Vector3& operator*=(const Vector3 &v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    Vector3& operator*(const T &scalar) { x *= scalar; y *= scalar; v *= scalar; return *this; }

    Vector3& operator/(const Vector3 &v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
    Vector3& operator/=(const Vector3 &v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

    bool operator==(const Vector3 &v) { return x == v.x && y == v.y && z == v.z; }
    bool operator!=(const Vector3 &v) { return !(*this == v); }

};
