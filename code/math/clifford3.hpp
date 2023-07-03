#ifndef MATH_CLIFFORD3_HPP
#define MATH_CLIFFORD3_HPP

#include <base.hpp>
#include <math/vector3.hpp>
#include <math/quaternion.hpp>


namespace math {
namespace ga {


// g3 represents an element in the Clifford G3 algebra
// A = a0 + a1*e1 + a2*e2 + a3*e3 + a4*e1e2 + a5*e2e3 + a6*e3e1 + a7*e1e2e3
struct g3
{
    struct bivector
    {
        float32 _12, _23, _31;

        static bivector outer(vector3 a, vector3 b);
    };

    float32 _0;
    union
    {
        struct { float32 _1, _2, _3; };
        vector3 V;
    };
    union
    {
        struct { float32 _4, _5, _6; };
        bivector B;
    };
    float32 _7;

    static g3 e1;
    static g3 e2;
    static g3 e3;
    static g3 e1e2;
    static g3 e2e3;
    static g3 e3e1;
    static g3 e1e2e3;

    static g3 B1;
    static g3 B2;
    static g3 B3;

    static g3 I;

    static g3 from_quaternion(quaternion q)
    {
        g3 result;
        result._0 = q.w;
        result._4 = q.z;
        result._5 = q.x;
        result._6 = q.y;

        return result;
    }
};

g3 g3::e1     = []{ g3 result = {0}; result._1 = 1.f; return result; }();
g3 g3::e2     = []{ g3 result = {0}; result._2 = 1.f; return result; }();
g3 g3::e3     = []{ g3 result = {0}; result._3 = 1.f; return result; }();
g3 g3::e1e2   = []{ g3 result = {0}; result._4 = 1.f; return result; }();
g3 g3::e2e3   = []{ g3 result = {0}; result._5 = 1.f; return result; }();
g3 g3::e3e1   = []{ g3 result = {0}; result._6 = 1.f; return result; }();
g3 g3::e1e2e3 = []{ g3 result = {0}; result._7 = 1.f; return result; }();

g3 g3::B1     = []{ g3 result = {0}; result._5 = 1.f; return result; }();
g3 g3::B2     = []{ g3 result = {0}; result._6 = 1.f; return result; }();
g3 g3::B3     = []{ g3 result = {0}; result._4 = 1.f; return result; }();
g3 g3::I      = []{ g3 result = {0}; result._7 = 1.f; return result; }();


g3 operator + (float32 x, g3 a)
{
    g3 result = a;
    result._0 = a._0 + x;

    return result;
}

g3 operator + (g3 a, float32 x)
{
    g3 result = x + a;
    return result;
}

g3 operator - (float32 x, g3 a)
{
    g3 result = a;
    result._0 = a._0 - x;

    return result;
}

g3 operator - (g3 a, float32 x)
{
    g3 result = x - a;
    return result;
}


g3 operator + (g3 a, g3 b)
{
    g3 result;
    result._0 = a._0 + b._0;
    result._1 = a._1 + b._1;
    result._2 = a._2 + b._2;
    result._3 = a._3 + b._3;
    result._4 = a._4 + b._4;
    result._5 = a._5 + b._5;
    result._6 = a._6 + b._6;
    result._7 = a._7 + b._7;

    return result;
}

g3 operator - (g3 a, g3 b)
{
    g3 result;
    result._0 = a._0 - b._0;
    result._1 = a._1 - b._1;
    result._2 = a._2 - b._2;
    result._3 = a._3 - b._3;
    result._4 = a._4 - b._4;
    result._5 = a._5 - b._5;
    result._6 = a._6 - b._6;
    result._7 = a._7 - b._7;

    return result;
}

g3 operator - (g3 a)
{
    g3 result;
    result._0 = -a._0;
    result._1 = -a._1;
    result._2 = -a._2;
    result._3 = -a._3;
    result._4 = -a._4;
    result._5 = -a._5;
    result._6 = -a._6;
    result._7 = -a._7;

    return result;
}

g3 operator * (float32 x, g3 a)
{
    g3 result;
    result._0 = x * a._0;
    result._1 = x * a._1;
    result._2 = x * a._2;
    result._3 = x * a._3;
    result._4 = x * a._4;
    result._5 = x * a._5;
    result._6 = x * a._6;
    result._7 = x * a._7;

    return result;
}

g3 operator * (g3 a, float32 x)
{
    return x * a;
}

g3 operator * (g3 a, g3 b)
{
    // 8*8 multiplications
    // 7*8 additions
    g3 result;
    result._0 = a._0*b._0 + a._1*b._1 + a._2*b._2 + a._3*b._3 - a._4*b._4 - a._5*b._5 - a._6*b._6 - a._7*b._7;
    result._1 = a._0*b._1 + a._1*b._0 - a._2*b._4 + a._3*b._6 + a._4*b._2 - a._5*b._7 - a._6*b._3 - a._7*b._5;
    result._2 = a._0*b._2 + a._1*b._4 + a._2*b._0 - a._3*b._5 - a._4*b._1 + a._5*b._3 - a._6*b._7 - a._7*b._6;
    result._3 = a._0*b._3 - a._1*b._6 + a._2*b._5 + a._3*b._0 - a._4*b._7 - a._5*b._2 + a._6*b._1 - a._7*b._4;
    result._4 = a._0*b._4 + a._1*b._2 - a._2*b._1 + a._3*b._7 + a._4*b._6 - a._5*b._6 + a._6*b._5 + a._7*b._3;
    result._5 = a._0*b._5 + a._1*b._7 + a._2*b._3 - a._3*b._2 + a._4*b._6 + a._5*b._0 - a._6*b._4 + a._7*b._1;
    result._6 = a._0*b._6 - a._3*b._3 + a._2*b._7 + a._3*b._1 - a._4*b._5 + a._5*b._4 + a._6*b._0 + a._7*b._2;
    result._7 = a._0*b._7 + a._1*b._5 + a._2*b._6 + a._3*b._4 + a._4*b._3 + a._5*b._1 + a._6*b._2 + a._7*b._0;

    return result;
}

g3 conjugate(g3 a)
{
    g3 result;
    result._0 = a._0;
    result._1 = a._1;
    result._2 = a._2;
    result._3 = a._3;
    result._4 = -a._4;
    result._5 = -a._5;
    result._6 = -a._6;
    result._7 = -a._7;

    return result;
}

g3 operator * (vector3 a, g3 b)
{
    g3 result;
    result._0 = a._1 * b._1 + a._2 * b._2 + a._3 * b._3;
    result._1 = a._1 * b._0 - a._2 * b._4 + a._3 * b._6;
    result._2 = a._1 * b._4 + a._2 * b._0 - a._3 * b._5;
    result._3 = a._3 * b._0 + a._2 * b._5 - a._1 * b._6;
    result._4 = a._1 * b._2 - a._2 * b._1 + a._3 * b._7;
    result._5 = a._2 * b._3 - a._3 * b._2 + a._1 * b._7;
    result._6 = a._3 * b._1 - a._1 * b._3 + a._2 * b._7;
    result._7 = a._1 * b._5 + a._2 * b._6 + a._3 * b._4;

    return result;
}

g3 operator * (g3 a, vector3 b)
{
    g3 result;
    result._0 = a._1 * b._1 + a._2 * b._2 + a._3 * b._3;
    result._1 = a._0 * b._1 + a._4 * b._2 - a._6 * b._3;
    result._2 = a._0 * b._2 - a._4 * b._1 + a._5 * b._3;
    result._3 = a._0 * b._3 - a._5 * b._2 + a._6 * b._1;
    result._4 = a._1 * b._2 - a._2 * b._1 + a._7 * b._3;
    result._5 = a._2 * b._3 - a._3 * b._2 + a._7 * b._1;
    result._6 = a._3 * b._1 - a._1 * b._3 + a._7 * b._2;
    result._7 = a._4 * b._3 + a._5 * b._1 + a._6 * b._2;

    return result;
}

g3::bivector g3::bivector::outer(vector3 a, vector3 b)
{
    // A = a1e1 + a2e2 + a3e3
    // B = b1e1 + b2e2 + b3e3
    // A/\B = (a1e1 + a2e2 + a3e3) /\ (b1e1 + b2e2 + b3e3) =
    //      = a1b1 e1e1 + a1b2 e1e2 + a1b3 e1e3
    //      + a2b1 e2e1 + a2b2 e2e2 + a2b3 e2e3
    //      + a3b1 e3e1 + a3b2 e3e2 + a3b3 e3e3
    // A/\B = (a1b2 - a2b1) e1e2 + (a2b3 - a3b2) e2e3 + (a3b1 - a1b3) e3e1
    g3::bivector result = {0};
    result._12 = a._1 * b._2 - a._2 * b._1;
    result._23 = a._2 * b._3 - a._3 * b._2;
    result._31 = a._3 * b._1 - a._1 * b._3;

    return result;
}

g3 outer(vector3 a, vector3 b)
{
    g3 result = {0};
    result.B = g3::bivector::outer(a, b);

    return result;
}

g3 operator * (vector3 a, vector3 b)
{
    g3 result = inner(a, b) + outer(a, b);
    return result;
}

vector3 to_vector3(g3 a)
{
    vector3 result;
    result._1 = a._1;
    result._2 = a._2;
    result._3 = a._3;

    return result;
}

vector3 to_vector3(g3::bivector b)
{
    vector3 result;
    result.x = b._23;
    result.y = b._31;
    result.z = b._12;

    return result;
}

quaternion to_quaternion(g3 a)
{
    quaternion result;
    result.x = a._5;
    result.y = a._6;
    result.z = a._4;
    result.w = a._0;

    return result;
}


} // namespace ga
} // namespace math

#endif // MATH_CLIFFORD3_HPP
