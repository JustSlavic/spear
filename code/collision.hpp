#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <base.h>
#include <float32.h>


vector2 closest_point_on_segment_to_point(vector2 a, vector2 b, vector2 p)
{
    auto ab = b - a;
    float32 t = clamp(inner(p - a, ab), 0.f, 1.f);
    return a + ab * t;
}


vector2 closest_point_on_segment_to_point(vector2 a, vector2 b, vector2 p, float32 *t)
{
    auto ab = b - a;
    *t = clamp(inner(p - a, ab), 0.f, 1.f);
    return a + ab * *t;
}


float32 distance_line_line(vector2 p1, vector2 d1, vector2 p2, vector2 d2, float32 *s = NULL, float32 *t = NULL)
{
    float32 result = 0.f;

    auto r = p1 - p2;
    auto a = inner(d1, d1);
    auto b = inner(d1, d2);
    auto c = inner(d1, r);
    auto e = inner(d2, d2);
    auto f = inner(d2, r);

    auto d = a*c - b*b;
    ASSERT(d > 0);

    if (d < EPSILON)
    {
        // Two lines are parallel
        *s = 0.f;
        *t = inner(p1 - p2, d2);
    }
    else
    {
        *s = (b*f - c*e) / d;
        *t = (a*f - b*c) / d;
    }

    auto q1 = p1 + d1 * *s;
    auto q2 = p2 + d2 * *t;
    result = norm(q2 - q1);

    return result;
}


float32 sq_distance_segment_segment(vector2 p1, vector2 q1, vector2 p2, vector2 q2,
                                    float& s, float& t, vector2& c1, vector2& c2)
{
    auto d1 = q1 - p1; // Direction of the 1st segment
    auto d2 = q2 - p2; // Direction of the 2nd segment
    auto r = p1 - p2;
    auto a = inner(d1, d1); // Squared norm of the 1st segment
    auto e = inner(d2, d2); // Squared norm of the 2nd segment
    auto f = inner(d2, r);

    // Check if either or both segments degenerate into points
    if (a < EPSILON && e < EPSILON)
    {
        // Both segments degenerate into points
        s = t = 0.f;
        c1 = p1;
        c2 = p2;
        return inner(c1 - c2, c1 - c2);
    }
    if (a < EPSILON)
    {
        // First segment degenerates into a point
        s = 0.f;
        t = f / e; // s = 0 => t = (b*s + f) / e = f / e
        t = clamp(t, 0, 1);
    }
    else
    {
        auto c = inner(d1, r);
        if (e < EPSILON)
        {
            // Second segment degenerates into a point
            t = 0.f;
            s = clamp(-c / a, 0, 1);
        }
        else
        {
            // The general nondegenerate case starts here
            auto b = inner(d1, d2);
            auto denom = a * e - b * b; // Always nonnegative

            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment s1. Else pick arbitrary s (here 0).
            if (denom != 0.0f)
            {
                s = clamp((b*f - c*e) / denom, 0, 1);
            }
            else
            {
                s = 0.f;
            }

            // Compute point on L2 closest to S1(s) using
            // t = inner((P1 + D1*s) - P2, D2) / inner(D2, D2) = (b*s + f) / e
            t = (b*s + f) / e;

            // If t in [0, 1] done. Else clamp t, recompute s for the new value of t using
            // s = inner((P2 + D2*t) - P1, D1) / inner(D1, D1) = (t*b - c) / a
            if (t < 0.f)
            {
                t = 0.f;
                s = clamp(-c / a, 0, 1);
            }
            else if (t > 1.f)
            {
                t = 1.0f;
                s = clamp((b - c) / a, 0, 1);
            }
        }
    }

    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return inner(c1 - c2, c1 - c2);
}


bool32 test_circle2d_point2d(vector2 p1, float32 r1, vector2 p2)
{
    bool32 result = norm_squared(p2 - p1) - square(r1) < EPSILON;
    return result;
}


bool32 test_circle2d_circle2d(vector2 p1, float32 r1, vector2 p2, float32 r2)
{
    bool32 result = norm_squared(p2 - p1) - square(r1 + r2) < EPSILON;
    return result;
}

bool32 test_ray_sphere(vector2 start, vector2 direction, vector2 center, float32 radius, float32 *r)
{
    vector2 m = start - center;
    float32 b = inner(m, direction);
    float32 c = inner(m, m) - radius * radius;
    // Exit if ray_start outside sphere and direction poin away from sphere
    if (c > 0.f && b > 0.f) return false;

    float32 discriminant = b * b - c;
    // A negative discriminant corresponds to ray missing a sphere
    if (discriminant < 0.f) return false;
    // Ray now found to intersect a sphere, compute smallest t value of intersection
    float32 t = -b-square_root(discriminant);
    // If t is negative, ray started inside sphere so clamp t to zero
    if (t < 0.f) t = 0.f;
    *r = t;
    return true;
}

bool32 test_segment_sphere(vector2 start, vector2 end, vector2 center, float32 radius, float32 *t = NULL)
{
    vector2 m = start - center;
    float32 segment_length = norm(end - start);
    vector2 d = normalized(end - start);
    float32 b = inner(m, d);
    float32 c = inner(m, m) - radius * radius;
    // Exit if ray_start outside sphere and direction poin away from sphere
    if (c > 0.f && b > 0.f) return false;

    float32 discriminant = b * b - c;
    // A negative discriminant corresponds to ray missing a sphere
    if (discriminant < 0.f) return false;
    // Ray now found to intersect a sphere, compute smallest t value of intersection
    float32 t_ = -b - square_root(discriminant);
    // If t is negative, ray started inside sphere so clamp t to zero
    if (t_ < 0.f) t_ = 0.f;
    if (t) *t = t_;
    // If t > |end - start| then intersection is further than end point
    bool32 result = (t_ < segment_length);
    return result;
}


float32 square_distance_point_aabb(vector2 p, math::rectangle2 aabb)
{
    float32 square_distance = 0.f;
    for (uint32 i = 0; i < ARRAY_COUNT(aabb.center.e); i++)
    {
        auto min = top_left(aabb);
        auto max = bottom_right(aabb);

        if (p[i] < min[i]) square_distance += square(min[i] - p[i]);
        if (p[i] > max[i]) square_distance += square(p[i] - max[i]);
    }
    return square_distance;
}


bool32 test_sphere_aabb(vector2 center, float32 radius, math::rectangle2 aabb)
{

    return false;
}


struct collision_data
{
    entity *entity1;
    entity *entity2;
    vector2 point;
    vector2 normal;
    float32 t_in_meters;
};


collision_data no_collision()
{
    collision_data result = {};
    result.t_in_meters = infinity;
    return result;
}


collision_data collide_ray_capsule(vector2 ray1, vector2 ray2,
                                   vector2 cap1, vector2 cap2, float32 cap_radius)
{
    collision_data collision = no_collision();

    float32 t1, t2;
    vector2 c1, c2;
    float32 sq_distance = sq_distance_segment_segment(
        ray1, ray2, cap1, cap2,
        t1, t2, c1, c2);

    if (sq_distance < square(cap_radius))
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
