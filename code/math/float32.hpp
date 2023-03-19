#ifndef MATH_FLOAT32_HPP
#define MATH_FLOAT32_HPP

#include <base.hpp>
#include <math.h>


namespace math
{


GLOBAL constexpr float32 pi = 3.14159265358979323846f;
GLOBAL constexpr float32 infinity = HUGE_VALF;


float32 absolute(float32 x)
{
    float32 result = (x < 0) ? -x : x;
    return result;
}

float32 square(float32 x)
{
    float32 result = x * x;
    return result;
}

float32 square_root(float32 x)
{
    float32 result = sqrtf(x);
    return result;
}

float32 to_radians(float32 degrees)
{
    float32 result = degrees * pi / 180.0f;
    return result;
}

float32 to_degrees(float32 radians)
{
    float32 result = radians * 180.0f / pi;
    return result;
}

float32 sin(float32 x)
{
    float32 result = sinf(x);
    return result;
}

float32 cos(float32 x)
{
    float32 result = cosf(x);
    return result;
}

float32 tg(float32 x)
{
    float32 result = tanf(x);
    return result;
}


} // namespace math


#endif // MATH_FLOAT32_HPP
