#ifndef MATH_CLIFFORD2_HPP
#define MATH_CLIFFORD2_HPP

#include <base.hpp>

namespace math {
namespace ga {


// AB = inner(A, B) + outer(A, B)


//
// g2 is the struct that represents clifford algebra G2,
//      A  =  a0 +  a1 * e1  +  a2 * e2  +  a3 * e1 /\ e2
//
struct g2
{
    float32 _0;
    float32 _1, _2;
    float32 _3;

    struct e1_t {};
    struct e2_t {};
    struct e1e2_t {};

    static e1_t e1;
    static e2_t e2;
    static e1e2_t e1e2;
    static e1e2_t I;
};


g2 operator + (g2 a, g2 b)
{
    g2 result;
    result._0 = a._0 + b._0;
    result._1 = a._1 + b._1;
    result._2 = a._2 + b._2;
    result._3 = a._3 + b._3;

    return result;
}


g2 operator + (g2 a, float32 x)
{
    g2 result;
    result._0 = a._0 + x;
    result._1 = a._1;
    result._2 = a._2;
    result._3 = a._3;

    return result;
}


g2 operator + (float32 x, g2 a)
{
    return a + x;
}


g2 operator * (g2 a, g2 b)
{
    g2 result;
    result._0 = a._0 * b._0 + a._1 * b._1 + a._2 * b._2 - a._3 * b._3;
    result._1 = a._0 * b._1 + a._1 * b._0 + a._3 * b._2 - a._2 * b._3;
    result._2 = a._0 * b._2 + a._2 * b._0 + a._1 * b._3 - a._3 * b._1;
    result._3 = a._0 * b._3 + a._3 * b._0 + a._1 * b._2 - a._2 * b._1;

    return result;
}


// All possible multiplications:

// s * e1 (the e1 * s is the same)
// s * e2 (the e2 * s is the same)
// s * e1e2 (the e1e2 * s is the same)


g2 operator * (g2::e1_t, float32 x)
{
    g2 result;
    result._0 = 0.f;
    result._1 = x;
    result._2 = 0.f;
    result._3 = 0.f;

    return result;
}


g2 operator * (float32 x, g2::e1_t)
{
    return g2::e1 * x;
}


g2 operator * (g2::e2_t, float32 x)
{
    g2 result;
    result._0 = 0.f;
    result._1 = 0.f;
    result._2 = x;
    result._3 = 0.f;

    return result;
}


g2 operator * (float32 x, g2::e2_t)
{
    return g2::e2 * x;
}


g2 operator * (g2::e1e2_t, float32 x)
{
    g2 result;
    result._0 = 0.f;
    result._1 = 0.f;
    result._2 = 0.f;
    result._3 = x;

    return result;
}


g2 operator * (float32 x, g2::e1e2_t)
{
    return g2::e1e2 * x;
}


float32 operator * (g2::e1_t, g2::e1_t)
{
    return 1.f;
}


g2 operator * (g2::e2_t, g2::e2_t)
{
    g2 result;
    result._0 = 1.f;
    result._1 = 0.f;
    result._2 = 0.f;
    result._3 = 0.f;

    return result;
}


g2 operator * (g2::e1e2_t, g2::e1e2_t)
{
    g2 result;
    result._0 = -1.f;
    result._1 =  0.f;
    result._2 =  0.f;
    result._3 =  0.f;

    return result;
}


g2 operator * (g2::e1_t, g2::e2_t)
{
    g2 result;
    result._0 = 0.f;
    result._1 = 0.f;
    result._2 = 0.f;
    result._3 = 1.f;

    return result;
}


g2 operator * (g2::e2_t, g2::e1_t)
{
    g2 result;
    result._0 =  0.f;
    result._1 =  0.f;
    result._2 =  0.f;
    result._3 = -1.f;

    return result;
}


g2 operator * (g2::e1_t, g2::e1e2_t)
{
    g2 result;
    result._0 = 0.f;
    result._1 = 0.f;
    result._2 = 1.f;
    result._3 = 0.f;

    return result;
}


g2 operator * (g2::e1e2_t, g2::e1_t)
{
    g2 result;
    result._0 =  0.f;
    result._1 =  0.f;
    result._2 = -1.f;
    result._3 =  0.f;

    return result;
}


g2 operator * (g2::e2_t, g2::e1e2_t)
{
    g2 result;
    result._0 =  0.f;
    result._1 = -1.f;
    result._2 =  0.f;
    result._3 =  0.f;

    return result;
}


g2 operator * (g2::e1e2_t, g2::e2_t)
{
    g2 result;
    result._0 = 0.f;
    result._1 = 1.f;
    result._2 = 0.f;
    result._3 = 0.f;

    return result;
}


g2 operator * (g2 a, float32 x)
{
    g2 result;
    result._0 = a._0 * x;
    result._1 = a._1 * x;
    result._2 = a._2 * x;
    result._3 = a._3 * x;

    return result;
}


g2 operator * (float32 x, g2 a)
{
    return a * x;
}


g2 operator * (g2::e1_t, g2 a)
{
    // A = a0 + a1*e1 + a2*e2 + a3*e1/\e2
    // e1A = a0*e1 + a1*e1e1 + a2*e1e2 + a3*(e1(e1/\e2))
    //    =>
    // e1e1 = inner(e1,e1) + outer(e1,e1) = 1 + 0
    //    =>
    // e1A = a1 + a0 * e1 + a3*e2 + a2*e1e2
    g2 result;
    result._0 = a._1;
    result._1 = a._0;
    result._2 = a._3;
    result._3 = a._2;

    return result;
}


g2 operator * (g2 a, g2::e1_t)
{
    // A = a0 + a1*e1 + a2*e2 + a3*e1/\e2
    // Ae1 = a0e1 + a1*e1e1 + a2*e2e1    + a3*(e1/\e2)e1
    //              |= a1     |= -a2e1e2   |= -a3*e2
    g2 result;
    result._0 =  a._1;
    result._1 =  a._0;
    result._2 = -a._3;
    result._3 = -a._2;

    return result;
}


g2 operator * (g2::e2_t, g2 a)
{
    // A = a0 + a1*e1 + a2*e2 + a3*e1/\e2
    // e2A = a0*e2 + a1*(e2e1) + a2*(e2e2) + a3(e2e1e2)
    //               |= -a1*e1e2 |= a2       |= -a3*e1
    g2 result;
    result._0 =  a._2;
    result._1 = -a._3;
    result._2 =  a._0;
    result._3 = -a._1;

    return result;
}


g2 operator * (g2 a, g2::e2_t)
{
    // A = a0 + a1*e1 + a2*e2 + a3*e1/\e2
    // Ae2 = a0*e2 + a1*e1e2 + a2 + a3*e1
    g2 result;
    result._0 = a._2;
    result._1 = a._3;
    result._2 = a._0;
    result._3 = a._1;

    return result;
}


g2 operator * (g2::e1e2_t, g2 a)
{
    // A = a0 + a1*e1 + a2*e2 + a3*e1/\e2
    // e1e2A = a0*e1e2 + a1*e1e2e1 + a2*e1e2e2 + a3*e1e2e1e2
    // e1e2A = a0*e1e2 - a1*e2 + a2*e1 - a3
    g2 result;
    result._0 = -a._3;
    result._1 =  a._2;
    result._2 = -a._1;
    result._3 =  a._0;

    return result;
}


g2 operator * (g2 a, g2::e1e2_t)
{
    // A = a0 + a1*e1 + a2*e2 + a3*e1/\e2
    // Ae1e2 = a0*e1e2 + a1*e1e1e2 + a2*e2e1e2 + a3*e1e2e1e2
    // Ae1e2 = a0*e1e2 + a1*e2 - a2*e1 - a3
    g2 result;
    result._0 = -a._3;
    result._1 = -a._2;
    result._2 =  a._1;
    result._3 =  a._0;

    return result;
}


} // namespace ga
} // namespace math

#endif // MATH_CLIFFORD2_HPP
