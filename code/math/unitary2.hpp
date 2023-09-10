#ifndef MATH_SU2_HPP
#define MATH_SU2_HPP

#include <base.h>
#include <math/complex.hpp>

namespace math {


// @todo: rename it to pauli2
struct unitary2
{
    union
    {
        struct
        {
            complex a, b;
            complex c, d;
        };
        struct
        {
            complex _11, _12;
            complex _21, _22;
        };
        complex e[2][2];
    };

    static unitary2 sigma_x()
    {
        unitary2 result;
        result.a = 0.f;
        result.b = 1.f;
        result.c = 1.f;
        result.d = 0.f;
        return result;
    }

    static unitary2 sigma_y()
    {
        unitary2 result;
        result.a =  0.f;
        result.b = -1_i;
        result.c =  1_i;
        result.d =  0.f;
        return result;
    }

    static unitary2 sigma_z()
    {
        unitary2 result;
        result.a =  1.f;
        result.b =  0.f;
        result.c =  0.f;
        result.d = -1.f;
        return result;
    }

    static unitary2 identity()
    {
        unitary2 result;
        result.a = 1.f;
        result.b = 0.f;
        result.c = 0.f;
        result.d = 1.f;
        return result;
    }
};


INLINE unitary2 & operator += (unitary2 & a, unitary2 b)
{
    a._11 += b._11;
    a._12 += b._12;
    a._21 += b._21;
    a._22 += b._22;
    return a;
}

INLINE unitary2 &operator -= (unitary2 &a, unitary2 b)
{
    a._11 -= b._11;
    a._12 -= b._12;
    a._21 -= b._21;
    a._22 -= b._22;
    return a;
}

INLINE unitary2 &operator *= (unitary2 &a, complex c)
{
    a._11 *= c;
    a._12 *= c;
    a._21 *= c;
    a._22 *= c;
    return a;
}

INLINE unitary2 operator - (unitary2 a)
{
    unitary2 result;
    result._11 = -a._11;
    result._12 = -a._12;
    result._21 = -a._21;
    result._22 = -a._22;
    return result;
}

INLINE unitary2 operator + (unitary2 a, unitary2 b)
{
    unitary2 result;
    result._11 = a._11 + b._11;
    result._12 = a._12 + b._12;
    result._21 = a._21 + b._21;
    result._22 = a._22 + b._22;
    return result;
}

INLINE unitary2 operator - (unitary2 a, unitary2 b)
{
    unitary2 result;
    result._11 = a._11 - b._11;
    result._12 = a._12 - b._12;
    result._21 = a._21 - b._21;
    result._22 = a._22 - b._22;
    return result;
}

INLINE unitary2 operator * (unitary2 a, float32 c)
{
    unitary2 result;
    result._11 = a._11 * c;
    result._12 = a._12 * c;
    result._21 = a._21 * c;
    result._22 = a._22 * c;
    return result;
}

INLINE unitary2 operator * (float32 c, unitary2 a)
{
    unitary2 result;
    result._11 = a._11 * c;
    result._12 = a._12 * c;
    result._21 = a._21 * c;
    result._22 = a._22 * c;
    return result;
}

INLINE unitary2 operator / (unitary2 a, float32 c)
{
    unitary2 result;
    result._11 = a._11 / c;
    result._12 = a._12 / c;
    result._21 = a._21 / c;
    result._22 = a._22 / c;
    return result;
}

unitary2 operator * (unitary2 a, unitary2 b)
{
    unitary2 result;

    result._11 = a._11*b._11 + a._12*b._21;
    result._12 = a._11*b._12 + a._12*b._22;

    result._21 = a._21*b._11 + a._22*b._21;
    result._22 = a._21*b._12 + a._22*b._22;

    return result;
}

unitary2 transpose(unitary2 a)
{
    unitary2 result;
    result._11 = a._11;
    result._12 = a._21;
    result._21 = a._12;
    result._22 = a._22;
    return result;
}

unitary2 conjugate(unitary2 a)
{
    unitary2 result;
    result._11 = conjugate(a._11);
    result._12 = conjugate(a._12);
    result._21 = conjugate(a._21);
    result._22 = conjugate(a._22);
    return result;
}

unitary2 hermitian_conjugate(unitary2 a)
{
    unitary2 result = conjugate(transpose(a));
    return result;
}

FORCE_INLINE unitary2 dagger(unitary2 a)
{
    unitary2 result = hermitian_conjugate(a);
    return result;
}

vector3 rotate_by_unitary2(unitary2 plane_of_rotation, float32 radians, vector3 v)
{
    unitary2 pauli_vector;
    pauli_vector._11 = v.z;
    pauli_vector._12 = v.x - v.y * 1_i;
    pauli_vector._21 = v.x + v.y * 1_i;
    pauli_vector._22 = -v.z;

    unitary2 rotation = cos(.5f * radians) * unitary2::identity() - sin(.5f * radians) * plane_of_rotation;
    unitary2 rotated_pauli_vector = rotation * pauli_vector * dagger(rotation);

    return V3(rotated_pauli_vector._21.re,
              rotated_pauli_vector._21.im,
              rotated_pauli_vector._11.re);
}


} // namespace math

#endif // MATH_SU2_HPP
