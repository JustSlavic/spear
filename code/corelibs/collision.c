#include "collision.h"


float intersect_ray_aabb(vector3 p, vector3 d, vector3 aabb_min, vector3 aabb_max)
{
    float tmax = infinity;
    float tmin = 0.f;

    int i;
    for (i = 0; i < 3; i++)
    {
        if (fabs(d.e[i]) < eps)
        {
            if (p.e[i] < aabb_min.e[i] || p.e[i] > aabb_max.e[i]) return 0;
        }
        else
        {
            // Compute intersection t value of ray with near and far plane of slab
            float ood = 1.0f / d.e[i];
            float t1 = (aabb_min.e[i]- p.e[i]) * ood;
            float t2 = (aabb_max.e[i]- p.e[i]) * ood;

            // Make t1 be intersection with near plane, t2 with far plane
            if (t1 > t2) { float tmp = t2; t2 = t1; t1 = tmp; }

            // Compute the intersection of slab intersection intervals
            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;
            // Exit with no collision as soon as slab intersection becomes empty
            if (tmin > tmax) return infinity;
        }
    }

    return tmin;
}
