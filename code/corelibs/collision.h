#ifndef _SPEAR_CORELIBS_COLLISION_H
#define _SPEAR_CORELIBS_COLLISION_H

#include <corelibs/base.h>


bool test_aabb_point2f(float aabb_min_x, float aabb_min_y, float aabb_max_x, float aabb_max_y, float px, float py);
float intersect_ray_aabb(vector3 p, vector3 d, vector3 aabb_min, vector3 aabb_max);

void triangle_get_normal(float ax, float ay, float az,
                         float bx, float by, float bz,
                         float cx, float cy, float cz,
                         float *x, float *y, float *z);


#endif // _SPEAR_CORELIBS_COLLISION_H
