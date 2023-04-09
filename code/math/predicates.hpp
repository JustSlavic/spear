#ifndef MATH_PREDICATES_HPP
#define MATH_PREDICATES_HPP

#include <base.hpp>

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
// Then, when in_circle2d(A, B, C, P) > 0, P lies inside the circle through the three points A, B, and C;
// if in_circle(A, B, C, P) < 0, then P lies outside the circle;
// if in_circle(A, B, C, P) = 0, then four points are cocircular.
//
// If orient2d(A, B, C) < 0, the result is reversed.
//
float32 in_circle2d(vector2 a, vector2 b, vector2 c, vector2 p)
{
    matrix3 m = make_matrix3(a.x - p.x, a.y - p.y, square(a.x - p.x) + square(a.y - p.y),
                             b.x - p.x, b.y - p.y, square(b.x - p.x) + square(b.y - p.y),
                             c.x - p.x, c.y - p.y, square(c.x - p.x) + square(c.y - p.y))
    float32 result = determinant(m);
    return result;
}

//
// Let the four points A, B, C, and D be oriented such that orient3d(A, B, C, D) > 0,
//
// Then, when in_sphere(A, B, C, D, P) > 0, then P lies inside the sphere through A, B, C, and D;
// if in_sphere(A, B, C, D, P) < 0, then P lies outside the sphere;
// if in_sphere(A, B, C, D, P) = 0, then five points are cospherical
//
float32 in_sphere3d(vector3 a, vector3 b, vector3 c, vector3 d, vector3 p)
{
    matrix4 m = make_matrix4(a.x - p.x, a.y - p.y, a.z - p.z, square(a.x - p.x) + square(a.y - p.y) + square(a.z - p.z),
                             b.x - p.x, b.y - p.y, b.z - p.z, square(b.x - p.x) + square(b.y - p.y) + square(b.z - p.z),
                             c.x - p.x, c.y - p.y, c.z - p.z, square(c.x - p.x) + square(c.y - p.y) + square(c.z - p.z),
                             d.x - p.x, d.y - p.y, d.z - p.z, square(d.x - p.x) + square(d.y - p.y) + square(d.z - p.z))
    float32 result = determinant(m);
    return result;
}


} // namespace math


#endif // MATH_PREDICATES_HPP
