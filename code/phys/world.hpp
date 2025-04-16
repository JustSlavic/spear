#ifndef PHYS_WORLD
#define PHYS_WORLD

#include <base.h>
#include "rigid_body.hpp"


namespace phys {

/*
    Y[0..2] -> vector3 X
    Y[3..6] -> quaternion Q
    Y[7..9] -> vector3 P
    Y[10..12] -> vector3 L
    Y[13..15] -> vector3 invI0
    Y[16] -> float32 invM
*/
#define PHYS_BODY_DIMENSIONS 17
#define PHYS_BODY_X 0
#define PHYS_BODY_Q 3
#define PHYS_BODY_P 7
#define PHYS_BODY_L 10
#define PHYS_BODY_I 13
#define PHYS_BODY_M 16
#define PHYS_BODY_SIZE (sizeof(float32) * PHYS_BODY_DIMENSIONS)

struct world
{
    float32 *Y0;
    float32 *Y1;
    uint32 capacity;
    uint32 count;

    float32 residual_time;
};

struct handle
{
    uint32 index;
};

handle create_rigid_body(phys::world *);

vector3 get_position(phys::world *world, phys::handle h);
quaternion get_orientation(phys::world *world, phys::handle h);
vector3 get_linear_momentum(phys::world *world, phys::handle h);
vector3 get_angular_momentum(phys::world *world, phys::handle h);
matrix3 get_inertia_tensor(phys::world *world, phys::handle h);
float32 get_mass(phys::world *world, phys::handle h);

void set_position(phys::world *world, phys::handle h, vector3 position);
void set_orientation(phys::world *world, phys::handle h, quaternion orientation);
void set_linear_momentum(phys::world *world, phys::handle h, vector3 linear_momentum);
void set_angular_momentum(phys::world *world, phys::handle h, vector3 angular_momentum);
void set_inertia_tensor(phys::world *world, phys::handle h, vector3 inertia_tensor_diag);
void set_mass(phys::world *world, phys::handle h, float32 mass);

void update_world(phys::world *world, float32 dt);


} // namespace phys

#endif // PHYS_WORLD
