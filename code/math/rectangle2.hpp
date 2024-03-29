#ifndef MATH_RECTANGLE2_HPP
#define MATH_RECTANGLE2_HPP

#include <base.h>
#include <vector2.hpp>


namespace math
{


struct rectangle2
{
    vector2 center;
    vector2 radii;

    static rectangle2 from_min_max(vector2 min, vector2 max)
    {
        rectangle2 result;
        result.center = 0.5f * min + 0.5f * max;
        result.radii = 0.5f * (max - min);

        return result;
    }

    static rectangle2 from_center_half_size(vector2 center, float32 half_width, float32 half_height)
    {
        rectangle2 result;
        result.center = center;
        result.radii = V2(half_width, half_height);

        return result;
    }

    static rectangle2 from_center_size(vector2 center, float32 width, float32 height)
    {
        rectangle2 result = from_center_half_size(center, 0.5f * width, 0.5f * height);
        return result;
    }
};

using rect2 = rectangle2;

vector2 top_left(rectangle2 rect)
{
    vector2 result;
    result.x = rect.center.x - rect.radii.x;
    result.y = rect.center.y - rect.radii.y;
    return result;
}

vector2 top_right(rectangle2 rect)
{
    vector2 result;
    result.x = rect.center.x + rect.radii.x;
    result.y = rect.center.y - rect.radii.y;
    return result;
}

vector2 bottom_left(rectangle2 rect)
{
    vector2 result;
    result.x = rect.center.x - rect.radii.x;
    result.y = rect.center.y + rect.radii.y;
    return result;
}

vector2 bottom_right(rectangle2 rect)
{
    vector2 result;
    result.x = rect.center.x + rect.radii.x;
    result.y = rect.center.y + rect.radii.y;
    return result;
}

vector2 get_min(rectangle2 rect)
{
    vector2 result = top_left(rect);
    return result;
}

vector2 get_max(rectangle2 rect)
{
    vector2 result = bottom_right(rect);
    return result;
}


float32 width(rectangle2 rect)
{
    float32 result = 2.0f * rect.radii.x;
    return result;
}


float height(rectangle2 rect)
{
    float32 result = 2.0f * rect.radii.y;
    return result;
}

bool32 is_inside(rectangle2 a, vector2 p)
{
    bool32 result = ((a.center.x - a.radii.x) <= p.x && p.x <= (a.center.x + a.radii.x)) &&
                     (a.center.y - a.radii.y) <= p.y && p.y <= (a.center.y + a.radii.y);
    return result;
}

} // namespace math


bool32 is_intersecting(math::rectangle2 a, math::rectangle2 b)
{
    if (absolute(a.center.x - b.center.x) > (a.radii.x + b.radii.x)) return false;
    if (absolute(a.center.y - b.center.y) > (a.radii.y + b.radii.y)) return false;
    return true;
}


#endif // MATH_RECTANGLE2_HPP
