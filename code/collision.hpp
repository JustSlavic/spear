#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <base.hpp>


math::vector2 closest_point_on_segment_to_point(math::vector2 a, math::vector2 b, math::vector2 p)
{
    auto ab = b - a;
    float32 t = math::clamp(dot(p - a, ab), 0.f, 1.f);
    return a + ab * t;
}


math::vector2 closest_point_on_segment_to_point(math::vector2 a, math::vector2 b, math::vector2 p, float32 *t)
{
    auto ab = b - a;
    *t = math::clamp(dot(p - a, ab), 0.f, 1.f);
    return a + ab * *t;
}


float32 distance_line_line(math::vector2 p1, math::vector2 d1, math::vector2 p2, math::vector2 d2, float32 *s = NULL, float32 *t = NULL)
{
    float32 result = 0.f;

    auto r = p1 - p2;
    auto a = dot(d1, d1);
    auto b = dot(d1, d2);
    auto c = dot(d1, r);
    auto e = dot(d2, d2);
    auto f = dot(d2, r);

    auto d = a*c - b*b;
    ASSERT(d > 0);

    if (d < EPSILON)
    {
        // Two lines are parallel
        *s = 0.f;
        *t = dot(p1 - p2, d2);
    }
    else
    {
        *s = (b*f - c*e) / d;
        *t = (a*f - b*c) / d;
    }

    auto q1 = p1 + d1 * *s;
    auto q2 = p2 + d2 * *t;
    result = length(q2 - q1);

    return result;
}


float32 sq_distance_segment_segment(math::vector2 p1, math::vector2 q1, math::vector2 p2, math::vector2 q2,
                                    float& s, float& t, math::vector2& c1, math::vector2& c2)
{
    auto d1 = q1 - p1; // Direction of the 1st segment
    auto d2 = q2 - p2; // Direction of the 2nd segment
    auto r = p1 - p2;
    auto a = dot(d1, d1); // Squared length of the 1st segment
    auto e = dot(d2, d2); // Squared length of the 2nd segment
    auto f = dot(d2, r);

    // Check if either or both segments degenerate into points
    if (a < EPSILON && e < EPSILON)
    {
        // Both segments degenerate into points
        s = t = 0.f;
        c1 = p1;
        c2 = p2;
        return dot(c1 - c2, c1 - c2);
    }
    if (a < EPSILON)
    {
        // First segment degenerates into a point
        s = 0.f;
        t = f / e; // s = 0 => t = (b*s + f) / e = f / e
        t = math::clamp(t, 0, 1);
    }
    else
    {
        auto c = dot(d1, r);
        if (e < EPSILON)
        {
            // Second segment degenerates into a point
            t = 0.f;
            s = math::clamp(-c / a, 0, 1);
        }
        else
        {
            // The general nondegenerate case starts here
            auto b = dot(d1, d2);
            auto denom = a * e - b * b; // Always nonnegative

            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment s1. Else pick arbitrary s (here 0).
            if (denom != 0.0f)
            {
                s = math::clamp((b*f - c*e) / denom, 0, 1);
            }
            else
            {
                s = 0.f;
            }

            // Compute point on L2 closest to S1(s) using
            // t = dot((P1 + D1*s) - P2, D2) / Dot(D2, D2) = (b*s + f) / e
            t = (b*s + f) / e;

            // If t in [0, 1] done. Else clamp t, recompute s for the new value of t using
            // s = Dot((P2 + D2*t) - P1, D1) / Dot(D1, D1) = (t*b - c) / a
            if (t < 0.f)
            {
                t = 0.f;
                s = math::clamp(-c / a, 0, 1);
            }
            else if (t > 1.f)
            {
                t = 1.0f;
                s = math::clamp((b - c) / a, 0, 1);
            }
        }
    }

    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return dot(c1 - c2, c1 - c2);
}


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


float32 square_distance_point_aabb(math::vector2 p, math::rectangle2 aabb)
{
    float32 square_distance = 0.f;
    for (uint32 i = 0; i < ARRAY_COUNT(aabb.center.e); i++)
    {
        auto min = top_left(aabb);
        auto max = bottom_right(aabb);

        if (p[i] < min[i]) square_distance += math::square(min[i] - p[i]);
        if (p[i] > max[i]) square_distance += math::square(p[i] - max[i]);
    }
    return square_distance;
}


bool32 test_sphere_aabb(math::vector2 center, float32 radius, math::rectangle2 aabb)
{

    return false;
}


struct collision_data
{
    entity *entity1;
    entity *entity2;
    math::vector2 point;
    math::vector2 normal;
    float32 t_in_meters;
};


collision_data no_collision()
{
    collision_data result = {};
    result.t_in_meters = math::infinity;
    return result;
}


collision_data collide_ray_capsule(math::vector2 ray1, math::vector2 ray2,
                                   math::vector2 cap1, math::vector2 cap2, float32 cap_radius)
{
    collision_data collision = no_collision();

    float32 t1, t2;
    math::vector2 c1, c2;
    float32 sq_distance = sq_distance_segment_segment(
        ray1, ray2, cap1, cap2,
        t1, t2, c1, c2);

    if (sq_distance < math::square(cap_radius))
    {
        float32 r = 0.f;
        bool32 collided = test_segment_sphere(ray1, ray2, c2, cap_radius, &r);
        ASSERT(collided);

        collision.point = ray1 + normalized(ray2 - ray1) * r;
        collision.normal = normalized(c1 - c2);
        collision.t_in_meters = r;
    }

    return collision;
}


#endif // COLLISION_HPP
