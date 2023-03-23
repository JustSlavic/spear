#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <base.hpp>
#include <game_state.hpp>


bool32 test_circle2d_point2d(math::vector2 p1, float32 r1, math::vector2 p2)
{
    bool32 result = math::length_squared(p2 - p1) - math::square(r1) < EPSILON;
    return result;
}


bool32 test_circle2d_circle2d(math::vector2 p1, float32 r1, math::vector2 p2, float32 r2)
{
    bool32 result = math::length_squared(p2 - p1) - math::square(r1 + r2) < EPSILON;
    return result;
}

bool32 test_ray_sphere(math::vector2 start, math::vector2 direction, math::vector2 center, float32 radius, float32 *r)
{
    math::vector2 m = start - center;
    float32 b = dot(m, direction);
    float32 c = dot(m, m) - radius * radius;
    // Exit if ray_start outside sphere and direction poin away from sphere
    if (c > 0.f && b > 0.f) return false;

    float32 discriminant = b * b - c;
    // A negative discriminant corresponds to ray missing a sphere
    if (discriminant < 0.f) return false;
    // Ray now found to intersect a sphere, compute smallest t value of intersection
    float32 t = -b-math::square_root(discriminant);
    // If t is negative, ray started inside sphere so clamp t to zero
    if (t < 0.f) t = 0.f;
    *r = t;
    return true;
}

bool32 test_segment_sphere(math::vector2 start, math::vector2 end, math::vector2 center, float32 radius, float32 *t = NULL)
{
    math::vector2 m = start - center;
    float32 segment_length = 0.f;
    math::vector2 d = normalized(end - start, &segment_length);
    float32 b = dot(m, d);
    float32 c = dot(m, m) - radius * radius;
    // Exit if ray_start outside sphere and direction poin away from sphere
    if (c > 0.f && b > 0.f) return false;

    float32 discriminant = b * b - c;
    // A negative discriminant corresponds to ray missing a sphere
    if (discriminant < 0.f) return false;
    // Ray now found to intersect a sphere, compute smallest t value of intersection
    float32 t_ = -b - math::square_root(discriminant);
    // If t is negative, ray started inside sphere so clamp t to zero
    if (t_ < 0.f) t_ = 0.f;
    if (t) *t = t_;
    // If t > |end - start| then intersection is further than end point
    bool32 result = (t_ < segment_length);
    return result;
}


#endif // COLLISION_HPP
