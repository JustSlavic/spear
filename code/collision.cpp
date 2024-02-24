#include "collision.hpp"

template <typename T>
void swap(T & a, T & b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

int intersect_ray_aabb(vector3 p, vector3 d, rectangle3 aabb, float *t, vector3 *q)
{
    float tmax = infinity;
    float tmin = 0.f;

    for (int i = 0; i < 3; i++)
    {
        if (absolute(d[i]) < EPSILON)
        {
            if (p[i] < aabb.min[i] || p[i] > aabb.max[i]) return 0;
        }
        else
        {
            // Compute intersection t value of ray with near and far plane of slab
            float ood = 1.0f / d[i];
            float t1 = (aabb.min[i]- p[i]) * ood;
            float t2 = (aabb.max[i]- p[i]) * ood;

            // Make t1 be intersection with near plane, t2 with far plane
            if (t1 > t2) swap(t1, t2);

            // Compute the intersection of slab intersection intervals
            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;
            // Exit with no collision as soon as slab intersection becomes empty
            if (tmin > tmax) return 0;
        }
    }

    *t = tmin;
    *q = p + d*tmin;
    return 1;
}
