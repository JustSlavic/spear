#ifndef MATH_VECTOR2_HPP
#define MATH_VECTOR2_HPP

#include <base.hpp>
#include <math/float32.hpp>


namespace math {


struct vector2
{
    union
    {
        struct { float32 x, y; };
        struct { float32 u, v; };
        struct { float32 _1, _2; };
        float32 e[2];
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

    static vector2 zero()
    {
        vector2 result = { 0.0f, 0.0f };
        return result;
    }

    static vector2 ex()
    {
        vector2 result = { 1.0f, 0.0f };
        return result;
    }

    static vector2 ey()
    {
        vector2 result = { 0.0f, 1.0f };
        return result;
    }
};

INLINE vector2 & operator += (vector2 & a, vector2 b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

INLINE vector2 &operator -= (vector2 &a, vector2 b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

INLINE vector2 &operator *= (vector2 &a, float32 c)
{
    a.x *= c;
    a.y *= c;
    return a;
}

INLINE vector2 operator - (vector2 a)
{
    vector2 result = { -a.x, -a.y };
    return result;
}

INLINE vector2 operator + (vector2 a, vector2 b)
{
    vector2 result = { a.x + b.x, a.y + b.y };
    return result;
}

INLINE vector2 operator - (vector2 a, vector2 b)
{
    vector2 result = { a.x - b.x, a.y - b.y };
    return result;
}

INLINE vector2 operator * (vector2 a, float32 c)
{
    vector2 result = { c * a.x, c * a.y };
    return result;
}

INLINE vector2 operator * (float32 c, vector2 a)
{
    vector2 result = { c * a.x, c * a.y };
    return result;
}

INLINE vector2 operator / (vector2 a, float32 c)
{
    vector2 result = { a.x / c, a.y / c };
    return result;
}

INLINE bool operator == (vector2 a, vector2 b)
{
    bool result = (a.x == b.x) && (a.y == b.y);
    return result;
}

INLINE bool operator != (vector2 a, vector2 b)
{
    bool result = !(a == b);
    return result;
}

#define V2_1(xy) ::math::make_vector2((float32) (xy))
#define V2_2(x, y) ::math::make_vector2((float32) (x), (float32) (y))

#define V2(...) MACRO_EXPAND(MACRO_OVERLOAD_2(__VA_ARGS__, V2_2, V2_1)(__VA_ARGS__))

vector2 make_vector2(float32 value)
{
    vector2 result = { value, value };
    return result;
}

vector2 make_vector2(float32 x, float32 y)
{
    vector2 result = { x, y };
    return result;
}

INLINE float32 dot(vector2 a, vector2 b)
{
    float32 result = a.x * b.x + a.y * b.y;
    return result;
}

INLINE float32 length_squared(vector2 a)
{
    float32 result = dot(a, a);
    return result;
}

INLINE float32 length(vector2 a)
{
    float32 result = square_root(length_squared(a));
    return result;
}

INLINE vector2 coordinate_multiply(vector2 a, vector2 b)
{
    vector2 result = { a.x * b.x, a.y * b.y };
    return result;
}

INLINE vector2 coordinate_divide(vector2 a, vector2 b)
{
    vector2 result = { a.x / b.x, a.y / b.y };
    return result;
}

INLINE void normalize(vector2 &a)
{
    float32 n = length(a);
    if (n > 0)
    {
        a.x /= n;
        a.y /= n;
    }
}

INLINE void normalize(vector2 &a, float32 *norm)
{
    float32 n = length(a);
    if (n > 0)
    {
        a.x /= n;
        a.y /= n;
    }
    *norm = n;
}

INLINE vector2 normalized(vector2 a)
{
    vector2 result = a;
    normalize(result);
    return result;
}

INLINE vector2 normalized(vector2 a, float32 *norm)
{
    vector2 result = a;
    normalize(result, norm);
    return result;
}

// Reflect a vector along the normal to the mirror
// \  |n /
// v\ | /r
//   \|/
// -------mirror
INLINE vector2 reflect(vector2 v, vector2 n)
{
    // v = dot(v, n)*n + (v - dot(v, n)*n)
    // reflect the component that is perpendicular to n
    // r = - dot(v, n)*n + (v - dot(v, n)*n)
    // r = v - 2*dot(v, n)*n
    vector2 result = v - 2.0f*dot(v, n)*n;
    return result;
}

// Reflect a vector through a mirror
//    |mirror
// \  |  /
// v\ | /r
//   \|/
//    |
INLINE vector2 mirror(vector2 v, vector2 m)
{
    // The same as 'reflect', but makes result is conserves the direction of vector
    vector2 result = -reflect(v, m);
    return result;
}


} // namespace math

#endif // MATH_VECTOR2_HPP
