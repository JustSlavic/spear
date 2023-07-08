#ifndef MATH_CLIFFORD3_HPP
#define MATH_CLIFFORD3_HPP

#include <base.hpp>
#include <math/float32.hpp>
#include <math/vector3.hpp>
#include <math/quaternion.hpp>


namespace math {
namespace g3 {


// Vector basis elements
struct _e1 { float32 _1; };
struct _e2 { float32 _2; };
struct _e3 { float32 _3; };
// Plane basis elements
struct _e1e2 { float32 _4; };
struct _e2e3 { float32 _5; };
struct _e3e1 { float32 _6; };
// Pseudoscalar
struct _e1e2e3 { float32 _7; };
// 3-D vectors
struct _e1_e2_e3
{
    union
    {
        struct { float32 x, y, z; };
        struct { float32 r, g, b; };
        struct { float32 _1, _2, _3; };
        float32 e[3];
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
// Quaternions
struct _e0_e1e2_e2e3_e3e1
{
    union
    {
        struct { float32 _0, _4, _5, _6; };
        struct { float32 w, z, x, y; };
        struct { float32 r, k, i, j; };
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
// Full G3 algebra
struct _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 { float32 _0, _1, _2, _3, _4, _5, _6, _7; };

// Basis elements
static _e1 e1 = []{ _e1 r; r._1 = 1.f; return r; }();
static _e2 e2 = []{ _e2 r; r._2 = 1.f; return r; }();
static _e3 e3 = []{ _e3 r; r._3 = 1.f; return r; }();
static _e1e2 e1e2 = []{ _e1e2 r; r._4 = 1.f; return r; }();
static _e2e3 e2e3 = []{ _e2e3 r; r._5 = 1.f; return r; }();
static _e3e1 e3e1 = []{ _e3e1 r; r._6 = 1.f; return r; }();
static _e1e2 K = []{ _e1e2 r; r._4 = 1.f; return r; }();
static _e2e3 I = []{ _e2e3 r; r._5 = 1.f; return r; }();
static _e3e1 J = []{ _e3e1 r; r._6 = 1.f; return r; }();
static _e1e2e3 S = []{ _e1e2e3 r; r._7 = 1.f; return r; }();

// Type aliases
typedef _e1_e2_e3 vector;
typedef _e0_e1e2_e2e3_e3e1 quaternion;

#include "g3_operators.hpp"

// Functions

vector make_vector(float32 value)
{
    vector result;
    result.x = value;
    result.y = value;
    result.z = value;
    return result;
}

vector make_vector(float32 x, float32 y, float32 z)
{
    vector result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

vector to_vector(quaternion q)
{
    vector result;
    result.x = q.x;
    result.y = q.y;
    result.z = q.z;
    return result;
}

vector to_vector(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 g)
{
    vector result;
    result.x = g._1;
    result.y = g._2;
    result.z = g._3;
    return result;
}

quaternion make_quaternion(vector v, float32 r)
{
    quaternion result;
    result.x = v.x;
    result.y = v.y;
    result.z = v.z;
    result.w = r;
    return result;
}

quaternion make_quaternion(vector axis_of_rotation, math::angle angle)
{
    vector v = axis_of_rotation * sin(0.5f * angle.radians);

    quaternion result;
    result.x = v.x;
    result.y = v.y;
    result.z = v.z;
    result.w = cos(0.5f * angle.radians);
    return result;
}


} // namespace g3
} // namespace math

#endif // MATH_CLIFFORD3_HPP
