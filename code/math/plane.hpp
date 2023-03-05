#ifndef MATH_PLANE_HPP
#define MATH_PLANE_HPP

#include <base.hpp>


namespace math
{


struct plane
{
    math::vector3 normal;
    float32 d; // d = dot(normal, P) for a given point P on the plane
};

//
// Given three noncollinear points (ordered ccw), compute plane equation
//
plane compute_plane(vector3 a, vector3 b, vector3 c)
{
    plane p;
    p.normal = normalized(cross(b - a, c - a));
    p.d = dot(p.normal, a);
    return p;
}


} // namespace math


#endif // MATH_PLANE_HPP
