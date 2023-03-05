#ifndef MATH_PREDICATES_HPP
#define MATH_PREDICATES_HPP

#include <math/vector2.hpp>
#include <math/vector3.hpp>
#include <math/matrix2.hpp>
#include <math/matrix3.hpp>
#include <math/matrix4.hpp>


namespace math
{

//
// if orient2d(A, B, C) > 0, then C lies to the LEFT of the directed line AB (triangle ABC is oriented counter-clockwise);
// if orient2d(A, B, C) < 0, then C lies to the RIGHT of the directed line AB (triangle ABC is oriented clockwise);
// if orient2d(A, B, C) = 0, then three points are collinear.
//
// Value of orient2d is equal to the 2*SignedArea, where SignedArea > 0 if ABC is counter-clockwise, and SignedArea < 0 if ABC is clockwise.
//
float32 orient2d(vector2 a, vector2 b, vector2 c)
{
    float32 result = determinant(make_matrix2(a.x - c.x, a.y - c.y,
                                              b.x - c.x, b.y - c.y));
    return result;
}

//
// if orient3d(A, B, C, D) < 0, then D lies above the supporting plane of triangle ABC (ABC appears counter-clockwise when viewed from D, right-hand rule);
// if orient3d(A, B, C, D) > 0, then D lies below the supporting plane of triangle ABC (ABC appears clockwise when viewed from D, left-hand rule);
// if orient3d(A, B, C, D) = 0, then four points are coplanar.
//
// Value of orient3d corresponds to the 6*SignedVolume of the tetrahedron.
//
float32 orient3d(vector3 a, vector3 b, vector3 c, vector3 d)
{
    float32 result = determinant(make_matrix3(a.x - d.x, a.y - d.y, a.z - d.z,
                                              b.x - d.x, b.y - d.y, b.z - d.z,
                                              c.x - d.x, c.y - d.y, c.z - d.z));
    return result;
}

//
// Let the triangle ABC appear in the counter-clockwise order, as indicated by the orient2d(A, B, C) > 0.
//
// Then, when in_circle2d(A, B, C, D) > 0, D lies inside the circle through the three points A, B, and C;
// if in_circle(A, B, C, D) < 0, then D lies outside the circle;
// if in_circle(A, B, C, D) = 0, then four points are cocircular.
//
// If orient2d(A, B, C) < 0, the result is reversed.
//
float32 in_circle2d(vector2 a, vector2 b, vector2 c, vector2 d)
{
    float32 result = determinant(make_matrix3(a.x - d.x, a.y - d.y, square(a.x - d.x) + square(a.y - d.y),
                                              b.x - d.x, b.y - d.y, square(b.x - d.x) + square(b.y - d.y),
                                              c.x - d.x, c.y - d.y, square(c.x - d.x) + square(c.y - d.y)));
    return result;
}

//
// Let the four points A, B, C, and D be oriented such that orient3d(A, B, C, D) > 0,
//
// Then, when in_sphere(A, B, C, D, E) > 0, then E lies inside the sphere through A, B, C, and D;
// if in_sphere(A, B, C, D, E) < 0, then E lies outside the sphere;
// if in_sphere(A, B, C, D, E) = 0, then five points are cospherical
//
float32 in_sphere3d(vector3 a, vector3 b, vector3 c, vector3 d, vector3 e)
{
    float32 result = determinant(make_matrix4(a.x - e.x, a.y - e.y, a.z - e.z, square(a.x - e.x) + square(a.y - e.y) + square(a.z - e.z),
                                              b.x - e.x, b.y - e.y, b.z - e.z, square(b.x - e.x) + square(b.y - e.y) + square(b.z - e.z),
                                              c.x - e.x, c.y - e.y, c.z - e.z, square(c.x - e.x) + square(c.y - e.y) + square(c.z - e.z),
                                              d.x - e.x, d.y - e.y, d.z - e.z, square(d.x - e.x) + square(d.y - e.y) + square(d.z - e.z)));
    return result;
}


} // namespace math


#endif // MATH_PREDICATES_HPP
