#ifndef MATH_CLIFFORD2_HPP
#define MATH_CLIFFORD2_HPP

#include <base.hpp>

namespace math {
namespace g2 {

// Basis elements
struct _e1 { float32 _1; };
struct _e2 { float32 _2; };
// Pseudoscalar
struct _e1e2 { float32 _3; };
// 2D-vectors
struct _e1_e2
{
    union
    {
        struct { float32 x, y; };
        struct { float32 u, v; };
        struct { float32 _1, _2; };
        float32 e[2];
    };

    float32& operator [] (uint32 index)
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
};
// Complex numbers
struct _e0_e1e2
{
    union
    {
        struct { float32 _0, _3; };
        struct { float32 re, im; };
        float32 e[2];
    };

    float32& operator [] (uint32 index)
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
};
// Full G2 algebra
struct _e0_e1_e2_e1e2
{
    union
    {
        struct { float32 _0, _1, _2, _3; };
        struct { float32 re,  x,  y, im; };
        float32 e[4];
    };

    float32& operator [] (uint32 index)
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
};

// Basis elements
static _e1 e1 = []{ _e1 r; r._1 = 1.f; return r; }();
static _e2 e2 = []{ _e2 r; r._2 = 1.f; return r; }();
static _e1e2 e1e2 = []{ _e1e2 r; r._3 = 1.f; return r; }();
static _e1e2 I = []{ _e1e2 r; r._3 = 1.f; return r; }();

// Type aliases
typedef float32  real;
typedef _e1_e2   vector2;
typedef _e0_e1e2 complex;
typedef _e0_e1_e2_e1e2 g2;

#define G2_V2(E1, E2) ::math::g2::vector2{E1, E2}
#define G2_C(E0, E3) ::math::g2::complex{E0, E3}
#define G2(E0, E1, E2, E3) ::math::g2::_e0_e1_e2_e1e2{E0, E1, E2, E3}

#include "g2_operators.hpp"

// Functions

vector2 make_vector(float32 value)
{
    vector2 result;
    result.x = value;
    result.y = value;
    return result;
}

vector2 make_vector(float32 x, float32 y)
{
    vector2 result;
    result.x = x;
    result.y = y;
    return result;
}

complex make_complex(float32 value)
{
    complex result;
    result.re = value;
    result.im = 0.f;
    return result;
}

complex make_complex(float32 re, float32 im)
{
    complex result;
    result.re = re;
    result.im = im;
    return result;
}

complex to_complex(vector2 a)
{
    complex result = e1 * a;
    return result;
}

complex to_complex(g2 a)
{
    complex result;
    result._0 = a._0;
    result._3 = a._3;
    return result;
}

vector2 to_vector(complex a)
{
    vector2 result = e1 * a;
    return result;
}

vector2 to_vector(g2 a)
{
    vector2 result;
    result.x = a.x;
    result.y = a.y;
    return result;
}

FORCE_INLINE complex operator / (float32 c, complex b)
{
    // c / (x + iy)
    // c * (x - iy) / (xx + yy)
    complex result = (c * conjugate(b)) / length_squared(b);
    return result;
}

FORCE_INLINE complex operator / (complex a, complex b)
{
    // (x + iy) / (r + is)
    // (x + iy)(r - is) / (rr + ss)
    complex result = (a * conjugate(b)) / length_squared(b);
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
    vector2 result = v - 2.0f*inner(v, n)*n;
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

} // namespace g2
} // namespace math

#endif // MATH_CLIFFORD2_HPP
