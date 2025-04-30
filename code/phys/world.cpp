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

rigid_body *get_rigid_body(phys::world *world, handle h)
{
    rigid_body *result = NULL;
    if (h.index < world->count)
    {
        result = world->Y0 + h.index;
    }
    return result;
}


#ifdef Ix
#undef Ix
#endif
#define Ix(x, y) (((x-1)*6) + (y-1))


void make_jacobian_nxn(float32 *Y, float32 *J, uint32 n);
// {
//     float32 m1 = Y[0];
//     float32 x1 = Y[1];
//     float32 y1 = Y[2];
//     float32 z1 = Y[3];
//     float32 vx1 = Y[8] / m1;
//     float32 vy1 = Y[9] / m1;
//     float32 vz1 = Y[10] / m1;

//     float32 m2 = Y[stride + 0];
//     float32 x2 = Y[stride + 1];
//     float32 y2 = Y[stride + 2];
//     float32 z2 = Y[stride + 3];
//     float32 vx2 = Y[stride + 8] / m2;
//     float32 vy2 = Y[stride + 9] / m2;
//     float32 vz2 = Y[stride + 10] / m2;

//     float32 x = x2 - x1;
//     float32 y = y2 - y1;
//     float32 z = z2 - z1;

//     // J - row1
//     J[Ix(1, 1)] = Gm2_d5 * (2.f*x*x - y*y - z*z);
//     J[Ix(1, 2)] = Gm2_d5 * 3.f*x*y;
//     J[Ix(1, 3)] = Gm2_d5 * 3.f*x*z;

//     J[Ix(1, 4)] = -J[Ix(1, 1)];
//     J[Ix(1, 5)] = -J[Ix(1, 2)];
//     J[Ix(1, 6)] = -J[Ix(1, 3)];

//     // J - row2
//     J[Ix(2, 1)] = Gm2_d5 * 3.f*x*y;
//     J[Ix(2, 2)] = Gm2_d5 * (-x*x + 2.f*y*y - z*z);
//     J[Ix(2, 3)] = Gm2_d5 * 3.f*y*z;

//     J[Ix(2, 4)] = -J[Ix(2, 1)];
//     J[Ix(2, 5)] = -J[Ix(2, 2)];
//     J[Ix(2, 6)] = -J[Ix(2, 3)];

//     // J - row3
//     J[Ix(3, 1)] = Gm2_d5 * 3.f*x*z;
//     J[Ix(3, 2)] = Gm2_d5 * 3.f*y*z;
//     J[Ix(3, 3)] = Gm2_d5 * (-x*x - y*y + 2.f*z*z);

//     J[Ix(3, 4)] = -J[Ix(3, 1)];
//     J[Ix(3, 5)] = -J[Ix(3, 2)];
//     J[Ix(3, 6)] = -J[Ix(3, 3)];

//     // J - row4
//     J[Ix(4, 1)] = Gm1_d5 * (-2.f*x*x + y*y + z*z);
//     J[Ix(4, 2)] = Gm1_d5 * (-3.f*x*y);
//     J[Ix(4, 3)] = Gm1_d5 * (-3.f*x*z);

//     J[Ix(4, 4)] = -J[Ix(4, 1)];
//     J[Ix(4, 5)] = -J[Ix(4, 2)];
//     J[Ix(4, 6)] = -J[Ix(4, 3)];

//     // J - row 5
//     J[Ix(5, 1)] = Gm1_d5 * (-3.f*x*y);
//     J[Ix(5, 2)] = Gm1_d5 * (x*x - 2.f*y*y + z*z);
//     J[Ix(5, 3)] = Gm1_d5 * (-3.f*y*z);

//     J[Ix(5, 4)] = -J[Ix(5, 1)];
//     J[Ix(5, 5)] = -J[Ix(5, 2)];
//     J[Ix(5, 6)] = -J[Ix(5, 3)];

//     // J - row 6
//     J[Ix(6, 1)] = Gm1_d5 * (-3.f*x*z);
//     J[Ix(6, 2)] = Gm1_d5 * (-3.f*y*z);
//     J[Ix(6, 3)] = Gm1_d5 * (x*x + y*y - 2.f*z*z);

//     J[Ix(6, 4)] = -J[Ix(6, 1)];
//     J[Ix(6, 5)] = -J[Ix(6, 2)];
//     J[Ix(6, 6)] = -J[Ix(6, 3)];

// }

void update_step(rigid_body *in, rigid_body *out, uint32 count, float32 h)
{
    float32 M1 = in[0].M;
    vector3 X1 = in[0].X;
    vector3 V1 = in[0].P / M1;

    float32 M2 = in[1].M;
    vector3 X2 = in[1].X;
    vector3 V2 = in[1].P / M2;

    vector3 d = X2 - X1;

    float32 denom3 = powf(norm_squared(d), 3.f/2.f);
    float32 Gm1_d3 = PHYS_G*M1/denom3;
    float32 Gm2_d3 = PHYS_G*M2/denom3;

    float32 denom5 = powf(norm_squared(d), 5.f/2.f);
    float32 Gm1_d5 = PHYS_G*M1/denom5;
    float32 Gm2_d5 = PHYS_G*M2/denom5;

    // ====== Make Jacobian here ======
    float32 J[6*6] = {};
    // ================================

    float32 I_hJ[6*6];
    for (int i = 1; i <= 6; i++)
        for (int j = 1; j <= 6; j++)
            I_hJ[Ix(i, j)] = ((i == j) ? 1.0f : 0.f) - h*J[Ix(i, j)];

    float32 F0[6];
    F0[0] = Gm2_d3 * d.x;
    F0[1] = Gm2_d3 * d.y;
    F0[2] = Gm2_d3 * d.z;
    F0[3] = Gm1_d3 * (-d.x);
    F0[4] = Gm1_d3 * (-d.y);
    F0[5] = Gm1_d3 * (-d.z);

    // Initial guess of dY (hF(Y0))
    float32 dY0_[6];
    float32 dY1_[6];

    float32 *dY0 = dY0_;
    float32 *dY1 = dY1_;

    dY0[0] = h * F0[0];
    dY0[1] = h * F0[1];
    dY0[2] = h * F0[2];
    dY0[3] = h * F0[3];
    dY0[4] = h * F0[4];
    dY0[5] = h * F0[5];

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
                }
            }

            dY1[i] = (h*F0[i] - acc) / I_hJ[Ix(i+1, i+1)];
        }

        float32 *tmp = dY0;
        dY0 = dY1;
        dY1 = tmp;
    }

    float32 new_vx1 = V1.x + dY0[0];
    float32 new_vy1 = V1.y + dY0[1];
    float32 new_vz1 = V1.z + dY0[2];

    float32 new_vx2 = V2.x + dY0[3];
    float32 new_vy2 = V2.y + dY0[4];
    float32 new_vz2 = V2.z + dY0[5];

    // P
    out[0].P.x = M1 * new_vx1;
    out[0].P.y = M1 * new_vy1;
    out[0].P.z = M1 * new_vz1;

    out[1].P.x = M2 * new_vx2;
    out[1].P.y = M2 * new_vy2;
    out[1].P.z = M2 * new_vz2;

    // X
    out[0].X.x = X1.x + h * new_vx1;
    out[0].X.y = X1.y + h * new_vy1;
    out[0].X.z = X1.z + h * new_vz1;

    out[1].X.x = X2.x + h * new_vx2;
    out[1].X.y = X2.y + h * new_vy2;
    out[1].X.z = X2.z + h * new_vz2;

    for (uint32 i = 0; i < count; i++)
    {
        float32 M = in[i].M;
        quaternion Q = in[i].Q;
        vector3 L = in[i].L;
        vector3 I = in[i].I;

        /*
                    | y^2 + z^2    -yx       -zx    |
            I = int(| -xy       x^2 + z^2    -zy    |)
                    | -xz          -yz    x^2 + y^2 |

                       | b^2 + c^2     0         0     |
            I = 1/12 V |     0     a^2 + c^2     0     |
                       |     0         0     a^2 + b^2 |
        */
        matrix3 invI0 = {};
        invI0._11 = I.x;
        invI0._22 = I.y;
        invI0._33 = I.z;

        vector3 omega = apply_unit_quaternion(Q,
            invI0 * apply_unit_quaternion(conjugated(Q), L));

        Q += 0.5f * quaternion::pure(omega) * Q * h;
        normalize(Q);

        out[i].Q = Q;
        out[i].L = L;
        out[i].I = I;
        out[i].M = M;
    }
}


void swap_buffers(phys::world *world)
{
    rigid_body *tmp = world->Y0;
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
                    world->count,
                    PHYS_DT);
        world->residual_time -= PHYS_DT;
        swap_buffers(world);
    }
}

} // namespace phys
