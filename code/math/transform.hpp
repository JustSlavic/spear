#ifndef MATH_TRANSFORM_HPP
#define MATH_TRANSFORM_HPP

#include <base.hpp>
#include <math/vector3.hpp>
#include <math/matrix3.hpp>
#include <math/matrix4.hpp>

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

    static transform zero()
    {
        transform result = {};
        return result;
    }

    static transform identity()
    {
        transform result = {};
        result._11 = 1.f;
        result._22 = 1.f;
        result._33 = 1.f;
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

matrix4 to_matrix4(transform tm)
{
    matrix4 result;
    result._1 = V4(tm._1, 0);
    result._2 = V4(tm._2, 0);
    result._3 = V4(tm._3, 0);
    result._4 = V4(tm._4, 1);
    transpose(result);
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
    // 1 0 0 0     11 12 13 14                       11                    12                    13  14=0
    // 0 1 0 0  *  21 22 23 24  =                    21                    22                    23  24=0
    // 0 0 1 0     31 32 33 34                       31                    32                    33  34=0
    // x y z 1     41 42 43 44     11x + 21y + 31z + 41, 12x + 22y + 32z + 42, 13x + 23y + 33z + 43, 44=1

    tm._41 = tm._11*v.x + tm._21*v.y + tm._31*v.z + tm._41;
    tm._42 = tm._12*v.x + tm._22*v.y + tm._32*v.z + tm._42;
    tm._43 = tm._13*v.x + tm._23*v.y + tm._33*v.z + tm._43;
}

transform translated(vector3 v, transform tm)
{
    translate(tm, v);
    return tm;
}

void scale_x(transform& tm, float32 sx)
{
    tm._11 *= sx;
    tm._21 *= sx;
    tm._31 *= sx;
    tm._41 *= sx;
}

transform scaled_x(transform tm, float32 sx)
{
    scale_x(tm, sx);
    return tm;
}

void scale_y(transform& tm, float32 sy)
{
    tm._12 *= sy;
    tm._22 *= sy;
    tm._32 *= sy;
    tm._42 *= sy;
}

transform scaled_y(transform tm, float32 sy)
{
    scale_y(tm, sy);
    return tm;
}

void scale_z(transform& tm, float32 sz)
{
    tm._13 *= sz;
    tm._23 *= sz;
    tm._33 *= sz;
    tm._43 *= sz;
}

transform scaled_z(transform tm, float32 sz)
{
    scale_z(tm, sz);
    return tm;
}

void scale(transform & tm, vector3 v)
{
    // sx  0  0  0     11 12 13 14     sx*11 sx*12 sx*13 sx*14=0
    //  0 sy  0  0  *  21 22 23 24  =  sy*21 sy*22 sy*23 sy*24=0
    //  0  0 sz  0     31 32 33 34     sz*31 sz*32 sz*33 sz*34=0
    //  0  0  0  1     41 42 43 44        41    42    43    44=1

    tm._1 = tm._1 * v.x;
    tm._2 = tm._2 * v.y;
    tm._3 = tm._3 * v.z;
}

transform scaled(vector3 v, transform tm)
{
    scale(tm, v);
    return tm;
}

void rotate_x(transform& tm, float32 radians)
{
    //  1  0  0  0     11 12 13 14           11         12         13         14  = 0
    //  0  c -s  0  *  21 22 23 24  =  21c - 31s  22c - 32s  23c - 33s  24c - 34s = 0
    //  0  s  c  0     31 32 33 34     21s + 31c  22s + 32c  23s + 33c  24s + 34c = 0
    //  0  0  0  1     41 42 43 44           41         42         43         44  = 1

    auto c = math::cos(radians);
    auto s = math::sin(radians);

    auto tm_21 = tm._21 * c - tm._31 * s;
    auto tm_22 = tm._22 * c - tm._32 * s;
    auto tm_23 = tm._23 * c - tm._33 * s;

    auto tm_31 = tm._31 * c + tm._21 * s;
    auto tm_32 = tm._32 * c + tm._22 * s;
    auto tm_33 = tm._33 * c + tm._23 * s;

    tm._21 = tm_21;
    tm._22 = tm_22;
    tm._23 = tm_23;

    tm._31 = tm_31;
    tm._32 = tm_32;
    tm._33 = tm_33;
}

transform rotated_x(float32 radians, transform tm)
{
    rotate_x(tm, radians);
    return tm;
}

void rotate_y(transform& tm, float32 radians)
{
    //  c  0 -s  0     11 12 13 14     11c - 31s  12c - 32s  13c - 33s  14c - 24s = 0
    //  0  1  0  0  *  21 22 23 24  =        21         22         23         24  = 0
    //  s  0  c  0     31 32 33 34     11s + 31c  12s + 32c  13s + 33c  14s + 34c = 0
    //  0  0  0  1     41 42 43 44           41         42         43         44  = 1

    auto c = math::cos(radians);
    auto s = math::sin(radians);

    auto tm_11 = tm._11 * c - tm._31 * s;
    auto tm_12 = tm._12 * c - tm._32 * s;
    auto tm_13 = tm._13 * c - tm._33 * s;

    auto tm_31 = tm._31 * c + tm._11 * s;
    auto tm_32 = tm._32 * c + tm._12 * s;
    auto tm_33 = tm._33 * c + tm._13 * s;

    tm._11 = tm_11;
    tm._12 = tm_12;
    tm._13 = tm_13;

    tm._31 = tm_31;
    tm._32 = tm_32;
    tm._33 = tm_33;
}

transform rotated_y(float32 radians, transform tm)
{
    rotate_y(tm, radians);
    return tm;
}

void rotate_z(transform & tm, float32 radians)
{
    //  c -s  0  0     11 12 13 14     11c - 21s  12c - 22s  13c - 23s  14c - 24s = 0
    //  s  c  0  0  *  21 22 23 24  =  11s + 21c  12s + 22c  13s + 23c  14s + 24c = 0
    //  0  0  1  0     31 32 33 34           31         32         33         34  = 0
    //  0  0  0  1     41 42 43 44           41         42         43         44  = 1

    auto c = math::cos(radians);
    auto s = math::sin(radians);

    auto tm_11 = tm._11 * c - tm._21 * s;
    auto tm_12 = tm._12 * c - tm._22 * s;
    auto tm_13 = tm._13 * c - tm._23 * s;

    auto tm_21 = tm._21 * c + tm._11 * s;
    auto tm_22 = tm._22 * c + tm._12 * s;
    auto tm_23 = tm._23 * c + tm._13 * s;

    tm._11 = tm_11;
    tm._12 = tm_12;
    tm._13 = tm_13;

    tm._21 = tm_21;
    tm._22 = tm_22;
    tm._23 = tm_23;
}

transform rotated_z(float32 radians, transform tm)
{
    rotate_z(tm, radians);
    return tm;
}

float32 determinant(transform tm)
{
    float32 result = determinant(tm.matrix);
    return result;
}

transform inverse(transform tm)
{
    transform result = transform::zero();

    float32 det = determinant(tm.matrix);
    if (!near_zero(det))
    {
        result.matrix = (1.0f / det) * adjoint(tm.matrix);

        float32 m_32_43_33_42 = (tm._32 * tm._43 - tm._33 * tm._42);
        float32 m_31_43_33_41 = (tm._31 * tm._43 - tm._33 * tm._41);
        float32 m_31_42_32_41 = (tm._31 * tm._42 - tm._32 * tm._41);
        float32 m_12_23_13_22 = (tm._12 * tm._23 - tm._13 * tm._22);
        float32 m_11_23_13_21 = (tm._11 * tm._23 - tm._13 * tm._21);
        float32 m_11_22_12_21 = (tm._11 * tm._22 - tm._12 * tm._21);

        result._41 = (- tm._21 * m_32_43_33_42 + tm._22 * m_31_43_33_41 - tm._23 * m_31_42_32_41) / det;
        result._42 = (  tm._11 * m_32_43_33_42 - tm._12 * m_31_43_33_41 + tm._13 * m_31_42_32_41) / det;
        result._43 = (- tm._41 * m_12_23_13_22 + tm._42 * m_11_23_13_21 - tm._43 * m_11_22_12_21) / det;
    }
    return result;
}


} // namespace math

#endif // MATH_TRANSFORM_HPP
