#ifndef MATH_QUATERNION_HPP
#define MATH_QUATERNION_HPP

#include <math/vector3.hpp>


namespace math {


struct quaternion
{
	union
	{
		// Real and three imaginary parts representation
		struct { float32 r, i, j, k; };
		// Scalar and vector3 representation
		struct { float32 s; vector3 v; };
	};

	static quaternion ei()
	{
		quaternion result;
		result.r = 0.f;
		result.i = 1.f;
		result.j = 0.f;
		result.k = 0.f;
		return result;
	}

	static quaternion ej()
	{
		quaternion result;
		result.r = 0.f;
		result.i = 0.f;
		result.j = 1.f;
		result.k = 0.f;
		return result;
	}

	static quaternion ek()
	{
		quaternion result;
		result.r = 0.f;
		result.i = 0.f;
		result.j = 0.f;
		result.k = 1.f;
		return result;
	}
};


#define Q_1(x) (void)0
#define Q_2(u, s) (void)0
#define Q_3(x, y, z) (void)0
#define Q_4(x, y, z, w) ::math::make_quaternion(x, y, z, w)

#define Q(...) MACRO_EXPAND(MACRO_OVERLOAD_4(__VA_ARGS__, Q_4, Q_3, Q_2, Q_1)(__VA_ARGS__))


quaternion make_quaternion(float32 x, float32 y, float32 z, float32 w)
{
	quaternion result;
	result.r = x;
	result.i = y;
	result.j = z;
	result.k = w;
	return result;
}

quaternion make_quaternion(vector3 u, float32 s)
{
	quaternion result;
	result.v = u * sin(0.5f * s);
	result.s = cos(0.5f * s);
	return result;
}


vector3 rotate_by_unit_quaternion(quaternion q, vector3 v)
{
	vector3 result = 2.0f * dot(q.v, v) * q.v + (square(q.s) - dot(q.v, q.v)) * v + 2.0f * q.s * cross(q.v, v);
	return result;
}


} // namespace math


#endif // MATH_QUATERNION_HPP
