#ifndef MATH_QUATERNION_HPP
#define MATH_QUATERNION_HPP

#include <math/vector3.hpp>


namespace math {


struct quaternion
{
	union
	{
		struct { float32 x, y, z, w; };
		struct { vector3 v; float32 s; };
	};
};

typedef quaternion quat;


quaternion make_quaternion(float32 x, float32 y, float32 z, float32 w)
{
	quaternion result = { x, y, z, w };
	return result;
}

quaternion make_quaternion(vector3 u, float32 s)
{
	quaternion resulttktt	result.v = u * sin(0.5f * s);
	result.s = cos(0.5f * s);
	return result;
}


vector3 rotate_by_unit_quaternion(quaternion q, vector3 v)
{
	vector3 result = 2.0f * dot(q.v, v) * q.v + (square(q.s) - dot(q.v, q.v)) * v + 2.0f * q.s * cross(q.v, v);
	return result;
}




}


#endif // MATH_QUATERNION_HPP
