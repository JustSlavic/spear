#ifndef MATH_RECTANGLE2_HPP
#define MATH_RECTANGLE2_HPP

#include <base.hpp>
#include <math/vector2.hpp>


namespace math
{


struct rectangle2
{
    vector2 min, max;

    static rectangle2 from_min_max(vector2 min, vector2 max)
    {
        rectangle2 result;
        result.min = min;
        result.max = max;

        return result;
    }

    static rectangle2 from_center_half_size(vector2 center, float32 half_width, float32 half_height)
    {
        rectangle2 result;
        result.min = center - make_vector2(half_width, half_height);
        result.max = center + make_vector2(half_width, half_height);

        return result;
    }

    static rectangle2 from_center_size(vector2 center, float32 width, float32 height)
    {
        rectangle2 result = from_center_half_size(center, 0.5f * width, 0.5f * height);
        return result;
    }
};

using rect2 = rectangle2;


float32 width(rectangle2 rect)
{
    float32 result = rect.max.x - rect.min.x;
    return result;
}


float height(rectangle2 rect)
{
    float32 result = rect.max.y - rect.min.y;
    return result;
}


} // namespace math


#endif // MATH_RECTANGLE2_HPP
