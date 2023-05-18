#ifndef MATH_TRANSFORM_HPP
#define MATH_TRANSFORM_HPP

#include <base.hpp>
#include <math/vector3.hpp>
#include <math/matrix3.hpp>

namespace math {


/*
    This structure defines row-major 3x3 matrix and displacement vector;
    When vector is transformed with transform, it is treated as row-major matrix, with the displacement;
*/
struct transform
{
    union
    {
        struct
        {
            float32 _11, _12, _13;
            float32 _21, _22, _23;
            float32 _31, _32, _33;
            float32 _41, _42, _43;
        };
        struct
        {
            matrix3 matrix;
            vector3 displacement;
        };
        struct
        {
            vector3 sx; // Vector Ox in the parent (old, previous) coordinated
            vector3 sy; // Vector Oy
            vector3 sz; // Vector Oz
            vector3 sd; // Displacement
        };
        struct
        {
            vector3 _1, _2, _3, _4;
        };
        float32 e[4][3];
        vector3 s[4];
    };

    static transform identity()
    {
        transform result;

        result._11 = 1.f;
        result._12 = 0.f;
        result._13 = 0.f;

        result._21 = 0.f;
        result._22 = 1.f;
        result._23 = 0.f;

        result._31 = 0.f;
        result._32 = 0.f;
        result._33 = 1.f;

        result._41 = 0.f;
        result._42 = 0.f;
        result._43 = 0.f;

        return result;
    }
};

template <typename T>
transform make_transform (T t)
{
    transform result;
    result._11 = t; result._12 = t; result._13 = t;
    result._21 = t; result._22 = t; result._23 = t;
    result._31 = t; result._32 = t; result._33 = t;
    result._41 = t; result._42 = t; result._43 = t;
    return result;
}

INLINE transform make_transform (float32 t11, float32 t12, float32 t13,
                                 float32 t21, float32 t22, float32 t23,
                                 float32 t31, float32 t32, float32 t33,
                                 float32 t41, float32 t42, float32 t43)
{
    transform result;
    result._11 = t11; result._12 = t12; result._13 = t13;
    result._21 = t21; result._22 = t22; result._23 = t23;
    result._31 = t31; result._32 = t32; result._33 = t33;
    result._41 = t41; result._42 = t42; result._43 = t43;
    return result;
}

// This operator is deleted on pupose
vector3 operator * (vector3 v, transform tm) = delete;

vector3 operator * (transform tm, vector3 v)
{
    // This operator only syntactically m*v, actually performing v*m
    vector3 result;
    result.x = tm._11*v._1 + tm._21*v._2 + tm._31*v._3 + tm._41;
    result.y = tm._12*v._1 + tm._22*v._2 + tm._32*v._3 + tm._42;
    result.z = tm._13*v._1 + tm._23*v._2 + tm._33*v._3 + tm._43;
    return result;
}

transform operator * (transform tm1, transform tm2)
{
    transform result;

    result._11 = tm2._11*tm1._11 + tm2._12*tm1._21 + tm2._13*tm1._31;
    result._12 = tm2._11*tm1._12 + tm2._12*tm1._22 + tm2._13*tm1._32;
    result._13 = tm2._11*tm1._13 + tm2._12*tm1._23 + tm2._13*tm1._33;

    result._21 = tm2._21*tm1._11 + tm2._22*tm1._21 + tm2._23*tm1._31;
    result._22 = tm2._21*tm1._12 + tm2._22*tm1._22 + tm2._23*tm1._32;
    result._23 = tm2._21*tm1._13 + tm2._22*tm1._23 + tm2._23*tm1._33;

    result._31 = tm2._31*tm1._11 + tm2._32*tm1._21 + tm2._33*tm1._31;
    result._32 = tm2._31*tm1._12 + tm2._32*tm1._22 + tm2._33*tm1._32;
    result._33 = tm2._31*tm1._13 + tm2._32*tm1._23 + tm2._33*tm1._33;

    result._41 = tm2._41*tm1._11 + tm2._42*tm1._21 + tm2._43*tm1._31 + tm1._41;
    result._42 = tm2._41*tm1._12 + tm2._42*tm1._22 + tm2._43*tm1._32 + tm1._42;
    result._43 = tm2._41*tm1._13 + tm2._42*tm1._23 + tm2._43*tm1._33 + tm1._43;

    return result;
}

INLINE transform& operator *= (transform& tm1, transform const& tm2)
{
    tm1 = tm1 * tm2;
    return tm1;
}

void translate(transform& tm, vector3 v)
{
    tm._4 += v;
}

transform translated(vector3 v, transform tm)
{
    translate(tm, v);
    return tm;
}

void scale_x(transform& tm, float32 sx)
{
    tm._11 *= sx;
}

transform scaled_x(transform tm, float32 sx)
{
    scale_x(tm, sx);
    return tm;
}

void scale_y(transform& tm, float32 sy)
{
    tm._22 *= sy;
}

transform scaled_y(transform tm, float32 sy)
{
    scale_y(tm, sy);
    return tm;
}

void scale_z(transform& tm, float32 sz)
{
    tm._33 *= sz;
}

transform scaled_z(transform tm, float32 sz)
{
    scale_z(tm, sz);
    return tm;
}

void scale(transform & tm, vector3 v)
{
    tm._11 *= v.x;
    tm._22 *= v.y;
    tm._33 *= v.z;
}

transform scaled(vector3 v, transform tm)
{
    scale(tm, v);
    return tm;
}

void rotate_x(transform& tm, float32 radians)
{
    auto c = math::cos(radians);
    auto s = math::sin(radians);
    auto rotation = make_transform(1,  0,  0,
                                   0,  c, -s,
                                   0,  s,  c,
                                   0,  0,  0);
    tm *= rotation;
}

transform rotated_x(float32 radians, transform tm)
{
    rotate_x(tm, radians);
    return tm;
}


} // namespace math

#endif // MATH_TRANSFORM_HPP
