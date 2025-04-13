#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include <base.h>


namespace phys {

struct body
{
    vector3 X;
    quaternion Q;

    vector3 P;
    vector3 L;
    matrix3 I;

    float64 M;
};

} // namespace phys


#endif // RIGID_BODY_HPP
