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


vector4 operator * (matrix4 a, vector4 v)
{
    vector4 result;

    result.x = a._11*v._1 + a._12*v._2 + a._13*v._3 + a._14*v._4;
    result.y = a._21*v._1 + a._22*v._2 + a._23*v._3 + a._24*v._4;
    result.z = a._31*v._1 + a._32*v._2 + a._33*v._3 + a._34*v._4;
    result.w = a._41*v._1 + a._42*v._2 + a._43*v._3 + a._44*v._4;

    return result;
}

vector4 operator * (vector4 v, matrix4 a)
{
    vector4 result;

    result.x = a._11*v._1 + a._21*v._2 + a._31*v._3 + a._41*v._4;
    result.y = a._12*v._1 + a._22*v._2 + a._32*v._3 + a._42*v._4;
    result.z = a._13*v._1 + a._23*v._2 + a._33*v._3 + a._43*v._4;
    result.w = a._14*v._1 + a._24*v._2 + a._34*v._3 + a._44*v._4;

    return result;
}

matrix4 operator * (matrix4 a, matrix4 b)
{
    matrix4 result;

    result._11 = a._11*b._11 + a._12*b._21 + a._13*b._31 + a._14*b._41;
    result._12 = a._11*b._12 + a._12*b._22 + a._13*b._32 + a._14*b._42;
    result._13 = a._11*b._13 + a._12*b._23 + a._13*b._33 + a._14*b._43;
    result._14 = a._11*b._14 + a._12*b._24 + a._13*b._34 + a._14*b._44;

    result._21 = a._21*b._11 + a._22*b._21 + a._23*b._31 + a._24*b._41;
    result._22 = a._21*b._12 + a._22*b._22 + a._23*b._32 + a._24*b._42;
    result._23 = a._21*b._13 + a._22*b._23 + a._23*b._33 + a._24*b._43;
    result._24 = a._21*b._14 + a._22*b._24 + a._23*b._34 + a._24*b._44;

    result._31 = a._31*b._11 + a._32*b._21 + a._33*b._31 + a._34*b._41;
    result._32 = a._31*b._12 + a._32*b._22 + a._33*b._32 + a._34*b._42;
    result._33 = a._31*b._13 + a._32*b._23 + a._33*b._33 + a._34*b._43;
    result._34 = a._31*b._14 + a._32*b._24 + a._33*b._34 + a._34*b._44;

    result._41 = a._41*b._11 + a._42*b._21 + a._43*b._31 + a._44*b._41;
    result._42 = a._41*b._12 + a._42*b._22 + a._43*b._32 + a._44*b._42;
    result._43 = a._41*b._13 + a._42*b._23 + a._43*b._33 + a._44*b._43;
    result._44 = a._41*b._14 + a._42*b._24 + a._43*b._34 + a._44*b._44;

    return result;
}

void transpose(matrix4& m)
{
    swap(m._12, m._21);
    swap(m._13, m._31);
    swap(m._14, m._41);
    swap(m._23, m._32);
    swap(m._24, m._42);
    swap(m._34, m._43);
}

matrix4 transposed(matrix4 m)
{
    transpose(m);
    return m;
}

float32 determinant(matrix4 const& m)
{
    float m_33_44_34_43 = (m._33 * m._44 - m._34 * m._43);
    float m_32_43_33_42 = (m._32 * m._43 - m._33 * m._42);
    float m_32_44_34_42 = (m._32 * m._44 - m._34 * m._42);
    float m_31_42_32_41 = (m._31 * m._42 - m._32 * m._41);
    float m_31_43_33_41 = (m._31 * m._43 - m._33 * m._41);
    float m_31_44_34_41 = (m._31 * m._44 - m._34 * m._41);

    float result = m._11 * (m._22 * m_33_44_34_43 - m._23 * m_32_44_34_42 + m._24 * m_32_43_33_42)
                 - m._12 * (m._21 * m_33_44_34_43 - m._23 * m_31_44_34_41 + m._24 * m_31_43_33_41)
                 + m._13 * (m._21 * m_32_44_34_42 - m._22 * m_31_44_34_41 + m._24 * m_31_42_32_41)
                 - m._14 * (m._21 * m_32_43_33_42 - m._22 * m_31_43_33_41 + m._23 * m_31_42_32_41);
    return result;
}


} // namespace math


#endif // MATH_MATRIX4_HPP
