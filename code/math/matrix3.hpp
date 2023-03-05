#ifndef MATH_MATRIX3_HPP
#define MATH_MATRIX3_HPP

#include <base.hpp>
#include <math/vector3.hpp>


namespace math
{


struct matrix3
{
    union
    {
        struct {
            float32 _11, _12, _13;
            float32 _21, _22, _23;
            float32 _31, _32, _33;
        };
        vector3 row[3];
        float32 e[3][3];
    };

    float32 *data() {
        return &e[0][0];
    }

    float32 const *data() const {
        return &e[0][0];
    }

    static matrix3 zero()
    {
        matrix3 result = {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
        };
        return result;
    }

    static matrix3 identity()
    {
        matrix3 result = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };
        return result;
    }
};


INLINE matrix3 & operator += (matrix3 & a, matrix3 b)
{
    a._11 += b._11; a._12 += b._12; a._13 += b._13;
    a._21 += b._21; a._22 += b._22; a._23 += b._23;
    a._31 += b._31; a._32 += b._32; a._33 += b._33;
    return a;
}

INLINE matrix3 &operator -= (matrix3 &a, matrix3 b)
{
    a._11 -= b._11; a._12 -= b._12; a._13 -= b._13;
    a._21 -= b._21; a._22 -= b._22; a._23 -= b._23;
    a._31 -= b._31; a._32 -= b._32; a._33 -= b._33;
    return a;
}

INLINE matrix3 &operator *= (matrix3 &a, float32 c)
{
    a._11 *= c; a._12 *= c; a._13 *= c;
    a._21 *= c; a._22 *= c; a._23 *= c;
    a._31 *= c; a._32 *= c; a._33 *= c;
    return a;
}

INLINE matrix3 operator - (matrix3 a) {
    matrix3 result = {
        -a._11, -a._12, -a._13,
        -a._21, -a._22, -a._23,
        -a._31, -a._32, -a._33,
    };
    return result;
}

INLINE matrix3 operator + (matrix3 a, matrix3 b) {
    matrix3 result = {
        a._11 + b._11, a._12 + b._12, a._13 + b._13,
        a._21 + b._21, a._22 + b._22, a._23 + b._23,
        a._31 + b._31, a._32 + b._32, a._33 + b._33,
    };
    return result;
}

INLINE matrix3 operator - (matrix3 a, matrix3 b) {
    matrix3 result = {
        a._11 - b._11, a._12 - b._12, a._13 - b._13,
        a._21 - b._21, a._22 - b._22, a._23 - b._23,
        a._31 - b._31, a._32 - b._32, a._33 - b._33,
    };
    return result;
}

INLINE matrix3 operator * (matrix3 a, f32 c) {
    matrix3 result = {
        a._11 * c, a._12 * c, a._13 * c,
        a._21 * c, a._22 * c, a._23 * c,
        a._31 * c, a._32 * c, a._33 * c,
    };
    return result;
}

INLINE matrix3 operator * (f32 c, matrix3 a) {
    matrix3 result = {
        a._11 * c, a._12 * c, a._13 * c,
        a._21 * c, a._22 * c, a._23 * c,
        a._31 * c, a._32 * c, a._33 * c,
    };
    return result;
}

INLINE matrix3 operator / (matrix3 a, f32 c) {
    matrix3 result = {
        a._11 / c, a._12 / c, a._13 / c,
        a._21 / c, a._22 / c, a._23 / c,
        a._31 / c, a._32 / c, a._33 / c,
    };
    return result;
}

INLINE bool operator == (matrix3 a, matrix3 b) {
    bool result = (a._11 == b._11) && (a._12 == b._12) && (a._13 == b._13)
               && (a._21 == b._21) && (a._22 == b._22) && (a._23 == b._23)
               && (a._31 == b._31) && (a._32 == b._32) && (a._33 == b._33);
    return result;
}

INLINE bool operator != (matrix3 a, matrix3 b) {
    bool result = !(a == b);
    return result;
}

template <typename T>
matrix3 make_matrix3 (T t)
{
    matrix3 result = {
        t, t, t,
        t, t, t,
        t, t, t,
    };
    return result;
}

template <typename T11, typename T12, typename T13,
          typename T21, typename T22, typename T23,
          typename T31, typename T32, typename T33>
matrix3 make_matrix3 (T11 t11, T12 t12, T13 t13,
                      T21 t21, T22 t22, T23 t23,
                      T31 t31, T32 t32, T33 t33)
{
    matrix3 result = {
        (float32) t11, (float32) t12, (float32) t13,
        (float32) t21, (float32) t22, (float32) t23,
        (float32) t31, (float32) t32, (float32) t33,
    };
    return result;
}


// @todo:
// - transpose
// - determinant
// - m * a
// - a * m
// - m * m


} // namespace math


#endif // MATH_MATRIX3_HPP
