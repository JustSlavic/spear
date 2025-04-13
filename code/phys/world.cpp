#include "world.hpp"

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

matrix make_inertia_tensor(float64 a, float64 b, float64 c)
{
    matrix I = {};
    I._11 = b*b + c*c;
    I._22 = a*a + c*c;
    I._33 = a*a + b*b;

    float64 V = a*b*c;
    return (1/12.0) * V * I;
}

matrix cross(vector v)
{
    matrix result = {};
    result._12 = -v.z; result._13 = v.y;
    result._21 = v.z; result._23 = -v.x;
    result._31 = -v.y; result._32 = v.x;
    return result;
}

void simulate_step(phys::world *world)
{
    for (int body_index = 0; body_index < world->body_count; body_index++)
    {
        body *b = world->bodies + body_index;
        b->X += PHYS_DT * b->P / b->M;

        matrix R = b->R;

        float64 det = determinant(R);
        console::print("det = %f\n", det);

        matrix I0 = make_inertia_tensor(1.4, 0.7, 2.1);

        matrix invI0 = I0;
        invI0._11 = 1.0/invI0._11;
        invI0._22 = 1.0/invI0._22;
        invI0._33 = 1.0/invI0._33;

        console::print("I0 = |%f, %f, %f|\n"
                       "     |%f, %f, %f|\n"
                       "     |%f, %f, %f|\n",
            I0._11, I0._12, I0._13,
            I0._21, I0._22, I0._23,
            I0._31, I0._32, I0._33);
        console::print("invI0 = |%f, %f, %f|\n"
                       "        |%f, %f, %f|\n"
                       "        |%f, %f, %f|\n",
            invI0._11, invI0._12, invI0._13,
            invI0._21, invI0._22, invI0._23,
            invI0._31, invI0._32, invI0._33);

        vector omega = R * invI0 * transposed(R) * b->L;
        matrix cross_omega = cross(omega);
        b->R += cross_omega * R * PHYS_DT;
    }
}

void update_world(phys::world *world, float32 dt)
{
    world->residual_time += dt;
    while (world->residual_time >= PHYS_DT)
    {
        simulate_step(world);
        world->residual_time -= PHYS_DT;
    }
}

} // namespace phys
