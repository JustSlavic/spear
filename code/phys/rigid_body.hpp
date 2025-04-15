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
    vector3 I0;

    float32 M;
};

} // namespace phys


#endif // RIGID_BODY_HPP
