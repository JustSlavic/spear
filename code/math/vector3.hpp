#ifndef MATH_VECTOR3_HPP
#define MATH_VECTOR3_HPP

#include <base.h>
#include <math/vector2.hpp>


namespace math
{


struct vector3
{
    union
    {
        struct { float32 x, y, z; };
        struct { float32 u, v, pad_1; };
        struct { float32 r, g, b; };
        struct { float32 _1, _2, _3; };
        float32 e[3];

        struct { vector2 xy; float32 pad_2; };
        struct { vector2 rg; float32 pad_3; };
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
        vector3 result;
        result.x = 0.f;
        result.y = 0.f;
        result.z = 0.f;
        return result;
    }

    static vector3 ex()
    {
        vector3 result;
        result.x = 1.f;
        result.y = 0.f;
        result.z = 0.f;
        return result;
    }

    static vector3 ey()
    {
        vector3 result;
        result.x = 0.f;
        result.y = 1.f;
        result.z = 0.f;
        return result;
    }

    static vector3 ez()
    {
        vector3 result;
        result.x = 0.f;
        result.y = 0.f;
        result.z = 1.f;
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
    vector3 result;
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    return result;
}

INLINE vector3 operator + (vector3 a, vector3 b)
{
    vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

INLINE vector3 operator - (vector3 a, vector3 b)
{
    vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

INLINE vector3 operator * (vector3 a, float32 c)
{
    vector3 result;
    result.x = a.x * c;
    result.y = a.y * c;
    result.z = a.z * c;
    return result;
}

INLINE vector3 operator * (float32 c, vector3 a)
{
    vector3 result;
    result.x = c * a.x;
    result.y = c * a.y;
    result.z = c * a.z;
    return result;
}

INLINE vector3 operator / (vector3 a, float32 c)
{
    vector3 result;
    result.x = a.x / c;
    result.y = a.y / c;
    result.z = a.z / c;
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
    vector3 result;
    result.x = value;
    result.y = value;
    result.z = value;
    return result;
}

vector3 make_vector3 (float32 x, float32 y, float32 z)
{
    vector3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

vector3 make_vector3 (vector2 xy, float32 z)
{
    vector3 result;
    result.x = xy.x;
    result.y = xy.y;
    result.z = z;
    return result;
}

INLINE float32 dot(vector3 a, vector3 b)
{
    float32 result = a.x * b.x + a.y * b.y + a.z * b.z;
    return result;
}

FORCE_INLINE float32 inner(vector3 a, vector3 b)
{
    return dot(a, b);
}

INLINE vector3 cross(vector3 a, vector3 b)
{
    vector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
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
    vector3 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}

INLINE vector3 coordinate_divide(vector3 a, vector3 b)
{
    vector3 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
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
