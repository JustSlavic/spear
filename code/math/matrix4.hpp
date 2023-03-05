#ifndef MATH_MATRIX4_HPP
#define MATH_MATRIX4_HPP

#include <base.hpp>
#include <math/vector4.hpp>


namespace math
{


struct matrix4
{
    union
    {
        struct {
            float32 _11, _12, _13, _14;
            float32 _21, _22, _23, _24;
            float32 _31, _32, _33, _34;
            float32 _41, _42, _43, _44;
        };
        vector4 row[4];
        float32 e[4][4];
    };

    float32 *data() {
        return &e[0][0];
    }

    float32 const *data() const {
        return &e[0][0];
    }

    static matrix4 zero()
    {
        matrix4 result = {
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
        };
        return result;
    }

    static matrix4 identity()
    {
        matrix4 result = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
        return result;
    }
};


INLINE matrix4 & operator += (matrix4 & a, matrix4 b)
{
    a._11 += b._11; a._12 += b._12; a._13 += b._13; a._14 += b._14;
    a._21 += b._21; a._22 += b._22; a._23 += b._23; a._24 += b._24;
    a._31 += b._31; a._32 += b._32; a._33 += b._33; a._34 += b._34;
    a._41 += b._41; a._42 += b._42; a._43 += b._43; a._44 += b._44;
    return a;
}

INLINE matrix4 &operator -= (matrix4 &a, matrix4 b)
{
    a._11 -= b._11; a._12 -= b._12; a._13 -= b._13; a._14 -= b._14;
    a._21 -= b._21; a._22 -= b._22; a._23 -= b._23; a._24 -= b._24;
    a._31 -= b._31; a._32 -= b._32; a._33 -= b._33; a._34 -= b._34;
    a._41 -= b._41; a._42 -= b._42; a._43 -= b._43; a._44 -= b._44;
    return a;
}

INLINE matrix4 &operator *= (matrix4 &a, float32 c)
{
    a._11 *= c; a._12 *= c; a._13 *= c; a._14 *= c;
    a._21 *= c; a._22 *= c; a._23 *= c; a._24 *= c;
    a._31 *= c; a._32 *= c; a._33 *= c; a._34 *= c;
    a._41 *= c; a._42 *= c; a._43 *= c; a._44 *= c;
    return a;
}

INLINE matrix4 operator - (matrix4 a) {
    matrix4 result = {
        -a._11, -a._12, -a._13, -a._14,
        -a._21, -a._22, -a._23, -a._24,
        -a._31, -a._32, -a._33, -a._34,
        -a._41, -a._42, -a._43, -a._44,
    };
    return result;
}

INLINE matrix4 operator + (matrix4 a, matrix4 b) {
    matrix4 result = {
        a._11 + b._11, a._12 + b._12, a._13 + b._13, a._14 + b._14,
        a._21 + b._21, a._22 + b._22, a._23 + b._23, a._24 + b._24,
        a._31 + b._31, a._32 + b._32, a._33 + b._33, a._34 + b._34,
        a._41 + b._41, a._42 + b._42, a._43 + b._43, a._44 + b._44,
    };
    return result;
}

INLINE matrix4 operator - (matrix4 a, matrix4 b) {
    matrix4 result = {
        a._11 - b._11, a._12 - b._12, a._13 - b._13, a._14 - b._14,
        a._21 - b._21, a._22 - b._22, a._23 - b._23, a._24 - b._24,
        a._31 - b._31, a._32 - b._32, a._33 - b._33, a._34 - b._34,
        a._41 - b._41, a._42 - b._42, a._43 - b._43, a._44 - b._44,
    };
    return result;
}

INLINE matrix4 operator * (matrix4 a, float32 c) {
    matrix4 result = {
        a._11 * c, a._12 * c, a._13 * c, a._14 * c,
        a._21 * c, a._22 * c, a._23 * c, a._24 * c,
        a._31 * c, a._32 * c, a._33 * c, a._34 * c,
        a._41 * c, a._42 * c, a._43 * c, a._44 * c,
    };
    return result;
}

INLINE matrix4 operator * (float32 c, matrix4 a) {
    matrix4 result = {
        a._11 * c, a._12 * c, a._13 * c, a._14 * c,
        a._21 * c, a._22 * c, a._23 * c, a._24 * c,
        a._31 * c, a._32 * c, a._33 * c, a._34 * c,
        a._41 * c, a._42 * c, a._43 * c, a._44 * c,
    };
    return result;
}

INLINE matrix4 operator / (matrix4 a, float32 c) {
    matrix4 result = {
        a._11 / c, a._12 / c, a._13 / c, a._14 / c,
        a._21 / c, a._22 / c, a._23 / c, a._24 / c,
        a._31 / c, a._32 / c, a._33 / c, a._34 / c,
        a._41 / c, a._42 / c, a._43 / c, a._44 / c,
    };
    return result;
}

INLINE bool operator == (matrix4 a, matrix4 b) {
    bool result = (a._11 == b._11) && (a._12 == b._12) && (a._13 == b._13) && (a._14 == b._14)
               && (a._21 == b._21) && (a._22 == b._22) && (a._23 == b._23) && (a._24 == b._24)
               && (a._31 == b._31) && (a._32 == b._32) && (a._33 == b._33) && (a._34 == b._34)
               && (a._41 == b._41) && (a._42 == b._42) && (a._43 == b._43) && (a._44 == b._44);
    return result;
}

INLINE bool operator != (matrix4 a, matrix4 b) {
    bool result = !(a == b);
    return result;
}

template <typename T>
matrix4 make_matrix4 (T t)
{
    matrix4 result = {
        t, t, t, t,
        t, t, t, t
        t, t, t, t
        t, t, t, t
    };
    return result;
}

template <typename T11, typename T12, typename T13, typename T14,
          typename T21, typename T22, typename T23, typename T24,
          typename T31, typename T32, typename T33, typename T34,
          typename T41, typename T42, typename T43, typename T44>
matrix4 make_matrix4 (T11 t11, T12 t12, T13 t13, T14 t14,
                      T21 t21, T22 t22, T23 t23, T24 t24,
                      T31 t31, T32 t32, T33 t33, T34 t34,
                      T41 t41, T42 t42, T43 t43, T44 t44)
{
    matrix4 result = {
        (float32) t11, (float32) t12, (float32) t13, (float32) t14,
        (float32) t21, (float32) t22, (float32) t23, (float32) t24,
        (float32) t31, (float32) t32, (float32) t33, (float32) t34,
        (float32) t41, (float32) t42, (float32) t43, (float32) t44,
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


#endif // MATH_MATRIX4_HPP
