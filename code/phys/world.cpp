#include "world.hpp"
#include "ode_solver.hpp"

#define PHYS_DT (1.0f / 100.0f)
#define PHYS_G   0.1f

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
    return result;
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
    world->Y0[index] = mass;
}


#ifdef Ix
#undef Ix
#endif
#define Ix(x, y) (((x-1)*6) + (y-1))


void make_jacobian_nxn(float32 *Y, float32 *J, uint32 stride)
{
    float32 m1 = Y[16];
    float32 x1 = Y[0];
    float32 y1 = Y[1];
    float32 z1 = Y[2];
    float32 vx1 = Y[7] / m1;
    float32 vy1 = Y[8] / m1;
    float32 vz1 = Y[9] / m1;

    float32 m2 = Y[stride + 16];
    float32 x2 = Y[stride + 0];
    float32 y2 = Y[stride + 1];
    float32 z2 = Y[stride + 2];
    float32 vx2 = Y[stride + 7] / m2;
    float32 vy2 = Y[stride + 8] / m2;
    float32 vz2 = Y[stride + 9] / m2;

    float32 x = x2 - x1;
    float32 y = y2 - y1;
    float32 z = z2 - z1;

    float32 denom5 = powf(x*x + y*y + z*z, 5.f/2.f);
    float32 Gm1_d5 = PHYS_G*m1/denom5;
    float32 Gm2_d5 = PHYS_G*m2/denom5;

    // J - row1
    J[Ix(1, 1)] = Gm2_d5 * (2.f*x*x - y*y - z*z);
    J[Ix(1, 2)] = Gm2_d5 * 3.f*x*y;
    J[Ix(1, 3)] = Gm2_d5 * 3.f*x*z;

    J[Ix(1, 4)] = -J[Ix(1, 1)];
    J[Ix(1, 5)] = -J[Ix(1, 2)];
    J[Ix(1, 6)] = -J[Ix(1, 3)];

    // J - row2
    J[Ix(2, 1)] = Gm2_d5 * 3.f*x*y;
    J[Ix(2, 2)] = Gm2_d5 * (-x*x + 2.f*y*y - z*z);
    J[Ix(2, 3)] = Gm2_d5 * 3.f*y*z;

    J[Ix(2, 4)] = -J[Ix(2, 1)];
    J[Ix(2, 5)] = -J[Ix(2, 2)];
    J[Ix(2, 6)] = -J[Ix(2, 3)];

    // J - row3
    J[Ix(3, 1)] = Gm2_d5 * 3.f*x*z;
    J[Ix(3, 2)] = Gm2_d5 * 3.f*y*z;
    J[Ix(3, 3)] = Gm2_d5 * (-x*x - y*y + 2.f*z*z);

    J[Ix(3, 4)] = -J[Ix(3, 1)];
    J[Ix(3, 5)] = -J[Ix(3, 2)];
    J[Ix(3, 6)] = -J[Ix(3, 3)];

    // J - row4
    J[Ix(4, 1)] = Gm1_d5 * (-2.f*x*x + y*y + z*z);
    J[Ix(4, 2)] = Gm1_d5 * (-3.f*x*y);
    J[Ix(4, 3)] = Gm1_d5 * (-3.f*x*z);

    J[Ix(4, 4)] = -J[Ix(4, 1)];
    J[Ix(4, 5)] = -J[Ix(4, 2)];
    J[Ix(4, 6)] = -J[Ix(4, 3)];

    // J - row 5
    J[Ix(5, 1)] = Gm1_d5 * (-3.f*x*y);
    J[Ix(5, 2)] = Gm1_d5 * (x*x - 2.f*y*y + z*z);
    J[Ix(5, 3)] = Gm1_d5 * (-3.f*y*z);

    J[Ix(5, 4)] = -J[Ix(5, 1)];
    J[Ix(5, 5)] = -J[Ix(5, 2)];
    J[Ix(5, 6)] = -J[Ix(5, 3)];

    // J - row 6
    J[Ix(6, 1)] = Gm1_d5 * (-3.f*x*z);
    J[Ix(6, 2)] = Gm1_d5 * (-3.f*y*z);
    J[Ix(6, 3)] = Gm1_d5 * (x*x + y*y - 2.f*z*z);

    J[Ix(6, 4)] = -J[Ix(6, 1)];
    J[Ix(6, 5)] = -J[Ix(6, 2)];
    J[Ix(6, 6)] = -J[Ix(6, 3)];

}

void update_step(float32 *in, float32 *out, uint32 stride, uint32 count, float32 dt)
{
    float32 m1 = in[16];
    float32 x1 = in[0];
    float32 y1 = in[1];
    float32 z1 = in[2];
    float32 vx1 = in[7] / m1;
    float32 vy1 = in[8] / m1;
    float32 vz1 = in[9] / m1;

    float32 m2 = in[stride + 16];
    float32 x2 = in[stride + 0];
    float32 y2 = in[stride + 1];
    float32 z2 = in[stride + 2];
    float32 vx2 = in[stride + 7] / m2;
    float32 vy2 = in[stride + 8] / m2;
    float32 vz2 = in[stride + 9] / m2;

    float32 x = x2 - x1;
    float32 y = y2 - y1;
    float32 z = z2 - z1;

    float32 denom3 = powf(x*x + y*y + z*z, 3.f/2.f);
    float32 Gm1_d3 = PHYS_G*m1/denom3;
    float32 Gm2_d3 = PHYS_G*m2/denom3;

    float32 J[6*6];
    make_jacobian_nxn(in, J, stride);
    float32 I_hJ[6*6];
    for (int i = 1; i <= 6; i++)
        for (int j = 1; j <= 6; j++)
            I_hJ[Ix(i, j)] = ((i == j) ? 1.0f : 0.f) - dt*J[Ix(i, j)];
    float32 F0[6];
    F0[0] = Gm2_d3 * x;
    F0[1] = Gm2_d3 * y;
    F0[2] = Gm2_d3 * z;
    F0[3] = Gm1_d3 * (-x);
    F0[4] = Gm1_d3 * (-y);
    F0[5] = Gm1_d3 * (-z);
    // Initial guess of dY (hF(Y0))
    float32 dY0_[6];
    float32 dY1_[6];

    float32 *dY0 = dY0_;
    float32 *dY1 = dY1_;

    dY0[0] = dt * F0[0];
    dY0[1] = dt * F0[1];
    dY0[2] = dt * F0[2];
    dY0[3] = dt * F0[3];
    dY0[4] = dt * F0[4];
    dY0[5] = dt * F0[5];

    for (int approx_step = 0; approx_step < 3; approx_step++)
    {
        for (int i = 0; i < 6; i++)
        {
            float32 acc = 0.f;
            for (int j = 0; j < 6; j++)
            {
                if (i != j)
                {
                    acc += I_hJ[Ix(i+1, j+1)] * dY0[j];
                    console::print("acc += %f * %f\n", I_hJ[Ix(i+1, j+1)], dY0[j]);
                }
            }

            dY1[i] = (dt*F0[i] - acc) / I_hJ[Ix(i+1, i+1)];
        }

        float32 *tmp = dY0;
        dY0 = dY1;
        dY1 = tmp;
    }

    float32 new_vx1 = vx1 + dY0[0];
    float32 new_vy1 = vy1 + dY0[1];
    float32 new_vz1 = vz1 + dY0[2];

    float32 new_vx2 = vx2 + dY0[3];
    float32 new_vy2 = vy2 + dY0[4];
    float32 new_vz2 = vz2 + dY0[5];

    // P
    out[7] = m1 * new_vx1;
    out[8] = m1 * new_vy1;
    out[9] = m1 * new_vz1;

    out[stride + 7] = m2 * new_vx2;
    out[stride + 8] = m2 * new_vy2;
    out[stride + 9] = m2 * new_vz2;

    // X
    out[0] = in[0]; // x1 + dt * new_vx1;
    out[1] = in[1]; // y1 + dt * new_vy1;
    out[2] = in[2]; // z1 + dt * new_vz1;

    out[stride + 0] = x2 + dt * new_vx2;
    out[stride + 1] = y2 + dt * new_vy2;
    out[stride + 2] = z2 + dt * new_vz2;

    for (uint32 i = 0; i < count; i++)
    {
        quaternion Q;
        Q._1 = in[i*stride + 3];
        Q._e32 = in[i*stride + 4];
        Q._e13 = in[i*stride + 5];
        Q._e21 = in[i*stride + 6];

        vector3 L;
        L._e1 = in[i*stride + 10];
        L._e2 = in[i*stride + 11];
        L._e3 = in[i*stride + 12];

        /*
                    | y^2 + z^2    -yx       -zx    |
            I = int(| -xy       x^2 + z^2    -zy    |)
                    | -xz          -yz    x^2 + y^2 |

                       | b^2 + c^2     0         0     |
            I = 1/12 V |     0     a^2 + c^2     0     |
                       |     0         0     a^2 + b^2 |
        */
        matrix3 invI0 = {};
        invI0._11 = in[i*stride + 13];
        invI0._22 = in[i*stride + 14];
        invI0._33 = in[i*stride + 15];

        vector3 omega = apply_unit_quaternion(Q,
            invI0 * apply_unit_quaternion(conjugated(Q), L));

        Q += 0.5f * quaternion::pure(omega) * Q * dt;
        normalize(Q);

        out[i*stride + 3] = Q._1;
        out[i*stride + 4] = Q._e32;
        out[i*stride + 5] = Q._e13;
        out[i*stride + 6] = Q._e21;

        out[i*stride + 10] = in[i*stride + 10]; // Lx
        out[i*stride + 11] = in[i*stride + 11]; // Ly
        out[i*stride + 12] = in[i*stride + 12]; // Lz
        out[i*stride + 13] = in[i*stride + 13]; // Ixx
        out[i*stride + 14] = in[i*stride + 14]; // Iyy
        out[i*stride + 15] = in[i*stride + 15]; // Izz
        out[i*stride + 16] = in[i*stride + 16]; // m
    }
}


void swap_buffers(phys::world *world)
{
    float32 *tmp = world->Y0;
    world->Y0 = world->Y;
    world->Y = tmp;
}


void update_world(phys::world *world, float32 dt)
{
    world->residual_time += dt;
    while (world->residual_time >= PHYS_DT)
    {
        update_step(world->Y0,
                    world->Y,
                    PHYS_BODY_DIMENSIONS,
                    world->count,
                    PHYS_DT);
        world->residual_time -= PHYS_DT;
        swap_buffers(world);
    }
}

} // namespace phys
