#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include <base.h>
#include "vector.hpp"
#include "matrix.hpp"


namespace phys {

struct body
{
    vector X;
    matrix R;

    vector P;
    vector L;
    matrix I;

    float64 M;
};



} // namespace phys


#endif // RIGID_BODY_HPP
