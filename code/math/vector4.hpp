#ifndef MATH_VECTOR4_HPP
#define MATH_VECTOR4_HPP

#include <base.hpp>
#include <math/vector2.hpp>
#include <math/vector3.hpp>


namespace math
{


struct vector4
{
    union
    {
        struct { float32 x, y, z, w; };
        struct { float32 r, g, b, a; };
        struct { float32 _1, _2, _3, _4; };
        float32 e[4];

        struct { vector2 xy, zw; };
        struct { vector2 rg, ba; };
        struct { vector3 xyz; float32 pad_1; };
        struct { vector3 rgb; float32 pad_2; };
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

    static vector4 zero()
    {
        vector4 result;
        result.x = 0.f;
        result.y = 0.f;
        result.z = 0.f;
        result.w = 0.f;
        return result;
    }

    static vector4 ex()
    {
        vector4 result;
        result.x = 1.f;
        result.y = 0.f;
        result.z = 0.f;
        result.w = 0.f;
        return result;
    }

    static vector4 ey()
    {
        vector4 result;
        result.x = 0.f;
        result.y = 1.f;
        result.z = 0.f;
        result.w = 0.f;
        return result;
    }

    static vector4 ez()
    {
        vector4 result;
        result.x = 0.f;
        result.y = 0.f;
        result.z = 1.f;
        result.w = 0.f;
        return result;
    }

    static vector4 ew()
    {
        vector4 result;
        result.x = 0.f;
        result.y = 0.f;
        result.z = 0.f;
        result.w = 1.f;
        return result;
    }
};

INLINE vector4 & operator += (vector4 & a, vector4 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
    return a;
}

INLINE vector4 &operator -= (vector4 &a, vector4 b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    a.w -= b.w;
    return a;
}

INLINE vector4 &operator *= (vector4 &a, float32 c)
{
    a.x *= c;
    a.y *= c;
    a.z *= c;
    a.w *= c;
    return a;
}

INLINE vector4 operator - (vector4 a)
{
    vector4 result;
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    result.w = -a.w;
    return result;
}

INLINE vector4 operator + (vector4 a, vector4 b)
{
    vector4 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    return result;
}

INLINE vector4 operator - (vector4 a, vector4 b)
{
    vector4 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    return result;
}

INLINE vector4 operator * (vector4 a, float32 c)
{
    vector4 result;
    result.x = a.x * c;
    result.y = a.y * c;
    result.z = a.z * c;
    result.w = a.w * c;
    return result;
}

INLINE vector4 operator * (float32 c, vector4 a)
{
    vector4 result;
    result.x = c * a.x;
    result.y = c * a.y;
    result.z = c * a.z;
    result.w = c * a.w;
    return result;
}

INLINE vector4 operator / (vector4 a, float32 c)
{
    vector4 result;
    result.x = a.x / c;
    result.y = a.y / c;
    result.z = a.z / c;
    result.w = a.w / c;
    return result;
}

INLINE bool operator == (vector4 a, vector4 b)
{
    bool result = (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
    return result;
}

INLINE bool operator != (vector4 a, vector4 b)
{
    bool result = !(a == b);
    return result;
}

#define V4_1(xyzw) ::math::make_vector4((float32) (xyzw))
#define V4_2(xyz, w) ::math::make_vector4((xyz), (float32) (w))
#define V4_3(xy, z, w) ::math::make_vector4((xy), (float32) (z), (float32) (w))
#define V4_4(x, y, z, w) ::math::make_vector4((float32) (x), (float32) (y), (float32) (z), (float32) (w))

#define V4(...) MACRO_EXPAND(MACRO_OVERLOAD_4(__VA_ARGS__, V4_4, V4_3, V4_2, V4_1)(__VA_ARGS__))

vector4 make_vector4 (float32 value)
{
    vector4 result;
    result.x = value;
    result.y = value;
    result.z = value;
    result.w = value;
    return result;
}

vector4 make_vector4 (float32 x, float32 y, float32 z, float32 w)
{
    vector4 result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

vector4 make_vector4 (vector2 xy, float32 z, float32 w)
{
    vector4 result;
    result.x = xy.x;
    result.y = xy.y;
    result.z = z;
    result.w = w;
    return result;
}

vector4 make_vector4 (vector3 xyz, float32 w)
{
    vector4 result;
    result.x = xyz.x;
    result.y = xyz.y;
    result.z = xyz.z;
    result.w = w;
    return result;
}

INLINE float32 dot(vector4 a, vector4 b)
{
    float32 result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
    return result;
}

INLINE float32 length_squared(vector4 a)
{
    float32 result = dot(a, a);
    return result;
}

INLINE float32 length(vector4 a)
{
    float32 result = square_root(length_squared(a));
    return result;
}

INLINE vector4 coordinate_multiply(vector4 a, vector4 b)
{
    vector4 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    return result;
}

INLINE vector4 coordinate_divide(vector4 a, vector4 b)
{
    vector4 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;
    return result;
}

INLINE void normalize(vector4 &a)
{
    float32 n = length(a);
    if (n > 0)
    {
        a.x /= n;
        a.y /= n;
        a.z /= n;
        a.w /= n;
    }
}

INLINE vector4 normalized(vector4 a)
{
    vector4 result = a;
    normalize(result);
    return result;
}


} // namespace math


#endif // MATH_VECTOR4_HPP
