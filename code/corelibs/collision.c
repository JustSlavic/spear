#include "collision.h"


bool test_aabb_point2f(float aabb_min_x, float aabb_min_y, float aabb_max_x, float aabb_max_y, float px, float py)
{
    bool result = (aabb_min_x <= px && px <= aabb_max_x)
               && (aabb_min_y <= py && py <= aabb_max_y);
    return result;
}

float intersect_ray_aabb(vector3 p, vector3 d, vector3 aabb_min, vector3 aabb_max)
{
    float tmax = INFINITY;
    float tmin = 0.f;

    int i;
    for (i = 0; i < 3; i++)
    {
        if (fabs(d.e[i]) < EPSILON)
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
            if (tmin > tmax) return INFINITY;
        }
    }

    return tmin;
}


void triangle_get_normal(float ax, float ay, float az,
                         float bx, float by, float bz,
                         float cx, float cy, float cz,
                         float *x, float *y, float *z)
{
    *x = ay * bz - az * by;
    *y = az * bx - ax * bz;
    *z = ax * by - ay * bx;
}
