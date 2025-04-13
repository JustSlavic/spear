#ifndef PHYS_VECTOR
#define PHYS_VECTOR

#include <base.h>


namespace phys {


struct vector
{
    union
    {
        struct { float64 x, y, z; };
        struct { float64 _e1, _e2, _e3; };
    };
};

FORCE_INLINE vector & operator += (vector & a, vector b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
FORCE_INLINE vector & operator -= (vector & a, vector b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }
FORCE_INLINE vector & operator *= (vector & a, float64 c) { a.x *= c;   a.y *= c;   a.z *= c;   return a; }
FORCE_INLINE vector & operator /= (vector & a, float64 c) { a.x /= c;   a.y /= c;   a.z /= c;   return a; }

FORCE_INLINE bool operator == (vector a, vector b) { bool result = (a.x == b.x) && (a.y == b.y) && (a.z == b.z); return result; }
FORCE_INLINE bool operator != (vector a, vector b) { bool result = !(a == b); return result; }

FORCE_INLINE vector operator - (vector a) { vector result; result.x = -a.x; result.y = -a.y; result.z = -a.z; return result; }
FORCE_INLINE vector operator + (vector a, vector b) { vector result; result.x = a.x + b.x; result.y = a.y + b.y; result.z = a.z + b.z; return result; }
FORCE_INLINE vector operator - (vector a, vector b) { vector result; result.x = a.x - b.x; result.y = a.y - b.y; result.z = a.z - b.z; return result; }
FORCE_INLINE vector operator * (vector a, float64 c) { vector result; result.x = a.x * c; result.y = a.y * c; result.z = a.z * c; return result; }
FORCE_INLINE vector operator * (float64 c, vector a) { vector result; result.x = c * a.x; result.y = c * a.y; result.z = c * a.z; return result; }
FORCE_INLINE vector operator / (vector a, float64 c) { vector result; result.x = a.x / c; result.y = a.y / c; result.z = a.z / c; return result; }

FORCE_INLINE vector make_vector(float64 x, float64 y, float64 z) { vector result; result.x = x; result.y = y; result.z = z; return result; }
FORCE_INLINE vector to_vector(vector3 v) { vector result; result.x = v.x; result.y = v.y; result.z = v.z; return result; }
FORCE_INLINE vector3 to_vector3(vector v) { vector3 result; result.x = v.x; result.y = v.y; result.z = v.z; return result; }


} // namespace phys


#endif // PHYS_VECTOR
