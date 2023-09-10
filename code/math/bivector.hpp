#ifndef MATH_BIVECTOR_HPP
#define MATH_BIVECTOR_HPP

#include <base.h>
#include <math/vector3.hpp>


namespace math {


struct bivector
{
    float32 _12, _13, _23;
};





float32 outer_product(bivector ab, vector3 c)
{
    float32 result = ab._12 * c._3 - ab._13 * c._2 + ab._23 * c._1;
    return result;
}

float32 outer_product(vector3 a, bivector bc)
{
    float32 result = a._1 * bc._23 - a._2 * bc._13 + a._3 * bc._12;
    return result;
}


} // namespace math

#endif // MATH_BIVECTOR_HPP
