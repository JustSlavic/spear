#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <base.h>
#include <vector3.hpp>
#include <rectangle3.hpp>

// Intersect ray R(t) = p + t*d against AABB a. When intersecting,
// return intersection distance tmin and point q of intersection.
int intersect_ray_aabb(vector3 p, vector3 d, rectangle3 aabb, float *t, vector3 *q);


#endif // COLLISION_HPP
