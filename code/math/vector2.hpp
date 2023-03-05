#ifndef MATH_VECTOR2_HPP
#define MATH_VECTOR2_HPP

#include <base.hpp>
#include <math/float32.hpp>


namespace math
{


struct vector2
{
    union
    {
        struct { float32 x, y; };
        struct { float32 u, v; };
        struct { float32 _1, _2; };
        float32 e[2];
    };

    float32 & operator [] (int32_t index)
    {
        ASSERT_MSG(index < ARRAY_COUNT(e), "Attempt to access vector element out of range");

        float32 & result = e[index];
        return result;
    }

    float *data() {
        return &e[0];
    }

    float const *data() const {
        return &e[0];
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

INLINE vector2 operator - (vector2 a) {
    vector2 result = { -a.x, -a.y };
    return result;
}

INLINE vector2 operator + (vector2 a, vector2 b) {
    vector2 result = { a.x + b.x, a.y + b.y };
    return result;
}

INLINE vector2 operator - (vector2 a, vector2 b) {
    vector2 result = { a.x - b.x, a.y - b.y };
    return result;
}

INLINE vector2 operator * (vector2 a, float32 c) {
    vector2 result = { c * a.x, c * a.y };
    return result;
}

INLINE vector2 operator * (float32 c, vector2 a) {
    vector2 result = { c * a.x, c * a.y };
    return result;
}

INLINE vector2 operator / (vector2 a, float32 c) {
    vector2 result = { a.x / c, a.y / c };
    return result;
}

INLINE bool operator == (vector2 a, vector2 b) {
    bool result = (a.x == b.x) && (a.y == b.y);
    return result;
}

INLINE bool operator != (vector2 a, vector2 b) {
    bool result = !(a == b);
    return result;
}

template <typename XY>
vector2 make_vector2(XY xy)
{
    vector2 result = { (float32) xy, (float32) xy };
    return result;
}

template <typename X, typename Y>
vector2 make_vector2(X x, Y y)
{
    vector2 result = { (float32) x, (float32) y };
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
    if (n > 0) {
        a.x /= n;
        a.y /= n;
    }
}

INLINE vector2 normalized(vector2 a)
{
    vector2 result = a;
    normalize(result);
    return result;
}


} // namespace math


#endif // MATH_VECTOR2_HPP
