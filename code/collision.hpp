#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <base.h>
#include <vector3.hpp>
#include <rectangle3.hpp>

// Intersect ray R(t) = p + t*d against AABB a.
// When intersecting, return intersection distance t.
int intersect_ray_aabb(vector3 p, vector3 d, rectangle3 aabb, float *t);

// Intersect ray R(t) = p + t*d against sphere at center = c, and radius r.
// When intersecting, return t value of intersection.
int intersect_ray_sphere(vector3 p, vector3 d, vector3 c, float r, float *t);


#endif // COLLISION_HPP
