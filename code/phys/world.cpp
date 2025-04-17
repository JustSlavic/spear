#include "world.hpp"
#include "ode_solver.hpp"

#define PHYS_DT (1.0f / 120.0f)


namespace phys {

handle create_rigid_body(phys::world *world)
{
    handle result = { world->count };
    world->count += 1;
    memset(world->Y0 + result.index * PHYS_BODY_DIMENSIONS, 0, PHYS_BODY_SIZE);
    return result;
}

vector3 get_position(phys::world *world, phys::handle h)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_X;
    vector3 result = *(vector3 *) (world->Y0 + index);
    return result;
}

quaternion get_orientation(phys::world *world, phys::handle h)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_Q;
    quaternion result = *(quaternion *) (world->Y0 + index);
    return result;
}

vector3 get_linear_momentum(phys::world *world, phys::handle h)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_P;
    vector3 result = *(vector3 *) (world->Y0 + index);
    return result;
}

vector3 get_angular_momentum(phys::world *world, phys::handle h)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_L;
    vector3 result = *(vector3 *) (world->Y0 + index);
    return result;
}

matrix3 get_inertia_tensor(phys::world *world, phys::handle h)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_I;
    vector3 diagInvI0 = *(vector3 *) (world->Y0 + index);
    matrix3 I = {};
    I._11 = 1.0f / diagInvI0._e1;
    I._22 = 1.0f / diagInvI0._e2;
    I._33 = 1.0f / diagInvI0._e3;
    return I;
}

float32 get_mass(phys::world *world, phys::handle h)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_M;
    float32 result = *(float32 *) (world->Y0 + index);
    return 1.0f / result;
}

void set_position(phys::world *world, phys::handle h, vector3 position)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_X;
    *(vector3 *) (world->Y0 + index) = position;
}

void set_orientation(phys::world *world, phys::handle h, quaternion orientation)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_Q;
    *(quaternion *) (world->Y0 + index) = orientation;
}

void set_linear_momentum(phys::world *world, phys::handle h, vector3 linear_momentum)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_P;
    *(vector3 *) (world->Y0 + index) = linear_momentum;
}

void set_angular_momentum(phys::world *world, phys::handle h, vector3 angular_momentum)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_L;
    *(vector3 *) (world->Y0 + index) = angular_momentum;
}

void set_inertia_tensor(phys::world *world, phys::handle h, vector3 inertia_tensor_diag)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_I;
    world->Y0[index + 0] = 1.0f / inertia_tensor_diag._e1;
    world->Y0[index + 1] = 1.0f / inertia_tensor_diag._e2;
    world->Y0[index + 2] = 1.0f / inertia_tensor_diag._e3;
}

void set_mass(phys::world *world, phys::handle h, float32 mass)
{
    uint32 index = h.index * PHYS_BODY_DIMENSIONS + PHYS_BODY_M;
    world->Y0[index] = 1.0f / mass;
}

/*
            | y^2 + z^2    -yx       -zx    |
    I = int(| -xy       x^2 + z^2    -zy    |)
            | -xz          -yz    x^2 + y^2 |

               | b^2 + c^2     0         0     |
    I = 1/12 V |     0     a^2 + c^2     0     |
               |     0         0     a^2 + b^2 |
*/

void simulate_step_phase_space(float32 *in, float32 *out, float32 dt) {
    /*
        vector3 X;  0,1,2
        quaternion Q; 3,4,5,6

        vector3 P; 7,8,9
        vector3 L; 10,11,12
        vector3 I0; 13,14,15

        float32 M; 16
    */

    quaternion Q;
    Q._1 = in[3];
    Q._e32 = in[4];
    Q._e13 = in[5];
    Q._e21 = in[6];

    vector3 L;
    L._e1 = in[10];
    L._e2 = in[11];
    L._e3 = in[12];

    matrix3 invI0 = {};
    invI0._11 = in[13];
    invI0._22 = in[14];
    invI0._33 = in[15];

    vector3 omega = apply_unit_quaternion(Q,
        invI0 * apply_unit_quaternion(conjugated(Q), L));

    Q += 0.5f * quaternion::pure(omega) * Q * dt;
    normalize(Q);

    // X
    out[0] = in[0] + dt * in[7] * in[16];
    out[1] = in[1] + dt * in[8] * in[16];
    out[2] = in[2] + dt * in[9] * in[16];
    // Q
    out[3] = Q._1;
    out[4] = Q._e32;
    out[5] = Q._e13;
    out[6] = Q._e21;
    // P
    out[7] = in[7]; // Should apply force here
    out[8] = in[8];
    out[9] = in[9];
    // L
    out[10] = in[10]; // Should apply torque here
    out[11] = in[11];
    out[12] = in[12];
    // inv I0
    out[13] = in[13]; // Initial diagonalized
    out[14] = in[14]; // inertia tensor stays
    out[15] = in[15]; // the same.
    // M
    out[16] = in[16]; // Mass stays constant.

    return;
};

void swap_buffers(phys::world *world)
{
    float32 *tmp = world->Y0;
    world->Y0 = world->Y1;
    world->Y1 = tmp;
}

void update_world(phys::world *world, float32 dt)
{
    world->residual_time += dt;
    while (world->residual_time >= PHYS_DT)
    {
        solve_ode(world->Y0,
                  world->Y1,
                  world->count,
                  PHYS_BODY_DIMENSIONS,
                  simulate_step_phase_space,
                  PHYS_DT);
        world->residual_time -= PHYS_DT;
        swap_buffers(world);
    }
}

} // namespace phys
