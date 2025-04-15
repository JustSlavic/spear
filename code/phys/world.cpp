#include "world.hpp"
#include "ode_solver.hpp"

#define PHYS_DT (1.0 / 120.0)


namespace phys {

/*
            | y^2 + z^2    -yx       -zx    |
    I = int(| -xy       x^2 + z^2    -zy    |)
            | -xz          -yz    x^2 + y^2 |

               | b^2 + c^2     0         0     |
    I = 1/12 V |     0     a^2 + c^2     0     |
               |     0         0     a^2 + b^2 |
*/

matrix3 make_inertia_tensor(float64 a, float64 b, float64 c)
{
    matrix3 I = {};
    I._11 = b*b + c*c;
    I._22 = a*a + c*c;
    I._33 = a*a + b*b;

    float32 V = a*b*c;
    return (1.0f/12.0f) * V * I;
}

void simulate_step_phase_space(float32 *in, float32 *out, float32 dt) {
    /*
        vector3 X;  0,1,2
        quaternion Q; 3,4,5,6

        vector3 P; 7,8,9
        vector3 L; 10,11,12
        vector3 I0; 13,14,15

        float32 M; 16
    */
    matrix3 I0 = make_inertia_tensor(2.5f, 5.0f, 0.5f);

    matrix3 invI0 = I0;
    invI0._11 = 1.0/invI0._11;
    invI0._22 = 1.0/invI0._22;
    invI0._33 = 1.0/invI0._33;

    quaternion Q;
    Q._1 = in[3];
    Q._e32 = in[4];
    Q._e13 = in[5];
    Q._e21 = in[6];

    vector3 L;
    L._e1 = in[7];
    L._e2 = in[8];
    L._e3 = in[9];

    vector3 omega = apply_unit_quaternion(Q,
        invI0 * apply_unit_quaternion(conjugated(Q), L));

    Q += 0.5f * quaternion::pure(omega) * Q * PHYS_DT;
    normalize(Q);

    // X
    out[0] = dt * in[7] / in[16];
    out[1] = dt * in[8] / in[16];
    out[2] = dt + in[9] / in[16];
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
    // I0
    out[13] = in[13]; // Initial diagonalized
    out[14] = in[14]; // inertia tensor stays
    out[15] = in[15]; // the same.
    // M
    out[16] = in[16]; // Mass stays constant.

    return;
};

void simulate_step(phys::world *world)
{
    for (int body_index = 0; body_index < world->body_count; body_index++)
    {
        body *b = world->bodies + body_index;
        b->X += PHYS_DT * b->P / b->M;

        matrix3 I0 = make_inertia_tensor(2.5f, 5.0f, 0.5f);

        matrix3 invI0 = I0;
        invI0._11 = 1.0/invI0._11;
        invI0._22 = 1.0/invI0._22;
        invI0._33 = 1.0/invI0._33;

        vector3 omega = apply_unit_quaternion(b->Q,
            invI0 * apply_unit_quaternion(conjugated(b->Q), b->L));

        b->Q += 0.5f * quaternion::pure(omega) * b->Q * PHYS_DT;

        normalize(b->Q);
    }
}

void update_world(phys::world *world, float32 dt)
{
    world->residual_time += dt;
    while (world->residual_time >= PHYS_DT)
    {
        simulate_step(world);
        solve_ode(world->Y0,
                  world->Y1,
                  ARRAY_COUNT(world->Y0),
                  17,
                  simulate_step_phase_space,
                  PHYS_DT);
        world->residual_time -= PHYS_DT;
    }
}

} // namespace phys
