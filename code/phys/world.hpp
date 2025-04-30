#ifndef PHYS_WORLD
#define PHYS_WORLD

#include <base.h>
#include <memory/buffer.hpp>


namespace phys {

/*
    Y[0] -> float32 invM
    Y[1..3] -> vector3 X
    Y[4..7] -> quaternion Q
    Y[8..10] -> vector3 P
    Y[11..13] -> vector3 L
    Y[14..16] -> vector3 invI0
*/
#define PHYS_BODY_DIMENSIONS 17
#define PHYS_BODY_SIZE (sizeof(float32) * PHYS_BODY_DIMENSIONS)

struct rigid_body
{
    float32 M;
    vector3 X;
    vector3 P;
    quaternion Q;
    vector3 L;
    vector3 I;
};

static_assert(sizeof(rigid_body) == PHYS_BODY_SIZE);

struct world
{
    memory_buffer memory;
    rigid_body *Y;
    rigid_body *Y0;
    uint32 capacity;
    uint32 count;

    float32 residual_time;
};

struct handle
{
    uint32 index;
};

handle create_rigid_body(phys::world *);
rigid_body *get_rigid_body(phys::world *world, handle h);

void update_world(phys::world *world, float32 dt);


} // namespace phys

#endif // PHYS_WORLD
