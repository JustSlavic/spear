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
        struct
        {
            float32 _11, _12, _13;
            float32 _21, _22, _23;
            float32 _31, _32, _33;
        };
        struct
        {
            vector3 _1, _2, _3;
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
        matrix3 result = {};
        return result;
    }

    static matrix3 identity()
    {
        matrix3 result = {};
        result._11 = 1.f;
        result._22 = 1.f;
        result._33 = 1.f;
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
    matrix3 result;
    result._11 = -a._11; result._12 = -a._12; result._13 = -a._13;
    result._21 = -a._21; result._22 = -a._22; result._23 = -a._23;
    result._31 = -a._31; result._32 = -a._32; result._33 = -a._33;
    return result;
}

INLINE matrix3 operator + (matrix3 a, matrix3 b) {
    matrix3 result;
    result._11 = a._11 + b._11; result._12 = a._12 + b._12; result._13 = a._13 + b._13;
    result._21 = a._21 + b._21; result._22 = a._22 + b._22; result._23 = a._23 + b._23;
    result._31 = a._31 + b._31; result._32 = a._32 + b._32; result._33 = a._33 + b._33;
    return result;
}

INLINE matrix3 operator - (matrix3 a, matrix3 b) {
    matrix3 result;
    result._11 = a._11 - b._11; result._12 = a._12 - b._12; result._13 = a._13 - b._13;
    result._21 = a._21 - b._21; result._22 = a._22 - b._22; result._23 = a._23 - b._23;
    result._31 = a._31 - b._31; result._32 = a._32 - b._32; result._33 = a._33 - b._33;
    return result;
}

INLINE matrix3 operator * (matrix3 a, float32 c) {
    matrix3 result;
    result._11 = a._11 * c; result._12 = a._12 * c; result._13 = a._13 * c;
    result._21 = a._21 * c; result._22 = a._22 * c; result._23 = a._23 * c;
    result._31 = a._31 * c; result._32 = a._32 * c; result._33 = a._33 * c;
    return result;
}

INLINE matrix3 operator * (float32 c, matrix3 a) {
    matrix3 result;
    result._11 = c * a._11; result._12 = c * a._12; result._13 = c * a._13;
    result._21 = c * a._21; result._22 = c * a._22; result._23 = c * a._23;
    result._31 = c * a._31; result._32 = c * a._32; result._33 = c * a._33;
    return result;
}

INLINE matrix3 operator / (matrix3 a, float32 c) {
    matrix3 result;
    result._11 = a._11 / c; result._12 = a._12 / c; result._13 = a._13 / c;
    result._21 = a._21 / c; result._22 = a._22 / c; result._23 = a._23 / c;
    result._31 = a._31 / c; result._32 = a._32 / c; result._33 = a._33 / c;
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
    matrix3 result;
    result._11 = t; result._12 = t; result._13 = t;
    result._21 = t; result._22 = t; result._23 = t;
    result._31 = t; result._32 = t; result._33 = t;
    return result;
}

template <typename T11, typename T12, typename T13,
          typename T21, typename T22, typename T23,
          typename T31, typename T32, typename T33>
matrix3 make_matrix3 (T11 t11, T12 t12, T13 t13,
                      T21 t21, T22 t22, T23 t23,
                      T31 t31, T32 t32, T33 t33)
{
    matrix3 result;
    result._11 = (float32) t11; result._12 = (float32) t12; result._13 = (float32) t13;
    result._21 = (float32) t21; result._22 = (float32) t22; result._23 = (float32) t23;
    result._31 = (float32) t31; result._32 = (float32) t32; result._33 = (float32) t33;
    return result;
}

bool32 equal(matrix3 a, matrix3 b)
{
    bool32 result = equal(a._11, b._11)
                 && equal(a._12, b._12)
                 && equal(a._13, b._13)
                 && equal(a._21, b._21)
                 && equal(a._22, b._22)
                 && equal(a._23, b._23)
                 && equal(a._31, b._31)
                 && equal(a._32, b._32)
                 && equal(a._33, b._33);
    return result;
}

vector3 operator * (matrix3 a, vector3 v)
{
    vector3 result;

    result.x = a._11*v._1 + a._12*v._2 + a._13*v._3;
    result.y = a._21*v._1 + a._22*v._2 + a._23*v._3;
    result.z = a._31*v._1 + a._32*v._2 + a._33*v._3;

    return result;
}

vector3 operator * (vector3 v, matrix3 a)
{
    vector3 result;

    result.x = a._11*v._1 + a._21*v._2 + a._31*v._3;
    result.y = a._12*v._1 + a._22*v._2 + a._32*v._3;
    result.z = a._13*v._1 + a._23*v._2 + a._33*v._3;

    return result;
}

matrix3 operator * (matrix3 a, matrix3 b)
{
    matrix3 result;

    result._11 = a._11*b._11 + a._12*b._21 + a._13*b._31;
    result._12 = a._11*b._12 + a._12*b._22 + a._13*b._32;
    result._13 = a._11*b._13 + a._12*b._23 + a._13*b._33;

    result._21 = a._21*b._11 + a._22*b._21 + a._23*b._31;
    result._22 = a._21*b._12 + a._22*b._22 + a._23*b._32;
    result._23 = a._21*b._13 + a._22*b._23 + a._23*b._33;

    result._31 = a._31*b._11 + a._32*b._21 + a._33*b._31;
    result._32 = a._31*b._12 + a._32*b._22 + a._33*b._32;
    result._33 = a._31*b._13 + a._32*b._23 + a._33*b._33;

    return result;
}

void transpose(matrix3& m)
{
    swap(m._12, m._21);
    swap(m._13, m._31);
    swap(m._23, m._32);
}

matrix3 transposed(matrix3 m)
{
    transpose(m);
    return m;
}

float32 determinant(matrix3 const& m)
{
    float32 result = m._11 * (m._22 * m._33 - m._23 * m._32)
                   - m._12 * (m._21 * m._33 - m._23 * m._31)
                   + m._13 * (m._21 * m._32 - m._22 * m._31);
    return result;
}

matrix3 adjoint(matrix3 const& m)
{
    matrix3 result;

    result._11 =  m._22 * m._33 - m._23 * m._32;
    result._12 = -m._12 * m._33 + m._13 * m._32;
    result._13 =  m._12 * m._23 - m._13 * m._22;

    result._21 = -m._21 * m._33 + m._23 * m._31;
    result._22 =  m._11 * m._33 - m._13 * m._31;
    result._23 = -m._11 * m._23 + m._13 * m._21;

    result._31 =  m._21 * m._32 - m._22 * m._31;
    result._32 = -m._11 * m._32 + m._12 * m._31;
    result._33 =  m._11 * m._22 - m._12 * m._21;

    return result;
}

matrix3 inverse(matrix3 const& m)
{
    matrix3 result = matrix3::zero();

    float32 det = determinant(m);
    if (not near_zero(det))
    {
        result = (1.0f / det) * adjoint(m);
    }

    return result;
}


} // namespace math


#endif // MATH_MATRIX3_HPP
