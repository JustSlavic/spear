#ifndef PHYS_WORLD
#define PHYS_WORLD

#include <base.h>
#include "rigid_body.hpp"


namespace phys {

struct world
{
    body bodies[32];
    uint32 body_count;

    float32 residual_time;
};

void update_world(phys::world *world, float32 dt);

} // namespace phys

#endif // PHYS_WORLD
