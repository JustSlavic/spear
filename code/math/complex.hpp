#ifndef MATH_COMPLEX_HPP
#define MATH_COMPLEX_HPP

#include <base.hpp>
#include <math/float32.hpp>


namespace math {


struct complex
{
    union
    {
        struct { float32 x, y; };
        struct { float32 re, im; };
    };
};


#define C_1(VALUE)  ::math::make_complex(VALUE)
#define C_2(RE, IM) ::math::make_complex(RE, IM)

#define C(...) MACRO_EXPAND(MACRO_OVERLOAD_2(__VA_ARGS__, C_2, C_1)(__VA_ARGS__))


INLINE complex make_complex(float32 value)
{
    complex result;
    result.re = value;
    result.im = value;
    return result;
}

INLINE complex make_complex(float32 re, float32 im)
{
    complex result;
    result.re = re;
    result.im = im;
    return result;
}

INLINE complex operator + (complex a, complex b)
{
    complex result;
    result.re = a.re + b.re;
    result.im = a.im + b.im;
    return result;
}

INLINE complex operator - (complex a, complex b)
{
    complex result;
    result.re = a.re - b.re;
    result.im = a.im - b.im;
    return result;
}

INLINE complex operator * (complex a, float32 c)
{
    complex result;
    result.re = a.re * c;
    result.im = a.im * c;
    return result;
}

INLINE complex operator * (float32 c, complex a)
{
    complex result;
    result.re = c * a.re;
    result.im = c * a.im;
    return result;
}

INLINE complex operator * (complex a, complex b)
{
    complex result;
    result.re = a.re * b.re - a.im * b.im;
    result.im = a.re * b.im + a.im * b.re;
    return result;
}

INLINE complex conjugate(complex a)
{
    complex result;
    result.re = a.re;
    result.im = -a.im;
    return result;
}

INLINE float32 length_squared(complex a)
{
    float32 result = a.re * a.re + a.im * a.im;
    return result;
}

INLINE float32 length(complex a)
{
    float32 result = math::square_root(length_squared(a));
    return result;
}

INLINE complex operator / (complex a, float32 c)
{
    complex result;
    result.re = a.re / c;
    result.im = a.im / c;
    return result;
}

INLINE complex operator / (float32 c, complex b)
{
    // c / (x + iy)
    // c * (x - iy) / (xx + yy)
    complex result = (c * conjugate(b)) / length_squared(b);
    return result;
}

INLINE complex operator / (complex a, complex b)
{
    // (x + iy) / (r + is)
    // (x + iy)(r - is) / (rr + ss)
    complex result = (a * conjugate(b)) / length_squared(b);
    return result;
}

}

#endif // MATH_COMPLEX_HPP
