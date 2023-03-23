#ifndef MATH_FLOAT64_HPP
#define MATH_FLOAT64_HPP

#include <base.hpp>
#include <math.h>


namespace math
{


GLOBAL constexpr float64 pi64 = 3.14159265358979323846;
GLOBAL constexpr float64 infinity64 = HUGE_VAL;


bool32 finite(float64 x)
{
    bool32 result = (x < infinity64) && (x > -infinity64);
    return result;
}

float64 absolute(float64 x)
{
    float64 result = (x < 0) ? -x : x;
    return result;
}

float64 square(float64 x)
{
    float64 result = x * x;
    return result;
}

float64 square_root(float64 x)
{
    float64 result = sqrt(x);
    return result;
}

float64 to_radians(float64 degrees)
{
    float64 result = degrees * pi / 180.0;
    return result;
}

float64 to_degrees(float64 radians)
{
    float64 result = radians * 180.0 / pi;
    return result;
}

float64 sin(float64 x)
{
    float64 result = ::sin(x);
    return result;
}

float64 cos(float64 x)
{
    float64 result = ::cos(x);
    return result;
}

float64 tan(float64 x)
{
    float64 result = ::tan(x);
    return result;
}


} // namespace math


#endif // MATH_FLOAT64_HPP
