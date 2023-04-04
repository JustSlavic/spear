#ifndef MATH_MATRIX2_HPP
#define MATH_MATRIX2_HPP

#include <base.hpp>
#include <math/vector2.hpp>


namespace math
{


struct matrix2
{
    union
    {
        struct {
            float32 _11, _12;
            float32 _21, _22;
        };
        vector2 row[2];
        float32 e[2][2];
    };

    float32 *data() {
        return &e[0][0];
    }

    float32 const *data() const {
        return &e[0][0];
    }

    static matrix2 zero()
    {
        matrix2 result = {
            0.0f, 0.0f,
            0.0f, 0.0f,
        };
        return result;
    }

    static matrix2 identity()
    {
        matrix2 result = {
            1.0f, 0.0f,
            0.0f, 1.0f,
        };
        return result;
    }
};


INLINE matrix2 & operator += (matrix2 & a, matrix2 b)
{
    a._11 += b._11; a._12 += b._12;
    a._21 += b._21; a._22 += b._22;
    return a;
}

INLINE matrix2 &operator -= (matrix2 &a, matrix2 b)
{
    a._11 -= b._11; a._12 -= b._12;
    a._21 -= b._21; a._22 -= b._22;
    return a;
}

INLINE matrix2 &operator *= (matrix2 &a, float32 c)
{
    a._11 *= c; a._12 *= c;
    a._21 *= c; a._22 *= c;
    return a;
}

INLINE matrix2 operator - (matrix2 a) {
    matrix2 result = {
        -a._11, -a._12,
        -a._21, -a._22,
    };
    return result;
}

INLINE matrix2 operator + (matrix2 a, matrix2 b) {
    matrix2 result = {
        a._11 + b._11, a._12 + b._12,
        a._21 + b._21, a._22 + b._22,
    };
    return result;
}

INLINE matrix2 operator - (matrix2 a, matrix2 b) {
    matrix2 result = {
        a._11 - b._11, a._12 - b._12,
        a._21 - b._21, a._22 - b._22
    };
    return result;
}

INLINE matrix2 operator * (matrix2 a, float32 c) {
    matrix2 result = {
        a._11 * c, a._12 * c,
        a._21 * c, a._22 * c
    };
    return result;
}

INLINE matrix2 operator * (float32 c, matrix2 a) {
    matrix2 result = {
        a._11 * c, a._12 * c,
        a._21 * c, a._22 * c
    };
    return result;
}

INLINE matrix2 operator / (matrix2 a, float32 c) {
    matrix2 result = {
        a._11 / c, a._12 / c,
        a._21 / c, a._22 / c
    };
    return result;
}

INLINE bool operator == (matrix2 a, matrix2 b) {
    bool result = (a._11 == b._11) && (a._12 == b._12)
               && (a._21 == b._21) && (a._22 == b._22);
    return result;
}

INLINE bool operator != (matrix2 a, matrix2 b) {
    bool result = !(a == b);
    return result;
}

template <typename T>
matrix2 make_matrix2 (T t)
{
    matrix2 result = {
        t, t,
        t, t,
    };
    return result;
}

template <typename T11, typename T12,
          typename T21, typename T22>
matrix2 make_matrix2 (T11 t11, T12 t12,
                      T21 t21, T22 t22)
{
    matrix2 result = {
        (float32) t11, (float32) t12,
        (float32) t21, (float32) t22,
    };
    return result;
}

vector2 operator * (matrix2 a, vector2 v)
{
    vector2 result;

    result.x = a._11*v._1 + a._12*v._2;
    result.y = a._21*v._1 + a._22*v._2;

    return result;
}

vector2 operator * (vector2 v, matrix2 a)
{
    vector2 result;

    result.x = a._11*v._1 + a._21*v._2;
    result.y = a._12*v._1 + a._22*v._2;

    return result;
}

matrix2 operator * (matrix2 a, matrix2 b)
{
    matrix2 result;

    result._11 = a._11*b._11 + a._12*b._21;
    result._12 = a._11*b._12 + a._12*b._22;

    result._21 = a._21*b._11 + a._22*b._21;
    result._22 = a._21*b._12 + a._22*b._22;

    return result;
}

void transpose(matrix2& m)
{
    swap(m._12, m._21);
}

matrix2 transposed(matrix2 m)
{
    transpose(m);
    return m;
}

float32 determinant(matrix2 const& m)
{
    float32 result = m._11 * m._22 - m._12 * m._21;
    return result;
}

matrix2 adjoint(matrix2 const& m)
{
    matrix2 result = {
         m._22, -m._12,
        -m._21,  m._11,
    };
    return result;
}

matrix2 inverse(matrix2 const& m)
{
    matrix2 result = matrix2::zero();

    float32 det = determinant(m);
    if (not near_zero(det))
    {
        result = (1.0f / det) * adjoint(m);
    }

    return result;
}

matrix2 rotate_matrix(angle radians)
{
    float32 c = math::cos(radians);
    float32 s = math::sin(radians);
    matrix2 result =
    {
        c, -s,
        s, c,
    };
    return result;
}

vector2 rotated(angle radians, vector2 v)
{
    vector2 result = rotate_matrix(radians) * v;
    return result;
}


} // namespace math


#endif // MATH_MATRIX2_HPP
