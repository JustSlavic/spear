#ifndef MATH_QUATERNION_HPP
#define MATH_QUATERNION_HPP

#include <math/vector3.hpp>


namespace math {


struct quaternion
{
	union
	{
		// Real and three imaginary parts representation
		struct { float32 a, b, c, d; };
		// Scalar and vector3 representation
		struct { float32 s; vector3 v; };
	};

	static quaternion r()
	{
		quaternion result;
		result.a = 1.f;
		result.b = 0.f;
		result.c = 0.f;
		result.d = 0.f;
		return result;
	}

	static quaternion i()
	{
		quaternion result;
		result.a = 0.f;
		result.b = 1.f;
		result.c = 0.f;
		result.d = 0.f;
		return result;
	}

	static quaternion j()
	{
		quaternion result;
		result.a = 0.f;
		result.b = 0.f;
		result.c = 1.f;
		result.d = 0.f;
		return result;
	}

	static quaternion k()
	{
		quaternion result;
		result.a = 0.f;
		result.b = 0.f;
		result.c = 0.f;
		result.d = 1.f;
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
	result.a = x;
	result.b = y;
	result.c = z;
	result.d = w;
	return result;
}

quaternion make_quaternion(float32 r, vector3 v)
{
	quaternion result;
	result.a = r;
	result.b = v.x;
	result.c = v.y;
	result.d = v.z;
	return result;
}

quaternion make_quaternion(vector3 axis_of_rotation, float32 radians)
{
	quaternion result;
	result.s = cos(0.5f * radians);
	result.v = axis_of_rotation * sin(0.5f * radians);
	return result;
}

quaternion conjugate(quaternion q)
{
	quaternion result;
	result.a =  q.a;
	result.b = -q.b;
	result.c = -q.c;
	result.d = -q.d;
	return result;
}

quaternion operator + (quaternion q1, quaternion q2)
{
	quaternion result;
	result.a = q1.a + q2.a;
	result.b = q1.b + q2.b;
	result.c = q1.c + q2.c;
	result.d = q1.d + q2.d;
	return result;
}

quaternion operator * (quaternion q1, quaternion q2)
{
	quaternion result;
	result.a = q1.a * q2.a - q1.b * q2.b - q1.c * q2.c - q1.d * q2.d;
	result.b = q1.a * q2.b + q1.b * q2.a + q1.c * q2.d - q1.d * q2.c;
	result.c = q1.a * q2.c - q1.b * q2.d + q1.c * q2.a + q1.d * q2.b;
	result.d = q1.a * q2.d + q1.b * q2.c - q1.c * q2.b + q1.d * q2.a;
	return result;
}

vector3 rotate_by_quaternion(quaternion q, vector3 v)
{
	quaternion result = (q * Q(0, v.x, v.y, v.z) * conjugate(q));
	return result.v;
}

vector3 rotate_by_unit_quaternion(quaternion q, vector3 v)
{
	vector3 result = 2.0f * dot(q.v, v) * q.v + (square(q.s) - dot(q.v, q.v)) * v + 2.0f * q.s * cross(q.v, v);
	return result;
}


} // namespace math


#endif // MATH_QUATERNION_HPP
