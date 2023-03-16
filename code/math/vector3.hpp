#ifndef MATH_VECTOR3_HPP
#define MATH_VECTOR3_HPP

#include <base.hpp>
#include <math/vector2.hpp>


namespace math
{


struct vector3
{
    union
    {
        struct { float32 x, y, z; };
        struct { float32 u, v, w; };
        struct { float32 r, g, b; };
        struct { float32 _1, _2, _3; };
        float32 e[3];

        struct { vector2 xy; float32 pad_1; };
        struct { vector2 rg; float32 pad_2; };
    };

    float32 & operator [] (uint32 index)
    {
        ASSERT_MSG(index < ARRAY_COUNT(e), "Attempt to access vector element out of range");

        float32 & result = e[index];
        return result;
    }

    float *data()
    {
        return &e[0];
    }

    float const *data() const
    {
        return &e[0];
    }

    static vector3 zero()
    {
        vector3 result = { 0.0f, 0.0f, 0.0f };
        return result;
    }

    static vector3 ex()
    {
        vector3 result = { 1.0f, 0.0f, 0.0f };
        return result;
    }

    static vector3 ey()
    {
        vector3 result = { 0.0f, 1.0f, 0.0f };
        return result;
    }

    static vector3 ez()
    {
        vector3 result = { 0.0f, 0.0f, 1.0f };
        return result;
    }
};

INLINE vector3 & operator += (vector3 & a, vector3 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

INLINE vector3 &operator -= (vector3 &a, vector3 b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

INLINE vector3 &operator *= (vector3 &a, float32 c)
{
    a.x *= c;
    a.y *= c;
    a.z *= c;
    return a;
}

INLINE vector3 operator - (vector3 a)
{
    vector3 result = { -a.x, -a.y, -a.z };
    return result;
}

INLINE vector3 operator + (vector3 a, vector3 b)
{
    vector3 result = { a.x + b.x, a.y + b.y, a.z + b.z };
    return result;
}

INLINE vector3 operator - (vector3 a, vector3 b)
{
    vector3 result = { a.x - b.x, a.y - b.y, a.z - b.z };
    return result;
}

INLINE vector3 operator * (vector3 a, float32 c)
{
    vector3 result = { c * a.x, c * a.y, c * a.z };
    return result;
}

INLINE vector3 operator * (float32 c, vector3 a)
{
    vector3 result = { c * a.x, c * a.y, c * a.z };
    return result;
}

INLINE vector3 operator / (vector3 a, float32 c)
{
    vector3 result = { a.x / c, a.y / c, a.z / c };
    return result;
}

INLINE bool operator == (vector3 a, vector3 b)
{
    bool result = (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
    return result;
}

INLINE bool operator != (vector3 a, vector3 b)
{
    bool result = !(a == b);
    return result;
}

#define V3_1(xyz) ::math::make_vector3((float32) (xyz))
#define V3_2(xy, z) ::math::make_vector3((xy), (float32) (z))
#define V3_3(x, y, z) ::math::make_vector3((float32) (x), (float32) (y), (float32) (z))

#define V3(...) MACRO_EXPAND(MACRO_OVERLOAD_3(__VA_ARGS__, V3_3, V3_2, V3_1)(__VA_ARGS__))

vector3 make_vector3 (float32 value)
{
    vector3 result = { value, value, value };
    return result;
}

vector3 make_vector3 (float32 x, float32 y, float32 z)
{
    vector3 result = { x, y, z };
    return result;
}

vector3 make_vector3 (vector2 xy, float32 z)
{
    vector3 result = { xy.x, xy.y, z };
    return result;
}

INLINE float32 dot(vector3 a, vector3 b)
{
    float32 result = a.x * b.x + a.y * b.y + a.z * b.z;
    return result;
}

INLINE vector3 cross(vector3 a, vector3 b)
{
    vector3 result = { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
    return result;
}

INLINE float32 length_squared(vector3 a)
{
    float32 result = dot(a, a);
    return result;
}

INLINE float32 length(vector3 a)
{
    float32 result = square_root(length_squared(a));
    return result;
}

INLINE vector3 coordinate_multiply(vector3 a, vector3 b)
{
    vector3 result = { a.x * b.x, a.y * b.y, a.z * b.z };
    return result;
}

INLINE vector3 coordinate_divide(vector3 a, vector3 b)
{
    vector3 result = { a.x / b.x, a.y / b.y, a.z / b.z };
    return result;
}

INLINE void normalize(vector3 &a)
{
    float32 n = length(a);
    if (n > 0)
    {
        a.x /= n;
        a.y /= n;
        a.z /= n;
    }
}

INLINE vector3 normalized(vector3 a)
{
    vector3 result = a;
    normalize(result);
    return result;
}


} // namespace math


#endif // MATH_VECTOR3_HPP
