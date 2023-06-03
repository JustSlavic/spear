#ifndef MATH_QUATERNION_HPP
#define MATH_QUATERNION_HPP

#include <math/float32.hpp>
#include <math/vector3.hpp>
#include <math/matrix3.hpp>
#include <math/matrix4.hpp>
#include <math/transform.hpp>


namespace math {


struct quaternion
{
    union
    {
        // Real and three imaginary parts representation
        struct { float32 x, y, z, w; };
        struct { float32 i, j, k, r; };
        // vector3 + scalar representation
        struct { vector3 v; float32 s; };
    };

    static quaternion er()
    {
        quaternion result;
        result.x = 0.f;
        result.y = 0.f;
        result.z = 0.f;
        result.w = 1.f;
        return result;
    }

    static quaternion ei()
    {
        quaternion result;
        result.x = 1.f;
        result.y = 0.f;
        result.z = 0.f;
        result.w = 0.f;
        return result;
    }

    static quaternion ej()
    {
        quaternion result;
        result.x = 0.f;
        result.y = 1.f;
        result.z = 0.f;
        result.w = 0.f;
        return result;
    }

    static quaternion ek()
    {
        quaternion result;
        result.x = 0.f;
        result.y = 0.f;
        result.z = 1.f;
        result.w = 0.f;
        return result;
    }
};


#define Q_1(x) (void)0
#define Q_2(x, y) ::math::make_quaternion(x, y)
#define Q_3(x, y, z) (void)0
#define Q_4(x, y, z, w) ::math::make_quaternion(x, y, z, w)

#define Q(...) MACRO_EXPAND(MACRO_OVERLOAD_4(__VA_ARGS__, Q_4, Q_3, Q_2, Q_1)(__VA_ARGS__))


quaternion make_quaternion(float32 x, float32 y, float32 z, float32 w)
{
    quaternion result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

quaternion make_quaternion(vector3 v, float32 r)
{
    quaternion result;
    result.x = v.x;
    result.y = v.y;
    result.z = v.z;
    result.w = r;
    return result;
}

quaternion make_quaternion(vector3 axis_of_rotation, math::angle angle)
{
    quaternion result;
    result.v = axis_of_rotation * sin(0.5f * angle.radians);
    result.s = cos(0.5f * angle.radians);
    return result;
}

quaternion operator + (quaternion q1, quaternion q2)
{
    quaternion result;
    result.x = q1.x + q2.x;
    result.y = q1.y + q2.y;
    result.z = q1.z + q2.z;
    result.w = q1.w + q2.w;
    return result;
}

quaternion operator - (quaternion q1, quaternion q2)
{
    quaternion result;
    result.x = q1.x - q2.x;
    result.y = q1.y - q2.y;
    result.z = q1.z - q2.z;
    result.w = q1.w - q2.w;
    return result;
}

quaternion operator * (quaternion q, float32 c)
{
    quaternion result;
    result.x = q.x * c;
    result.y = q.y * c;
    result.z = q.z * c;
    result.w = q.w * c;
    return result;
}

quaternion operator * (float32 c, quaternion q)
{
    quaternion result;
    result.x = c * q.x;
    result.y = c * q.y;
    result.z = c * q.z;
    result.w = c * q.w;
    return result;
}

quaternion operator / (quaternion q, float32 c)
{
    quaternion result;
    result.x = q.x / c;
    result.y = q.y / c;
    result.z = q.z / c;
    result.w = q.w / c;
    return result;
}

quaternion operator * (quaternion q1, quaternion q2)
{
    quaternion result;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    return result;
}

INLINE float32 squared_length(quaternion q)
{
    float32 result = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    return result;
}

INLINE float32 length(quaternion q)
{
    float32 result = square_root(squared_length(q));
    return result;
}

matrix3 to_matrix3(quaternion q)
{
    auto s = length(q);

    matrix3 result;

    result._11 = 1.f - 2.f * s * (q.j * q.j + q.k * q.k);
    result._12 =       2.f * s * (q.i * q.j - q.k * q.r);
    result._13 =       2.f * s * (q.i * q.k + q.j * q.r);

    result._21 =       2.f * s * (q.i * q.j + q.k * q.r);
    result._22 = 1.f - 2.f * s * (q.i * q.i + q.k * q.k);
    result._23 =       2.f * s * (q.j * q.k - q.i * q.r);

    result._31 =       2.f * s * (q.i * q.k - q.j * q.r);
    result._32 =       2.f * s * (q.j * q.k + q.i * q.r);
    result._33 = 1.f - 2.f * s * (q.i * q.i + q.j * q.j);

    return result;
}

matrix4 to_matrix4(quaternion q)
{
    auto m3 = to_matrix3(q);

    matrix4 result;
    result._1.xyz = m3._1;
    result._14 = 0.f;
    result._2.xyz = m3._2;
    result._24 = 0.f;
    result._3.xyz = m3._3;
    result._34 = 0.f;
    result._4 = V4(0, 0, 0, 1);
    return result;
}

transform to_transform(quaternion q)
{
    transform result;
    result.matrix = to_matrix3(q);
    result.displacement = V3(0);
    return result;
}

quaternion conjugate(quaternion q)
{
    quaternion result;
    result.x = -q.x;
    result.y = -q.y;
    result.z = -q.z;
    result.w =  q.w;
    return result;
}

quaternion inverse(quaternion q)
{
	quaternion result = conjugate(q) / squared_length(q);
	return result;
}

vector3 rotate_by_unit_quaternion(quaternion q, vector3 v)
{
    quaternion result = (q * Q(v.x, v.y, v.z, 0) * conjugate(q));
    return result.v;
}

vector3 rotate_by_unit_quaternion2(quaternion q, vector3 v)
{
    vector3 result = 2.0f * dot(q.v, v) * q.v + (square(q.s) - dot(q.v, q.v)) * v + 2.0f * q.s * cross(q.v, v);
    return result;
}


} // namespace math


#endif // MATH_QUATERNION_HPP
